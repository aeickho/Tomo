#pragma once

#include <vector>
#include <set>
#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/remove_reference.hpp>

#include <boost/geometry/geometries/polygon.hpp>
#include "Ring.hpp"
#include "tomo/geometry/aux/Bounds.hpp"

namespace tomo
{
  namespace geometry
  {
    namespace prim
    {
      typedef boost::geometry::model::polygon<base::BoostPoint2> BoostPolygon;

      struct Polygon : Primitive2f
      {
        Polygon();
        Polygon(const BoostPolygon& _boostPolygon);
        Polygon(const Ring& _outer, const std::vector<Ring>& _inners);

        void lineSegments(ray_type& _ray, std::vector<LineSegment>& _lineSegments ) const;
        void fetchLineSegments(std::vector<LineSegment>& _lineSegments) const;
        void boundingRays(float _angle, ray_type& _rayBegin, ray_type& _rayEnd) const;  
        void add(const Ring& _ring);

        const BoostPolygon& operator()() const { return polygon_; }
        BoostPolygon& operator()() { return polygon_; }

        TBD_PROPERTY(BoostPolygon,polygon);
        TBD_PROPERTY_REF(bounds_type,bounds);

      private:
        void lineSegmentsFromSegMarkers(
            const ray_type& _ray,
            const std::set<float>& _segMarkers,
            std::vector<LineSegment>& _lineSegments) const;
      };
    }
  }
}

