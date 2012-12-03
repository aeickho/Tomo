#pragma once

#include "tomo/geometry/kd/BuildState.hpp"

namespace tomo
{
  namespace geometry
  {
    namespace kd
    {
      namespace object
      {
        template
        <
          typename NODE,
          typename PRIMITIVE,
          typename PRIM_NODE_INTERSECTION
        >
        struct BuildState : kd::BuildState<NODE>
        {
          typedef kd::BuildState<NODE> state_base_type;
          TOMO_INHERIT_STATE_TYPES()

          typedef PRIMITIVE primitive_type;
          typedef std::vector<primitive_type> input_type;
          typedef std::vector<const primitive_type*> prim_cntr_type;

          void init(const input_type& _input, unsigned _stackPos)
          {
            if (_stackPos == 0)
            {
              primList_.clear();
              primList_.reserve(_input.size());
              state_base_type::bounds() = bounds_type();
              // Fill initial primitive list with pointers of input objects
              // and calculate bounds on the fly
              for (auto it = _input.begin() ; it != _input.end() ; ++it )
              {
                state_base_type::bounds().extend(it->bounds());
                primList_.push_back(&(*it));
              }
            }
            else
            {
              primList_.reserve(_input.size() >> (_stackPos-1));
            }
          }

          void change(const inner_node_type& _innerNode,
                      const node_geometry_type& _nodeGeometry,
                      BuildState& _stateToPush)
          {
            state_base_type::change(_innerNode,_nodeGeometry,_stateToPush);
            _stateToPush.primList().clear();

            // Insert objects of current state into left and right subnode
            auto it = primList_.begin(), _leftIt = it;
            for (; it != primList_.end() ; ++it)
            {
              NodeIntersectResult _result = PRIM_NODE_INTERSECTION()(*it,_nodeGeometry);
              if (_result.right()) _stateToPush.primList().push_back(*it);
              if (_result.left())
              {
                *_leftIt = *it;
                ++_leftIt;
              }
            }
            // Erase remaining objects at back of container
            primList_.erase(_leftIt,primList_.end());
          }

          TBD_PROPERTY_REF(prim_cntr_type,primList)
        };
      }
    }
  }
}
