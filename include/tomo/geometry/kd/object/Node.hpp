#pragma once

"tomo/geometry/kd/Node.hpp"

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
        struct Node :
            kd::Node<typename PRIMITIVE::model_type,
            EmptyAttribute,
            EmptyAttribute,
            RangeAttribute>
        {
          typedef PRIMITIVE primitive_type;
          typedef typename primitive_type::model_type model_type;
        };
      }
    }
  }
}

