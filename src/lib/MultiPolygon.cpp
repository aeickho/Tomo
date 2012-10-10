#include "tomo/geometry/prim/MultiPolygon.hpp"

#include <boost/foreach.hpp>

namespace tomo
{
  namespace geometry
  {
    namespace prim
    {
      MultiPolygon::MultiPolygon()
      {
      }

      MultiPolygon::MultiPolygon(const std::vector<Ring>& _rings)
      {
        from(_rings);
      }

      MultiPolygon::MultiPolygon(const BoostMultiPolygon& _boostMultiPolygon)
      {
        from(_boostMultiPolygon);
      }
      

      void MultiPolygon::fetchLineSegments(std::vector<LineSegment>& _lineSegments) const
      {
        BOOST_FOREACH( const Polygon& _polygon, polygons_ )
          _polygon.fetchLineSegments(_lineSegments);
      }

      void MultiPolygon::fetch(BoostMultiPolygon& _boostMultiPolygon) const
      {
        _boostMultiPolygon.reserve(_boostMultiPolygon.size() + polygons_.size());
        BOOST_FOREACH( const Polygon& _polygon, polygons_ )
          _boostMultiPolygon.push_back(_polygon());
      }

      void MultiPolygon::from(const std::vector<Ring>& _rings)
      {
        polygons_.clear();
        std::vector<Ring> _outerRings;
        //determineOuterRings(_rings,_outerRings);
        //determineHoles(_rings,_outerRings);
        //BOOST_FOREACH( Ring& _ring, _outerRings )
        {

        }
      }
      
      void MultiPolygon::from(const BoostMultiPolygon& _boostMultiPolygon)
      {
        polygons_.clear();
        polygons_.reserve(_boostMultiPolygon.size());
        BOOST_FOREACH( const BoostPolygon& _polygon, _boostMultiPolygon )
        {
          polygons_.push_back(Polygon(_polygon));
        }
      }

      void MultiPolygon::unify(const Rings& _input, Rings& _output) const
      {

      }

      


    }
  }
}
