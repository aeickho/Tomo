#pragma once

#include "KDNode.hpp"
#include "KDNodeGeometry.hpp"

namespace tomo
{
  namespace geometry
  {
    namespace aux
    {
      template 
        <
          typename PRIMITIVE,
          unsigned MAX_DEPTH = 16
        >
      struct KDTree
      {   
        /// Node type
        typedef KDNode<PRIMITIVE> Node;

        typedef typename KDNode<PRIMITIVE>::Inner NodeInner;
        typedef typename PRIMITIVE::scalar_type scalar_type;
        typedef typename PRIMITIVE::bounds_type vec_type;
        typedef typename PRIMITIVE::bounds_type bounds_type;

        /// Node container
        typedef std::vector<Node> NodeCont;

        /// Primitive container
        typedef std::vector<PRIMITIVE*> PrimCont;

        NodeCont nodes_;
        PrimCont primLists_;
        bounds_type bounds_;

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

          /// Insert Primitive pointers from _src into primLists_
        void insertLeafNode(unsigned _nodeIndex, const PrimCont& _src)
        {
          nodes_[_nodeIndex].leaf_.offset(primLists_.size());
          nodes_[_nodeIndex].leaf_.size(_src.size());
          primLists_.insert(primLists_.end(),_src.begin(),_src.end());
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

        template< typename KDNODE_INTERSECTOR >
        void build(std::vector<PRIMITIVE>& _objs, unsigned _primitivesPerNode = 10)
        {
          // Clear data containers and reserve memory 
          primLists_.clear();
          primLists_.reserve(_objs.size()*2);
          nodes_.clear();
          nodes_.reserve(2*_objs.size()/_primitivesPerNode);
          nodes_.resize(1);

          // State holder struct 
          // needed for stack for iterative build and initial state
          struct State
          {
            State() : depth_(0), nodeIndex_(0) {}
            unsigned depth_;
            unsigned nodeIndex_;
            TBD_PROPERTY_REF(bounds_type,bounds);
            TBD_PROPERTY_REF(PrimCont,primList);
          } _state;
          
          int _stackPt = -1;

          // Declare state and reserve memory for each stack item
          State _stack[MAX_DEPTH];
          for (int i = 0; i < MAX_DEPTH; i++) 
            _stack[i].primList().reserve(_objs.size() >> i);

          // Fill initial primitive list with pointers of input objects
          // and calculate bounds on the fly
          _state.primList().reserve(_objs.size());
          for (auto it = _objs.begin() ; it != _objs.end() ; ++it )
          {
            _state.bounds().extend(it->bounds());
            _state.primList().push_back(&(*it));
          } 
          bounds_ = _state.bounds();

          while (1)
          {
            while (_state.depth_ < MAX_DEPTH && _state.primList().size() > _primitivesPerNode )
            {
              // Setup node
              nodes_.resize(nodes_.size()+2);
              Node* _node = &nodes_[_state.nodeIndex_];
              base::Axis _axis = _state.bounds().dominantAxis();
              scalar_type _splitPos = (_state.bounds().min()[_axis] + _state.bounds().max()[_axis])/2; ///@todo Insert for split plane intersection functor here
              _node->inner_.setup(nodes_,_axis,_splitPos);

              // Change state
              _state.depth_++;
              _state.nodeIndex_ = _node->inner_.left();
             
              // Initialize state to be pushed
              _stackPt++;
              State& _right = _stack[_stackPt];
              _right.primList().clear();
              _right.depth_ = _state.depth_;
              _right.nodeIndex_ = _state.nodeIndex_+1; /// Equal to _node->inner_.right(), but faster ;)
             
              // Make node geometry
              KDNodeGeometry<PRIMITIVE> _nodeGeometry(_state.bounds(),_axis,_splitPos);
              
              // Split node
              _state.bounds().split(_splitPos,_axis,_state.bounds(),_right.bounds());
             
              KDNODE_INTERSECTOR _nodeIntersector;

              // Insert objects of current state into left and right subnode
              auto it = _state.primList().begin(), _leftIt = it;
              for (; it != _state.primList().end() ; ++it)
              { 
                aux::KDNodeIntersectResult _result = _nodeIntersector(*(*it),_nodeGeometry);
                if (_result.right()) _right.primList().push_back(*it);
                if (_result.left()) 
                {
                  *_leftIt = *it; 
                  ++_leftIt; 
                } 
              }
              // Erase remaining objects at back of container 
              _state.primList().erase(_leftIt,_state.primList().end());
            }
           
            // We have a leaf node!
            insertLeafNode(_state.nodeIndex_,_state.primList());
            
            // Nothing left to do
            if (_stackPt < 0) return;

            _state = _stack[_stackPt];
            _stackPt--;
          }
        }
      };

    }
  }
}
