#pragma once

#include <vector>
#include <set>
#include "Ring.hpp"
#include "tomo/geometry/aux/Bounds.hpp"

namespace tomo
{
  namespace geometry
  {
    namespace prim
    {
      struct Polygon : public aux::Compound<Ring,2,float>
      {
        TOMO_COMPOUND_PRIMITIVE_NAME(rings);

        bool intersect(ray_type& _ray, float& _tNear, float& _tFar, vec_type* _normal = NULL) const
        {
          return false;
        }

        ctnr_type& lineSegments() { return objs_; }
        const ctnr_type& lineSegments() const { return objs_; }

        void lineSegments(ray_type& _ray, std::vector<LineSegment>& _lineSegments ) const;
        std::vector<LineSegment> fetchLineSegments() const;

        void fetch(Ring::Location& _location, ptr_vector_type& _output);

        void boundingRays(float _angle, ray_type& _rayBegin, ray_type& _rayEnd) const;

      private:

        void lineSegmentsFromSegMarkers(
            const ray_type& _ray,
            const std::set<float>& _segMarkers,
            std::vector<LineSegment>& _lineSegments) const;
        
      };

      typedef std::vector<Polygon> MultiPolygon;
    }
  }
}

