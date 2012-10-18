#pragma once

#include <tomo/geometry/aux/Ray.hpp>

namespace tomo
{
  namespace geometry
  {
    namespace prim
    {
      namespace intersect
      {
        template <class PRIMITIVE>
        struct RayIntersector : Intersector< PRIMITIVE,typename aux::Ray<PRIMITIVE::model_type> >
        {
          typedef typename aux::Ray<PRIMITIVE::model_type> ray_type;
          typedef typename PRIMITIVE primitive_type;

          TBD_PROPERTY(scalar_type,tNear);
          TBD_PROPERTY(scalar_type,tFar);
        };
      }
    }
  }
}
