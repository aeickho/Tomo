#pragma once

#include "tomo/geometry/prim/BoundingBox.hpp"

#include <vector>

namespace tomo
{
  namespace geometry
  {
    namespace aux
    {
      template <typename PRIMITIVE, 
                int DIMENSIONS = 3, 
                typename SCALAR = base::DEFAULT_TYPE>
      union KDNode
      {
        typedef SCALAR scalar_type;
        typedef std::vector<PRIMITIVE*> PrimCont;
        typedef Ray<DIMENSIONS,scalar_type> ray_type;
        typedef base::Vec<DIMENSIONS,scalar_type> vector_type;

        bool isLeaf() const
        {
          return !inner_.leafFlag();
        }

        /// Inner node
        struct Inner
        {
          typedef std::vector<KDNode> NodeCont;

          inline bool leafFlag() const
          {
            return data_ & (1 << 31);
          }
          inline base::Axis axis() const
          {
            return base::Axis(data_ & 3);
          }

          /// Insert children
          inline void setup(NodeCont& _nodes, base::Axis _axis, scalar_type _splitPos)
          {
            data_ = (0x7FFFFFFC & (_nodes.size() << 2)) | (1 << 31) | _axis;
            splitPos_ = _splitPos;
          }

          inline unsigned left() const
          {
            return (data_ & 0x7FFFFFFC) >> 2;
          }
          inline unsigned right() const
          {
            return ((data_ & 0x7FFFFFFC) >> 2) + 1;
          }

          inline scalar_type splitPos() const
          {
            return splitPos_;
          }

        private:
          unsigned int data_;
          scalar_type splitPos_;
        };

        /// Leaf node
        struct Leaf
        {
          typedef std::vector<PRIMITIVE*> PrimCont;

          /// Insert Primitive pointers from _src into _dest
          inline void insert(const PrimCont& _src, PrimCont& _dest)
          {
            // Make leaf node
            offset_ = _dest.size();
            size_ = _src.size();
            _dest.resize(offset_ + size_);
            for (unsigned i = 0; i < size_; i++)
              _dest[offset_+i] = _src[i];
          }

          inline PrimCont primitives(const PrimCont& _primLists) const
          {
            PrimCont _primList;
            _primList.resize(size_);
            for (unsigned i = offset_, j = 0; i < offset_ + size_; i++, j++)
              _primList[j] = _primLists[i];
            return _primList;
          }

        private:
          unsigned int size_;
          unsigned int offset_;
        };

        Inner inner_;
        Leaf leaf_;
      };

      template <typename PRIMITIVE,
               int DIMENSIONS = 3,
               typename SCALAR = base::DEFAULT_TYPE>
      struct KDTree
      {
        /// Node type
        typedef KDNode<PRIMITIVE,DIMENSIONS,SCALAR> Node;
        typedef typename KDNode<PRIMITIVE>::Inner NodeInner;

        typedef SCALAR scalar_type;
        typedef base::Vec<DIMENSIONS,scalar_type> vector_type;
        typedef Bounds<DIMENSIONS,scalar_type> bounds_type;
        typedef Ray<DIMENSIONS,scalar_type> ray_type;

        /// Node container
        typedef std::vector<Node> NodeCont;

        /// Primitive container
        typedef std::vector<PRIMITIVE*> PrimCont;

        NodeCont nodes_;
        PrimCont primLists_;
        bounds_type bounds_;

        struct NodeStackElement
        {
          bounds_type bounds_;
          PrimCont primList_;
          unsigned node_;
          int depth_;

          Node* node(NodeCont& _nodes)
          {
            return &_nodes[node_];
          }
        };

        const Node* root() const
        {
          return &nodes_[0];
        }
        Node* root()
        {
          return &nodes_[0];
        }

        const Node* node(unsigned _nodeIndex) const
        {
          return &nodes_[_nodeIndex];
        }

        /// Iterative traversal with visitor
        template<typename VISITOR>
        bool traversal(VISITOR& _visitor) const
        {
          typedef typename VISITOR::State State;

          State _stack[16];
          int _stackPt = -1;

          if (!_visitor.root()) return false;
          bool _found = false;

          while (1)
          {
            while (!_visitor.state().node()->isLeaf())
            {
              _stackPt += _visitor.inner(_stack[_stackPt+1]); 
            }
           
            _found |= _visitor.leaf();
            if (_found) return true;
            if (_stackPt < 0) return _found;

            _visitor.state(_stack[_stackPt]);
            _stackPt--;
          }

          return _found;
        }


        void build(std::vector<PRIMITIVE>& _objs, unsigned _primitivesPerNode = 10)
        {
          primLists_.clear();
          nodes_.clear();
          nodes_.resize(1);
          PrimCont _nodeObjs;
          _nodeObjs.reserve(_objs.size());

          bounds_type _bounds;
          for (unsigned i = 0; i < _objs.size(); i++)
          {
            _bounds.extend(_objs[i].bounds());
            _nodeObjs.push_back(&_objs[i]);
          } 
          bounds_ = _bounds;
          divideNode(0,bounds_,_nodeObjs,0,_primitivesPerNode);
        }

        virtual void divideNode(unsigned nodeIndex, bounds_type _bounds, PrimCont& _nodeObjs, unsigned depth, unsigned _primitivesPerNode)
        {
          if (depth >= maxDepth() || _nodeObjs.size() <= _primitivesPerNode )
          {
            // We have a leaf node!
            nodes_[nodeIndex].leaf_.insert(_nodeObjs,primLists_);
            return;
          }

          // Setup node
          base::Axis _axis = _bounds.dominantAxis();
          float _splitPos = 0.5*(_bounds.min()[_axis] + _bounds.max()[_axis]);

          nodes_[nodeIndex].inner_.setup(nodes_,_axis,_splitPos);
          bounds_type _leftBounds, _rightBounds;
          nodes_.resize(nodes_.size()+2);

          PrimCont _leftObjs, _rightObjs;
          insertion(_splitPos,_axis,_bounds,_nodeObjs,_leftObjs,_rightObjs,_leftBounds,_rightBounds);

          _nodeObjs.clear();
          divideNode(nodes_[nodeIndex].inner_.left(),_leftBounds,_leftObjs,depth+1,_primitivesPerNode);
          divideNode(nodes_[nodeIndex].inner_.right(),_rightBounds,_rightObjs,depth+1,_primitivesPerNode);
        }

        inline void insertion( scalar_type _splitPos,
                               base::Axis _axis,
                               bounds_type& _bounds,
                               PrimCont& _primList,
                               PrimCont& _leftList,
                               PrimCont& _rightList,
                               bounds_type& _leftBounds,
                               bounds_type& _rightBounds)
        {
          _bounds.split(_splitPos,_axis,_leftBounds,_rightBounds);
          for (unsigned i = 0; i < _primList.size(); i++)
          {
            PRIMITIVE* _prim = _primList[i];
            ///@todo Replace this by functors
            prim::SplitPlaneIntersect _result = _prim->intersect(_axis,_splitPos,_leftBounds,_rightBounds);

            if (_result.left())  _leftList.push_back(_prim);
            if (_result.right()) _rightList.push_back(_prim);
          }
          _primList.clear();
        }

        virtual float splitPos(const PrimCont& _primList, NodeInner* _inner, const bounds_type& _bounds) const
        {
          return 0.5*(_bounds.min()[_inner->axis()] + _bounds.max()[_inner->axis()]);
        }

        static unsigned maxDepth()
        {
          return 16;
        }


      };

    }
  }
}
