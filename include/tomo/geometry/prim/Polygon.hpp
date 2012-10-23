#pragma once

#include <vector>
#include <set>
#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/remove_reference.hpp>

#include <boost/geometry/geometries/polygon.hpp>
#include "Ring.hpp"
#include "tomo/geometry/base/Bounds.hpp"

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

        void segments(ray_type& _ray, std::vector<Segment>& _segments ) const;
        void fetchSegments(std::vector<Segment>& _segments) const;
        void boundingRays(float _angle, ray_type& _rayBegin, ray_type& _rayEnd) const;
        void add(const Ring& _ring);

        operator const BoostPolygon& () const
        {
          return polygon_;
        }
        operator BoostPolygon& ()
        {
          return polygon_;
        }

        template<class ARCHIVE>
        void serialize( ARCHIVE& _ar, const unsigned int _fileVersion )
        {
          _ar & polygon_;
          _ar & bounds_;
        }

        TBD_PROPERTY(BoostPolygon,polygon);
        TBD_PROPERTY_REF(bounds_type,bounds);

      private:
        void segmentsFromSegMarkers(
          const ray_type& _ray,
          const std::set<float>& _segMarkers,
          std::vector<Segment>& _segments) const;
      };
    }
  }
}

namespace boost
{
  namespace serialization
  {
    using tomo::geometry::prim::BoostPolygon;

    template<class ARCHIVE>
    void serialize(ARCHIVE& _ar, BoostPolygon& _boostPolygon, const unsigned int _fileVersion)
    {
      _ar & _boostPolygon.outer();
      _ar & _boostPolygon.inners();
    }
  }
}
