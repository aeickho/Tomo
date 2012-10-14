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
          primLists_.reserve(_objs.size()*2);
          nodes_.clear();
          nodes_.reserve(2*_objs.size()/_primitivesPerNode);
          nodes_.resize(1);
          PrimCont _nodeObjs;
          _nodeObjs.reserve(_objs.size());

          typename std::vector<PRIMITIVE>::iterator it;
          bounds_type _bounds;
          for (it = _objs.begin() ; it != _objs.end() ; ++it )
          {
            _bounds.extend(it->bounds());
            _nodeObjs.push_back(&(*it));
          } 
          bounds_ = _bounds;
          divideNode(0,bounds_,_nodeObjs,0,_primitivesPerNode);
        }

        void divideNode(unsigned nodeIndex, bounds_type _bounds, PrimCont& _primList, unsigned depth, unsigned _primitivesPerNode)
        {
          if (depth >= maxDepth() || _primList.size() <= _primitivesPerNode )
          {
            // We have a leaf node!
            nodes_[nodeIndex].leaf_.insert(_primList,primLists_);
            return;
          }

          // Setup node
          base::Axis _axis = _bounds.dominantAxis();
          scalar_type _splitPos = (_bounds.min()[_axis] + _bounds.max()[_axis])/2;

          nodes_[nodeIndex].inner_.setup(nodes_,_axis,_splitPos);
          bounds_type _leftBounds, _rightBounds;
          nodes_.resize(nodes_.size()+2);

          PrimCont _leftList, _rightList;
          _bounds.split(_splitPos,_axis,_leftBounds,_rightBounds);
          
          for (typename PrimCont::iterator it = _primList.begin(); 
              it != _primList.end() ; ++it)
          {
            prim::SplitPlaneIntersect _result = (*it)->intersect(_axis,_splitPos,_leftBounds,_rightBounds);
            if (_result.left())  _leftList.push_back(*it);
            if (_result.right()) _rightList.push_back(*it);
          }
          _primList.clear();

          divideNode(nodes_[nodeIndex].inner_.left(),_leftBounds,_leftList,depth+1,_primitivesPerNode);
          divideNode(nodes_[nodeIndex].inner_.right(),_rightBounds,_rightList,depth+1,_primitivesPerNode);
        }

        scalar_type splitPos(const PrimCont& _primList, NodeInner* _inner, const bounds_type& _bounds) const
        {
          return (_bounds.min()[_inner->axis()] + _bounds.max()[_inner->axis()])/2;
        }

        static unsigned maxDepth()
        {
          return 16;
        }
      };

    }
  }
}
