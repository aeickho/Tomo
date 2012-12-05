#pragma once

namespace tomo
{
  namespace geometry
  {
    namespace kd
    {
#define TOMO_INHERIT_STATE_TYPES(state_type) \
        typedef typename state_type::node_type node_type;\
        TOMO_INHERIT_NODE_TYPES(node_type)

      template<typename NODE>
      struct BuildState
      {
        BuildState() : depth_(0), nodeIndex_(0) {}

        typedef NODE node_type;
        TOMO_INHERIT_NODE_TYPES(node_type)
        typedef struct {} input_type;

        /**@brief Defines how to initialize the state from a certain input
          *@param _stackPos If stackPos == 0, it setups initial state,
          *                 otherwise, it will be state of the build stack
          */
        void init(const input_type& _input, unsigned _stackPos)
        {
        }

        /// Default behaviour of a state change when building a kdtree
        void change(inner_node_type _innerNode, BuildState& _stateToPush)
        {
          /// Increase depth as we have split the node before
          depth_++;
          nodeIndex_= _innerNode.left();

          // Initialize state to be pushed
          _stateToPush.depth(depth_);
          _stateToPush.nodeIndex(_innerNode.right());
          // Make bounds for subnodes
          nodeGeometry_.bounds().split(
              _innerNode.splitPos(),
              _innerNode.axis(),
              nodeGeometry_.bounds(),
              _stateToPush.nodeGeometry().bounds());
        }

        TBD_PROPERTY(unsigned,depth)
        TBD_PROPERTY(unsigned,nodeIndex)
        TBD_PROPERTY_REF(node_geometry_type,nodeGeometry)
      };
    }
  }
}
