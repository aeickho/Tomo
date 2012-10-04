#pragma once

#include "tomo/geometry/aux/KDNode.hpp"
#include "tomo/geometry/prim/BoundingBox.hpp"

namespace tomo
{
  namespace geometry
  {
    namespace aux
    {
      template <typename PRIMITIVE>
      struct KDTree
      {
        /// Node type
        typedef KDNode<PRIMITIVE> Node;
        typedef typename KDNode<PRIMITIVE>::Inner NodeInner;
        typedef typename PRIMITIVE::scalar_type scalar_type;
        typedef typename PRIMITIVE::bounds_type vec_type;
        typedef typename PRIMITIVE::bounds_type bounds_type;
        typedef typename PRIMITIVE::ray_type ray_type;

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
