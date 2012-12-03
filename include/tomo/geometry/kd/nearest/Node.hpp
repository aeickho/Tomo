#pragma once

#include "tomo/geometry/kd/Node.hpp"

namespace tomo
{
  namespace geometry
  {
    namespace kd
    {
      namespace nearest
      {
        /// An attribute which holds a pointer to a primitive
        template<typename PRIMITIVE>
        struct PrimitivePtrAttribute
        {
          typedef PRIMITIVE primitive_type;
          TBD_PROPERTY(const primitive_type*,primitive)
        };

        /**@brief A Node which has both for inner and for leaf a Pointer to a primitive.
         *@detail Moreover, a number of objects can be stored in a leaf node.
        **/
        template<typename PRIMITIVE>
        struct Node : 
          kd::Node<
            typename PRIMITIVE::model_type,
            PrimitivePtrAttribute<PRIMITIVE>,
            EmptyAttribute,
            RangeAttribute
          >
        {
          typedef PRIMITIVE primitive_type;
          typedef typename primitive_type::model_type model_type;
        };
      }
    }
  }
}
