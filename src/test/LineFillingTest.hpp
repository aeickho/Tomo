#pragma once 

#include "tomo/geometry/prim/Ring.hpp"
#include "tomo/slicing/filling/LineFilling.hpp"

#include <boost/geometry/algorithms/correct.hpp>

using tomo::geometry::prim::Ring;
using tomo::geometry::base::Point2f;


/// create demo circle
void makeCircle(Ring& _ring,
                Point2f _center,
                float _radius,
                int _numSegments = 24,
                bool _inverse = false)
{
  _ring().clear();
  for (int i = 0; i < _numSegments; i++)
  {
    float _angle = float(i)/float(_numSegments)*M_PI*2.0;
    float _sin = _radius*sin(_angle),
          _cos = _radius*cos(_angle);
    if (_inverse) _sin = -_sin;
    Point2f _p = Point2f(_cos,_sin) + _center;  
    _ring().push_back(_p.as());
  }
}



BOOST_AUTO_TEST_CASE( LineFillingTest )
{
  using namespace tomo::slicing; 

  MultiPolygon _multiPolygon;
  Ring _ring;

  Polygon _polygon1;
  {
    tomo::geometry::prim::BoostPolygon& _bP = _polygon1;
    makeCircle(_ring,Point2f(0.3,0.3),0.2,48);
    _bP.outer() = _ring();
    {
      makeCircle(_ring,Point2f(0.35,0.3),0.1,48,true);
      _bP.inners().push_back(_ring());
      makeCircle(_ring,Point2f(0.40,0.35),0.1,48,true);
      _bP.inners().push_back(_ring());
    }
  }
  _multiPolygon.polygons().push_back(_polygon1);

  int i = 0;
  Wrapper _w(1024,1024);
  _w.scale(tomo::geometry::base::Vec2f(1024,1024));
  _w.draw<>(_multiPolygon.polygons(),Magick::Color("red"));

  _w.image().write(TOMO_TEST_OUTPUT_NAME("_kdTree.png",++i));
}
