#pragma once

#include "tomo/geometry/aux/Ray.hpp"
#include "tomo/geometry/aux/Bounds.hpp"

namespace tomo
{
  namespace geometry
  {
    namespace prim
    {
      /// A primitive is an object which has an extent and for which an intersection point can be found
      template<class MODEL>
      struct Primitive : MODEL
      {
        TOMO_MODEL_TYPES(MODEL);
        typedef base::Point<MODEL> point_type;
        typedef base::Vec<MODEL> vec_type;
        typedef aux::Bounds<MODEL> bounds_type;
        /** @brief Return pointer to object
         */
        void* pointer() const
        {
          return (void*)const_cast<Primitive*>(this);
        }
      };

      typedef Primitive<Model2f> Primitive2f;
      typedef Primitive<Model3f> Primitive3f;
    }
  }
}

#define TOMO_PRIMITIVE_TYPES(PRIMITIVE) \
        TOMO_MODEL_TYPES(PRIMITIVE) \
        using typename PRIMITIVE::point_type; \
        using typename PRIMITIVE::vec_type; \
        using typename PRIMITIVE::bounds_type;


