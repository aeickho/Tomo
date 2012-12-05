#pragma once

#include "Node.hpp"
#include <array>

namespace tomo
{
  namespace geometry
  {
    namespace kd
    {
      template
      <
        typename NODE_CONTAINER,
        typename BUILD_POLICY,
        unsigned MAX_DEPTH = 16
      >
      struct Tree
      {        
        /// Node container types
        typedef NODE_CONTAINER node_cntr_type;
        typedef typename node_cntr_type::node_type node_type;
        typedef typename node_type::Inner inner_node_type;
        typedef typename node_type::Leaf leaf_node_type;
        typedef typename node_type::node_geometry_type node_geometry_type;
        typedef BUILD_POLICY build_policy_type;
        typedef typename build_policy_type::state_type state_type; 
        typedef typename state_type::input_type input_type;
        typedef typename state_type::bounds_type bounds_type;
        typedef typename node_type::scalar_type scalar_type;

        Tree() {}
        Tree(const input_type& _input) { build(_input); }

        bool empty() const
        {
          return nodes_.empty();
        }

        void clear()
        {
          nodes_.clear();
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

        void build(const input_type& _input)
        {
          typedef BUILD_POLICY build_policy_type;
          build_policy_type _buildPolicy;

          typedef typename build_policy_type::state_type BuildState;
          typedef std::array<BuildState,MAX_DEPTH> BuildStack;
          int _stackPt = 0;

          // Declare state and reserve memory for each stack item
          BuildStack _stack;
          int _stackItemIndex = 0;
          for ( BuildState& _stackItem : _stack)
          {
            _stackItem.init(_input,_stackItemIndex);
            ++_stackItemIndex;
          }

          BuildState& _state = _stack[0];
          bounds_ = _state.nodeGeometry().bounds();

          /// Initialize node container
          nodes_.init(_state);

          while (1)
          {
            while (_state.depth() < MAX_DEPTH)
            {
              // Break if node can't be split
              if (!_buildPolicy.split(_state)) break;

              // Make an inner node
              inner_node_type& _innerNode = nodes_.insertInner(_state);

              // Change state
              _stackPt++;
              BuildState& _right = _stack[_stackPt];
              _state.change(_innerNode,_right);
            }

            // We have a leaf node!
            nodes_.insertLeaf(_state);

            // Nothing left to do
            if (_stackPt <= 0) return;

            _stack[0] = _stack[_stackPt];
            _stackPt--;
          }
        }

        TBD_PROPERTY_REF(bounds_type,bounds);
        TBD_PROPERTY_REF_RO(node_cntr_type,nodes)  
      };
    }
  }
}
