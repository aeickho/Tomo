#pragma once
#include "Intersector.hpp"
#include <tomo/geometry/aux/KDNodeGeometry.hpp> 

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
            Intersector<PRIMITIVE, aux::KDNodeGeometry<typename PRIMITIVE::model_type> >
        {
        };
      }
    }
  }
}

