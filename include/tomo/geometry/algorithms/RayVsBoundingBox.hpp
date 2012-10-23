#include <tomo/geometry/prim/LineSegment.hpp>

namespace tomo
{
  namespace geometry
  {
    namespace algorithms
    {
      bool intersect(
          const BoundingBox& _boundingBox, 
          typename BoundingBox::ray_type& _ray, 
          typename BoundingBox::scalar_type* _tNear = nullptr,
          typename BoundingBox::scalar_type* _tFar = nullptr)
      {
          return _boundingBox.bounds().intersect(_ray,_tNear,_tFar);
      }
    }
  }
}
