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
          TOMO_INHERIT_STATE_TYPES(state_base_type)

          typedef PRIMITIVE primitive_type;
          typedef std::vector<primitive_type> input_type;
          typedef std::vector<const primitive_type*> primitive_cntr_type;

          void init(const input_type& _input, unsigned _stackPos)
          {
            if (_stackPos == 0)
            {
              primitives_.clear();
              primitives_.reserve(_input.size());
              bounds_type& _bounds = state_base_type::nodeGeometry().bounds();
              _bounds = bounds_type();
              // Fill initial primitive list with pointers of input objects
              // and calculate bounds on the fly
              for (auto it = _input.begin() ; it != _input.end() ; ++it )
              {
                _bounds.extend(it->bounds());
                primitives_.push_back(&(*it));
              }
            }
            else
            {
              // Reserves n / 2^p items on stack
              // n = number of primitives (_input.size()), p = _stackPos-1
              // Ensures that memory is allocated a priori and 
              // memory requirements are independent from MAX_DEPTH of a kd-tree 
              primitives_.reserve(_input.size() >> (_stackPos-1));
            }
          }

          void change(const inner_node_type& _innerNode,
                      const node_geometry_type& _nodeGeometry,
                      BuildState& _stateToPush)
          {
            state_base_type::change(_innerNode,_nodeGeometry,_stateToPush);
            _stateToPush.primitives().clear();

            // Insert objects of current state into left and right subnode
            auto it = primitives_.begin(), _leftIt = it;
            for (; it != primitives_.end() ; ++it)
            {
              NodeIntersectResult _result = PRIM_NODE_INTERSECTION()(*it,_nodeGeometry);
              if (_result.right()) _stateToPush.primitives().push_back(*it);
              if (_result.left())
              {
                *_leftIt = *it;
                ++_leftIt;
              }
            }
            // Erase remaining objects at back of container
            primitives_.erase(_leftIt,primitives_.end());
          }

          TBD_PROPERTY_REF(primitive_cntr_type,primitives)
        };
      }
    }
  }
}
