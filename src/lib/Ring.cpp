#include "tomo/geometry/prim/Ring.hpp"

#include <boost/foreach.hpp>
#include <tbd/log.h>
#include <math.h>
#include <boost/assert.hpp>
#include <boost/foreach.hpp>

#include <boost/geometry/algorithms/for_each.hpp>
#include <boost/geometry/algorithms/union.hpp>


using namespace std;

namespace tomo
{
  namespace geometry
  {
    namespace prim
    {
      using boost::geometry::get;
      
      Ring::Ring(Location _location) : 
        location_(_location)
      {
      }

      void Ring::add(point_type _point)
      {
        push_back(_point);
      }

      void Ring::fetchSegments(std::vector<Segment>& _segments) const
      {
        if (size() < 2) return;

        using boost::geometry::get;
        
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

      void Ring::fetchVertices(std::vector<Vertex2f>& _vertices) const
      {
        if (size() < 2) return;

        Ring::const_iterator it = begin();
        
        for (; it != end(); ++it)
        {
          _vertices.push_back(Vertex2f(point_type(*it),getNormal(it)));
        }
      }

      Ring::vec_type Ring::getNormal(Ring::const_iterator it) const
      {
        Ring::const_iterator _prev, _next;
        prevNext(it,_prev,_next);
        return 0.5*(getNormal(_prev,it) + getNormal(it,_next));
      }

      Ring::vec_type Ring::getNormal(Ring::const_iterator _p0, Ring::const_iterator _p1) const
      {
        vec_type _d = point_type(*_p0) - point_type(*_p1);
        return vec_type(-_d.y(),_d.x());
      }

      Ring::Orientation Ring::orientation() const
      {
        Ring::const_iterator it = begin();
        scalar_type _determinant = 0.0;
        for (; it != end(); ++it)
          _determinant += determinant(it);

        return (_determinant < 0) ? CW : CCW;
      }

      void Ring::prevNext(Ring::const_iterator it,
                          Ring::const_iterator& _prev,
                          Ring::const_iterator& _next) const
      {
        Ring::const_iterator _last = --end();
        _prev = (it == begin()) ? _last : it-1,
        _next = (it == _last) ? begin() : it+1;
      }

      Ring::scalar_type Ring::determinant(Ring::const_iterator it) const
      {
        Ring::const_iterator _prev, _next;
        prevNext(it,_prev,_next);
        return (it->x()*_next->y() + _prev->x()*it->y() + _prev->y()*_next->x()) -
               (_prev->y()*it->x() + it->y()*_next->x() + _prev->x()*_next->y());
      }

      Ring::IntersectResult intersect(const Ring& _a, const Ring& _b, vector<Ring> _output)
      { 
        /// Make Cheese and union
        vector<Ring> _rings;
        //boost::geometry::union_(_a,_b,_rings);
        
        if (_rings.size() != 1)
        {
          BOOST_FOREACH( Ring& _ring, _rings )
            _output.push_back(Ring(_ring));
          return Ring::DISJOINT;
        }
/*
        Ring& _unionRing = _output.front()();
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
