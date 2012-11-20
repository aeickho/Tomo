#pragma once

#include "Node.hpp"

namespace tomo
{
  namespace geometry
  {
    namespace kd
    {
      template
      <
      typename PRIMITIVE,
               typename NODE = Node,
               unsigned MAX_DEPTH = 16
               >
      struct Tree
      {
        TOMO_INHERIT_MODEL_TYPES(PRIMITIVE)
        typedef NODE node_type;
        typedef typename node_type::Inner NodeInner;
        typedef typename node_type::Leaf NodeLeaf;
        typedef PRIMITIVE primitive_type;
        typedef NodeGeometry<model_type> geometry_type;

        /// Node container
        typedef std::vector<node_type> node_cntr_type;

        /// Primitive container
        typedef std::vector<const primitive_type*> prim_cntr_type;
        typedef typename prim_cntr_type::iterator prim_iterator;
        typedef typename prim_cntr_type::const_iterator prim_const_iterator;

        bool empty() const
        {
          return nodes_.empty();
        }
        void clear()
        {
          nodes_.clear();
          primLists_.clear();
        }
        const node_type* root() const
        {
          return &nodes_[0];
        }

        node_type* root()
        {
          return &nodes_[0];
        }

        const node_type* node(unsigned _nodeIndex) const
        {
          return &nodes_[_nodeIndex];
        }


        /// Iterative traversal with visitor
        template<typename VISITOR>
        bool traversal(VISITOR& _visitor) const
        {
          typedef typename VISITOR::State State;

          State _stack[MAX_DEPTH];
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

        template<typename BUILD_POLICY>
        void build(const std::vector<primitive_type>& _objs)
        {
          BUILD_POLICY _buildPolicy;

          // Clear data containers and reserve memory
          clear();
          primLists_.reserve(_objs.size()*2);
          nodes_.reserve(MAX_DEPTH*_objs.size()/2);
          nodes_.resize(1);

          // State holder struct
          // needed for stack for iterative build and initial state
          struct State
          {
            State() : depth_(0), nodeIndex_(0) {}
            unsigned depth_;
            unsigned nodeIndex_;

            void change(unsigned _depth, unsigned _nodeIndex)
            {
              depth_=_depth;
              nodeIndex_=_nodeIndex;
            }

            TBD_PROPERTY_REF(bounds_type,bounds);
            TBD_PROPERTY_REF(prim_cntr_type,primList);
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
            while (_state.depth_ < MAX_DEPTH)
            {
              geometry_type _nodeGeometry;

//              if (!_buildPolicy.split()(_state.bounds(),_state.primList(),_splitCandidate)) break;

              // Make an inner node
              NodeInner& _innerNode = innerNodeSetup(_state.nodeIndex_,_nodeGeometry);
              _buildPolicy.nodeAttributes(nodes_[_state.nodeIndex_]);

              // Change state (State represents left subnode from now on!)
              _state.change(_state.depth_++,_innerNode.left());

              // Initialize state to be pushed
              _stackPt++;
              State& _right = _stack[_stackPt];
              _right.change(_state.depth_,_innerNode.right());
              _right.primList().clear();

              // Make bounds for subnodes
              _state.bounds().split(_nodeGeometry.splitPos(),_nodeGeometry.axis(),_state.bounds(),_right.bounds());

              // Insert objects of current state into left and right subnode
              auto it = _state.primList().begin(), _leftIt = it;
              for (; it != _state.primList().end() ; ++it)
              {
                NodeIntersectResult _result = _buildPolicy.intersect()(*it,_nodeGeometry);

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
            leafNodeSetup(_state.nodeIndex_,_state.primList());

            // Nothing left to do
            if (_stackPt < 0) return;

            _state = _stack[_stackPt];
            _stackPt--;
          }
        }

        void leafRange(const NodeLeaf& _leaf, prim_iterator& _begin, prim_iterator& _end)
        {
          _begin = primLists_.begin() + _leaf.begin();
          _end = primLists_.begin() + _leaf.end();
        }

        void leafRange(const NodeLeaf& _leaf, 
                       prim_const_iterator& _begin, 
                       prim_const_iterator& _end) const
        {
          _begin = primLists_.begin() + _leaf.begin();
          _end = primLists_.begin() + _leaf.end();
        }

        TBD_PROPERTY_REF(bounds_type,bounds);
      private:
        NodeInner& innerNodeSetup(unsigned _nodeIndex, geometry_type& _nodeGeometry)
        {
          NodeInner& _inner = nodes_[_nodeIndex].inner_;
          nodes_.resize(nodes_.size()+2);
          uint32_t _index = nodes_.size();
          _inner.setup(_index,_nodeGeometry.axis(),_nodeGeometry.splitPos());
          return _inner;
        }

        /// Insert Primitive pointers from _primList into primLists_
        NodeLeaf& leafNodeSetup(unsigned _nodeIndex, const prim_cntr_type& _primList)
        {
          nodes_.resize(nodes_.size()+1);
          nodes_[_nodeIndex].leaf_.begin(primLists_.size());
          nodes_[_nodeIndex].leaf_.end(primLists_.size()+_primList.size());
          primLists_.insert(primLists_.end(),_primList.begin(),_primList.end());
          return nodes_[_nodeIndex].leaf_;
        }

        node_cntr_type nodes_;
        prim_cntr_type primLists_;
      };
    }
  }
}
