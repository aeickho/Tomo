#pragma once

#include "Node.hpp"
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
          typename PRIMITIVE,
          typename PRIM_NODE_INTERSECTION
        >
        struct BuildState : kd::BuildState<Node<PRIMITIVE>>
        {
          typedef kd::BuildState<Node<PRIMITIVE>> state_base_type;
          TOMO_INHERIT_STATE_TYPES(state_base_type)

          typedef PRIMITIVE primitive_type;
          typedef std::vector<primitive_type> input_type;
          typedef std::vector<const primitive_type*> primitive_cntr_type;

          void init(const input_type& _input, unsigned _stackPos)
          {
            if (_stackPos == 0)
            {
              primitives_.clear();
              // Reserve appropriate number of primitives
              primitives_.reserve(_input.size());
              bounds_type& _bounds = state_base_type::nodeGeometry().bounds();
              _bounds = bounds_type();
              // Fill initial primitive list with pointers of input objects
              // and calculate bounds on the fly
              for (const auto& _primitive : _input)
              {
                _bounds.extend(_primitive.bounds());
                primitives_.push_back(&_primitive);
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
                      BuildState& _stateToPush)
          {
            _stateToPush.primitives().clear();

            // Insert objects of current state into left and right subnode
            auto _leftIt = primitives_.begin();
            for (auto& _primitive : primitives_) 
            {
              NodeIntersectResult _result = 
                PRIM_NODE_INTERSECTION()(*_primitive,state_base_type::nodeGeometry());
              if (_result.right()) _stateToPush.primitives().push_back(_primitive);
              if (_result.left())
              {
                *_leftIt = _primitive;
                ++_leftIt;
              }
            }
            // Erase remaining objects at back of container
            primitives_.erase(_leftIt,primitives_.end());
            
            // Call base-class change function
            // to induce default state change behaviour
            state_base_type::change(_innerNode,_stateToPush);
          }

          TBD_PROPERTY_REF(primitive_cntr_type,primitives)
        };
      }
    }
  }
}
