#include <tomo/geometry/prim/LineSegment.hpp>

namespace tomo
{
  namespace geometry
  {
    namespace prim
    {
      namespace intersect
      {
        struct RayVsBoundingBox : RayIntersector<BoundingBox>
        {
          /// @todo : That function is that ugly, make it template based and put it in Bounds class
          bool operator()(primitive_type& _boundingBox, ray_type& _ray)
          {
            return _boundingBox.bounds().intersect(_ray,tNear(),tFar());
          }

          TBD_PROPERTY_REF(scalar_type,tNear);
          TBD_PROPERTY_REF(scalar_type,tFar);
        };
      }
    }
  }
}
