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
        struct BuildState: kd::BuildState<Node<PRIMITIVE>>
        {
          typedef kd::BuildState<Node<PRIMITIVE>> state_base_type;
          TOMO_INHERIT_STATE_TYPES(state_base_type)

          BuildState() : 
            state_base_type(), 
            primitive_(nullptr) {}

          typedef PRIMITIVE primitive_type;
          typedef std::vector<primitive_type*> primitive_cntr_type;

          void change(const inner_node_type& _innerNode,
                      BuildState& _stateToPush)
          {
            _stateToPush.primitives().clear();

            primitive_cntr_type& _primitives = primitives();

            // Insert objects of current state into left and right subnode
            auto it = _primitives.begin(), _leftIt = it;
            for (; it != _primitives.end() ; ++it)
            {
              if ((*it) == primitive_ && primitive_ != nullptr) continue;

              NodeIntersectResult _result = 
                PRIM_NODE_INTERSECTION()(**it,state_base_type::nodeGeometry());
              if (_result.right()) _stateToPush.primitives().push_back(*it);
              if (_result.left())
              {
                *_leftIt = *it;
                ++_leftIt;
              }
            }
            // Erase remaining objects at back of container
            _primitives.erase(_leftIt,_primitives.end());
            
            state_base_type::change(_innerNode,_stateToPush);
          }

          TBD_PROPERTY(primitive_type*,primitive)
          TBD_PROPERTY_REF(primitive_cntr_type,primitives)
        };
      }
    }
  }
}
