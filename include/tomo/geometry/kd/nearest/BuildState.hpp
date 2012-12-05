#pragma once

#include "Node.hpp"
#include "tomo/geometry/kd/object/BuildState.hpp"

namespace tomo
{
  namespace geometry
  {
    namespace kd
    {
      namespace nearest
      {
        template<
          typename PRIMITIVE,
          typename PRIM_NODE_INTERSECTION
        >
        struct BuildState: object::BuildState<Node<PRIMITIVE>,PRIMITIVE,PRIM_NODE_INTERSECTION>
        {
          typedef object::BuildState<Node<PRIMITIVE>,PRIMITIVE,PRIM_NODE_INTERSECTION> state_base_type;
          TOMO_INHERIT_STATE_TYPES(state_base_type)

          BuildState() : 
            state_base_type(), 
            primitive_(nullptr) {}

          typedef typename state_base_type::primitive_type primitive_type;
          typedef typename state_base_type::primitive_cntr_type primitive_cntr_type;

          void change(const inner_node_type& _innerNode,
                      const node_geometry_type& _nodeGeometry,                       
                      BuildState& _stateToPush)
          {
            kd::BuildState<node_type>::change(_innerNode,_nodeGeometry,_stateToPush);
            _stateToPush.primitives().clear();

            primitive_cntr_type& _primitives = state_base_type::primitives();

            // Insert objects of current state into left and right subnode
            auto it = _primitives.begin(), _leftIt = it;
            for (; it != _primitives.end() ; ++it)
            {
              if ((*it) == primitive_ && primitive_ != nullptr) continue;

              NodeIntersectResult _result = PRIM_NODE_INTERSECTION()(**it,_nodeGeometry);
              if (_result.right()) _stateToPush.primitives().push_back(*it);
              if (_result.left())
              {
                *_leftIt = *it;
                ++_leftIt;
              }
            }
            // Erase remaining objects at back of container
            _primitives.erase(_leftIt,_primitives.end());
          }

          TBD_PROPERTY(primitive_type*,primitive)
        };
      }
    }
  }
}
