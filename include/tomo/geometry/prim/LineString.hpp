#pragma once

#include <boost/geometry/geometries/linestring.hpp>
#include <boost/geometry/geometries/register/linestring.hpp>

#include <boost/geometry/multi/geometries/multi_linestring.hpp>
#include <boost/geometry/multi/geometries/register/multi_linestring.hpp>

#include <boost/geometry/algorithms/for_each.hpp>


namespace tomo
{
  namespace geometry
  {
    namespace prim
    {
      struct LineString : std::vector<base::Point2f>, Primitive2f
      {
        void fetchSegments(std::vector<Segment>& _segments) const
        {
          struct Functor
          {
            Functor(std::vector<Segment>& _segments) :
              segments_(_segments) {}

            template<class T> void operator()( const T& _segment)
            {
              segments_.push_back(Segment(_segment.first,_segment.second));
            }
            std::vector<Segment>& segments_;
          } _functor(_segments);

          _segments.reserve(_segments.size() + size());
          boost::geometry::for_each_segment(*this,_functor);
        }
      };

      struct MultiLineString : std::vector<LineString>, Primitive2f
      {
      };
    }
  }
}

BOOST_GEOMETRY_REGISTER_LINESTRING(tomo::geometry::prim::LineString)
BOOST_GEOMETRY_REGISTER_MULTI_LINESTRING(tomo::geometry::prim::MultiLineString)

