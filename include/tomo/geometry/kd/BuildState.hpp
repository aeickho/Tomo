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

        void init(const input_type& _input, unsigned _stackPos)
        {
        }

        void change(const inner_node_type& _innerNode,
                    const node_geometry_type& _nodeGeometry,
                    BuildState& _stateToPush)
        {
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
