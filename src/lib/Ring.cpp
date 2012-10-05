#include "tomo/geometry/prim/Ring.hpp"

#include <boost/foreach.hpp>
#include <tbd/log.h>
#include <math.h>
#include <boost/assert.hpp>
#include <boost/foreach.hpp>

using namespace std;

namespace tomo
{
  namespace geometry
  {
    namespace prim
    {
        void Ring::add(point_type _point)
        {
          LineSegment _segment(_point,_point);
          lineSegments().push_back(_segment);
       //   size_t _n = lineSegments().size();
       //   if (_n <= 1) return;

          std::vector<LineSegment>& _segments = lineSegments();
          _segments.back().back() = _segments.front().front();


         /* LineSegment& _back = lineSegments()[_n-2];
          LineSegment& _front = lineSegments()[_n-1];
          _front.back(_point);
          _back.back();*/
        }

        void Ring::resize(scalar_type _distance, std::vector<Ring>& _rings )
        {
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
        }
        
        std::vector<Vertex2f> Ring::fetchVertices()
        {
          std::vector<Vertex2f> _vertices;
          _vertices.reserve(lineSegments().size());

          LineSegment& _lastSegment = lineSegments().back();
          BOOST_FOREACH( LineSegment& _segment, lineSegments() )
          {
            _vertices.push_back(Vertex2f(
                      _segment.front(),
                      (_segment.normal() + _lastSegment.normal()).normalized()));
            
            LOG_MSG << fmt("% %") % _segment.front().x() % _segment.front().y(); 
            _lastSegment = _segment;
          }
          LOG_MSG << _vertices.size();
          return _vertices;
        }
        
        void Ring::adapt()
        {
          std::vector<LineSegment>& _segments = lineSegments();
          LineSegment& _lastSegment = _segments.back();
          BOOST_FOREACH( LineSegment& _segment, _segments )
          {
            _segment.front() = _lastSegment.back();
            _lastSegment = _segment;
          }

          _lastSegment.back() = _segments.front().front();
        }
    }
  }
}
