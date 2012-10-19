#pragma once
#include "Intersector.hpp"
#include "tomo/geometry/aux/KDNodeGeometry.hpp"

namespace tomo
{
  namespace geometry
  {
    namespace prim
    {
      namespace intersect
      {
        template <class PRIMITIVE>
        struct KDNodeIntersector :
            Intersector<PRIMITIVE, aux::KDNodeGeometry<PRIMITIVE> >
        {
          typedef typename aux::KDNodeGeometry<PRIMITIVE> node_type;
          typedef PRIMITIVE primitive_type;
          typedef aux::KDNodeIntersectResult result_type;
        };
      }
    }
  }
}

