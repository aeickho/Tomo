#pragma once

"tomo/geometry/kd/Node.hpp"

namespace tomo
{
  namespace geometry
  {
    namespace kd
    {
      namespace coeff
      {
        /// Attribute which holds certain integer data and a float coefficient
        struct DataCoefficientAttribute
        {
          TBD_PROPERTY(uint32_t,data)
          TBD_PROPERTY(float,coeff)
        };

        /// A node in which a leaf holds only a single coefficient value
        template<typename PRIMITIVE>
        struct Node :
            kd::Node<
            typename PRIMITIVE::model_type,
            EmptyAttribute,
            EmptyAttribute,
            DataCoefficientAttribute>
        {
          typedef PRIMITIVE primitive_type;
          typedef typename primitive_type::model_type model_type;
        };
      }
    }
  }
}
