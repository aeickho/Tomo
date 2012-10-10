#include "tomo/geometry/prim/Ring.hpp"

#include <boost/foreach.hpp>
#include <tbd/log.h>
#include <math.h>
#include <boost/assert.hpp>
#include <boost/foreach.hpp>

#include <boost/geometry.hpp>
#include <boost/geometry/algorithms/union.hpp>
#include <boost/geometry/algorithms/area.hpp>


using namespace std;

namespace tomo
{
  namespace geometry
  {
    namespace prim
    {
      using boost::geometry::get;
      using base::BoostPoint2;
      
      Ring::Ring(Location _location) : 
        location_(_location)
      {
      }
      
      Ring::Ring(const BoostRing& _boostRing) :
        ring_(_boostRing)
      {
      }


      void Ring::add(point_type _point)
      {
        ring_.push_back(_point.as());
      }

      void Ring::resize(scalar_type _distance, std::vector<Ring>& _rings )
      {
        /*
            Ring _ring(OUTER);
            _ring.lineSegments() = this->lineSegments();
            std::vector<LineSegment>& _segments = _ring.lineSegments();

            BOOST_FOREACH( LineSegment& _segment, _segments )
            {
              vec_type _normal(_distance * (_segment.normal()).normalized());
              _segment.front() += _normal;
              _segment.back() += _normal;
            }
            _ring.adapt();

            _rings.push_back(_ring);
        */
      }

      void Ring::fetchLineSegments(std::vector<LineSegment>& _lineSegments) const
      {
        if (ring_.size() < 2) return;

        BoostRing::const_iterator it = ring_.begin();
        BoostPoint2 p0 = ring_.back(), p1 = ring_.front();
        _lineSegments.reserve(_lineSegments.size() + ring_.size());
        _lineSegments.push_back(LineSegment(p0,p1));

        while (1)
        {
          p0 = (*it);
          ++it;
          if (it == ring_.end()) break;
          p1 = (*it);
          _lineSegments.push_back(LineSegment(p0,p1));
        }
      }

      void Ring::fetchVertices(std::vector<Vertex2f>& _vertices) const
      {
        if (ring_.size() < 2) return;

        BoostRing::const_iterator it = ring_.begin();

        for (; it != ring_.end(); ++it)
        {
          _vertices.push_back(Vertex2f(point_type(*it),getNormal(it)));
        }
      }

      Ring::vec_type Ring::getNormal(BoostRing::const_iterator it) const
      {
        BoostRing::const_iterator _prev, _next;
        nextPrev(it,_prev,_next);
        return 0.5*(getNormal(_prev,it) + getNormal(it,_next));

      }

      Ring::vec_type Ring::getNormal(BoostRing::const_iterator _p0, BoostRing::const_iterator _p1) const
      {
        vec_type _d = point_type(*_p0) - point_type(*_p1);
        return vec_type(-_d.y(),_d.x());
      }

      Ring::Orientation Ring::orientation() const
      {
        BoostRing::const_iterator it = ring_.begin();
        scalar_type _determinant = 0.0;
        for (; it != ring_.end(); ++it)
          _determinant += determinant(it);

        return (_determinant < 0) ? CW : CCW;
      }

      void Ring::nextPrev(BoostRing::const_iterator it,
                          BoostRing::const_iterator& _prev,
                          BoostRing::const_iterator& _next) const
      {
        BoostRing::const_iterator _last = --ring_.end();
        _prev = (it == ring_.begin()) ? _last : it-1,
        _next = (it == _last) ? ring_.begin() : it+1;
      }

      Ring::scalar_type Ring::determinant(BoostRing::const_iterator it) const
      {
        BoostRing::const_iterator _prev, _next;
        nextPrev(it,_prev,_next);
        return (it->x()*_next->y() + _prev->x()*it->y() + _prev->y()*_next->x()) -
               (_prev->y()*it->x() + it->y()*_next->x() + _prev->x()*_next->y());
      }

      Ring::IntersectResult intersect(const Ring& _a, const Ring& _b, vector<Ring> _output)
      { 
        /// Make Cheese and union
        vector<BoostRing> _rings;
        boost::geometry::union_(_a(),_b(),_rings);
        
        if (_rings.size() != 1)
        {
          BOOST_FOREACH( BoostRing& _ring, _rings )
            _output.push_back(Ring(_ring));
          return Ring::DISJOINT;
        }
        BoostRing& _unionRing = _output.front()();
/*
        if (_a().size() != _unionRing.size() && 
            _b().size() != _unionRing.size()) goto overlap;

        float _areaA = boost::geometry::area(_a()),
              _areaB = boost::geometry::area(_b()),
              _areaOutput = boost::geometry::area(_unionRing);
                
        if (_areaA == _areaOutput) 
        {
          if (_a.location() != _b.location()) goto overlap;
          _output.push_back(_a);
          _output.push_back(_b);
          return Ring::B_WITHIN_A;
        }
        if (_areaB == _areaOutput) 
        {
          if (_a.location() != _b.location()) goto overlap;
          _output.push_back(_a);
          _output.push_back(_b);
          return Ring::A_WITHIN_B;
        }

     overlap:
        _output.push_back(_unionRing);*/
        return Ring::OVERLAP;

      }

    }
  }
}
