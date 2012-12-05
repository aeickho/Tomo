#pragma once

#include "tomo/geometry/kd/Node.hpp"

namespace tomo
{
  namespace geometry
  {
    namespace kd
    {
      namespace object
      {
        /// A Node in which a number of objects can be stored in a leaf node
        template<typename PRIMITIVE>
        struct Node : kd::Node<
            typename PRIMITIVE::model_type,
            /* Node attribute*/       EmptyAttribute,
            /* Inner node attribute*/ EmptyAttribute,
            /* Leaf node attribute*/  RangeAttribute>         
        {
          typedef PRIMITIVE primitive_type;
          typedef typename primitive_type::model_type model_type;
          typedef kd::Node<
            typename PRIMITIVE::model_type,
            EmptyAttribute,
            EmptyAttribute,
            RangeAttribute>  base_node_type;
          
          TOMO_INHERIT_NODE_TYPES(base_node_type)
        };
      }
    }
  }
}

