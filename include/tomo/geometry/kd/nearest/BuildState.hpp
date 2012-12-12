#pragma once

#include "Node.hpp"
#include "nodeIntersection.hpp"
#include "tomo/geometry/kd/split/pos.hpp"
#include "tomo/geometry/kd/BuildState.hpp"

namespace tomo
{
  namespace geometry
  {
    namespace kd
    {
      namespace nearest
      {        
        template<typename PRIMITIVE>
        struct BuildState: kd::BuildState<Node<PRIMITIVE>>
        {
          typedef kd::BuildState<Node<PRIMITIVE>> state_base_type;
          TOMO_INHERIT_STATE_TYPES(state_base_type)

          BuildState() : 
            state_base_type(), 
            primitive_(nullptr) {}

          typedef PRIMITIVE primitive_type;
          typedef std::vector<primitive_type> input_type;
          typedef std::vector<const primitive_type*> primitive_cntr_type;

          void init(const input_type& _input, unsigned _stackPos)
          {
            primitive(nullptr);
            if (_stackPos == 0)
            {
              primitives_.clear();
              primitives_.reserve(_input.size());
              bounds_type& _bounds = state_base_type::nodeGeometry().bounds();
              _bounds = bounds_type();
              // Fill initial primitive list with pointers of input objects
              // and calculate bounds on the fly
              for (const auto& _primitive : _input)
              {
                typename primitive_type::point_type 
                  _p(split::pos(_primitive,base::X),split::pos(_primitive,base::Y));
                _bounds.extend(_p);
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

            primitive_cntr_type& _primitives = primitives();

            // Insert objects of current state into left and right subnode
            auto it = _primitives.begin(), _leftIt = it;
            for (; it != _primitives.end() ; ++it)
            {
  //            if ((*it) == primitive_ && primitive_ != nullptr) continue;

              NodeIntersectResult _result = 
                nearest::nodeIntersection(**it,state_base_type::nodeGeometry());
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

          
          TBD_PROPERTY(const primitive_type*,primitive)
          TBD_PROPERTY_REF(primitive_cntr_type,primitives)
        };
        
      }
    }
  }
}
