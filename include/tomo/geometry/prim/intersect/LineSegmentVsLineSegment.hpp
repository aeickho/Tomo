#include "Intersector.hpp"
#include <tomo/geometry/prim/LineSegment.hpp>

namespace tomo
{
  namespace geometry
  {
    namespace prim
    {
      namespace intersect
      {
        struct LineSegmentVsLineSegment : Intersector<LineSegment,LineSegment>
        {
          bool operator()(const model_a_type& _a, const model_b_type& _b)
          {
            /// @todo add line segment intersection code here
          }

          TBD_PROPERTY_REF(point_type,intersectionPoint);
        };
      }
    }
  }
}
