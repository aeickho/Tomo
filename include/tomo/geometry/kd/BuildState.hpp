#pragma once

namespace tomo
{
  namespace geometry
  {
    namespace kd
    {
#define TOMO_INHERIT_STATE_TYPES() \
        typedef typename state_base_type::node_type node_type;\
        typedef typename state_base_type::inner_node_type inner_node_type;\
        typedef typename state_base_type::bounds_type bounds_type;\
        typedef typename state_base_type::scalar_type scalar_type;\
        typedef typename state_base_type::node_geometry_type node_geometry_type;

      template<typename NODE>
      struct BuildState
      {
        BuildState() : depth_(0), nodeIndex_(0) {}

        typedef NODE node_type;
        typedef typename node_type::Inner inner_node_type;
        typedef typename NODE::geometry_type node_geometry_type;
        typedef typename node_geometry_type::bounds_type bounds_type;
        typedef typename node_type::scalar_type scalar_type;
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
          bounds_.split(_innerNode.splitPos(),_innerNode.axis(),bounds_,_stateToPush.bounds());
        }

        TBD_PROPERTY(unsigned,depth)
        TBD_PROPERTY(unsigned,nodeIndex)
        TBD_PROPERTY_REF(bounds_type,bounds)
      };
    }
  }
}
