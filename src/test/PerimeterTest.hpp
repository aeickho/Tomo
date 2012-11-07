#pragma once

#include <boost/geometry.hpp>
#include <boost/geometry/multi/geometries/register/multi_polygon.hpp>
#include "tomo/slicing/perimeter/detail/RingOffset.hpp"
#include "tomo/slicing/perimeter/detail/PolygonOffset.hpp"

#include <list>

using tomo::geometry::prim::Ring;
using tomo::geometry::base::Point2f;

BOOST_GEOMETRY_REGISTER_MULTI_POLYGON(std::vector<tomo::geometry::prim::Polygon>);

/// create test polygon
void makePolygon(Ring& _ring,
                 Point2f _center,
                 float _radius,
                 int _numSegments = 24,
                 bool _inverse = false)
{
  _ring.clear();
  for (int i = 0; i < _numSegments; i++)
  {
    float _angle = float(i)/float(_numSegments)*M_PI*2.0;
    float _sin = _radius*sin(_angle),
          _cos = _radius*cos(_angle);

    if (i < _numSegments/4 || (i > _numSegments/2 && i <= 3*_numSegments/4))
    {
      _sin *= 0.5;
      _cos *= 0.5;
    }

    if (i == 0)
    {
      _sin *= 0.1;
      _cos *= 0.1;
    }

    if (_inverse) _sin = -_sin;
    _ring.add(Point2f(_cos,_sin) + _center);
  }
}

/// create test polygon
void makeCircle(Ring& _ring,
                 Point2f _center,
                 float _radius,
                 int _numSegments = 24,
                 bool _inverse = false)
{
  _ring.clear();
  for (int i = 0; i < _numSegments; i++)
  {
    float _angle = float(i)/float(_numSegments)*M_PI*2.0;
    float _sin = _radius*sin(_angle),
          _cos = _radius*cos(_angle);

    if (_inverse) _sin = -_sin;
    _ring.push_back(Point2f(_cos,_sin) + _center);
  }
}



typedef std::vector<Ring> Rings;

void unify(const Rings& _input, Rings& _output)
{
  typedef std::list<Ring> Container;
  Container _inputRings;

  BOOST_FOREACH( const Ring& _ring, _input)
  {
    Ring _newRing(_ring);
    boost::geometry::correct(_newRing);
    _inputRings.push_back(_newRing);
  }

  typedef Container::iterator iterator;
  for (iterator i = _inputRings.begin(); i != _inputRings.end(); ++i)
  {
    iterator j = i;
    ++j;
    for (; j != _inputRings.end(); ++j)
    {
      std::vector<Ring> _newRings;
      boost::geometry::union_(*i,*j,_newRings);
      if (_newRings.size() == 1)
      {
        *i = _newRings.back();
        _inputRings.erase(j);
        j = i;
      }
    }
  }

  BOOST_FOREACH ( const Ring& _inputRing, _inputRings )
  {
    _output.push_back(Ring(_inputRing));
  }
}

BOOST_AUTO_TEST_CASE( RingOffsetTest )
{
  using namespace tomo::geometry;
  using prim::Segment;
  using base::Vec2f;
  using tomo::slicing::perimeter::detail::RingOffset;

  Ring _ring;
  makePolygon(_ring,Point2f(0.5,0.5),0.4);

  int i = 0;
  Wrapper _w(1024,1024);
  _w.scale(tomo::geometry::base::Vec2f(1024,1024));

  for (float _offset = 0.02; _offset < 0.20; _offset += 0.04)
  {
    _w.clear();

    std::vector<Ring> _outputRings1;
    RingOffset<>()(_ring,_offset,_outputRings1);
    _w.draw(_ring,Magick::Color("green"));
    _w.draw<>(_outputRings1,Magick::Color("white"));
    _w.image().write(TOMO_TEST_OUTPUT_NAME(".png",++i));
  }

  for (float _offset = 0.02; _offset < 0.20; _offset += 0.04)
  {
    _w.clear();
    std::vector<Ring> _outputRings2;
    RingOffset<>()(_ring,-_offset,_outputRings2);
    _w.draw(_ring,Magick::Color("green"));
    _w.draw<>(_outputRings2,Magick::Color("white"));
    _w.image().write(TOMO_TEST_OUTPUT_NAME(".png",++i));
  }
}

BOOST_AUTO_TEST_CASE( PolygonOffsetTest )
{
  using namespace tomo::geometry;
  using prim::Segment;
  using prim::Polygon;
  using base::Vec2f;
  using tomo::slicing::perimeter::detail::PolygonOffset;

  Ring _ring;
  makePolygon(_ring,Point2f(0.5,0.5),0.4);

  Polygon _polygon(_ring);
  
  makeCircle(_ring,Point2f(0.3,0.7),0.1);
  _polygon.holes().push_back(_ring);
  makeCircle(_ring,Point2f(0.6,0.35),0.07);
  _polygon.holes().push_back(_ring);

  int i = 0;
  Wrapper _w(1024,1024);
  _w.scale(tomo::geometry::base::Vec2f(1024,1024));

  for (float _offset = 0.02; _offset < 0.10; _offset += 0.02)
  {
    _w.clear();
    std::vector<Polygon> _outputPolygons;
    PolygonOffset<>()(_polygon,_offset,_outputPolygons);
    _w.draw(_polygon,Magick::Color("green"));
    _w.draw<>(_outputPolygons,Magick::Color("white"));
    _w.image().write(TOMO_TEST_OUTPUT_NAME(".png",++i));
  }

  for (float _offset = 0.02; _offset < 0.10; _offset += 0.02)
  {
    _w.clear();
    std::vector<Polygon> _outputPolygons;
    PolygonOffset<>()(_polygon,-_offset,_outputPolygons);
    _w.draw(_polygon,Magick::Color("green"));
    _w.draw<>(_outputPolygons,Magick::Color("white"));
    _w.image().write(TOMO_TEST_OUTPUT_NAME(".png",++i));
  }
}


