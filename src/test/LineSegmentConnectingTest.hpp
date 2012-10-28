#pragma once

#include "tomo/slicing/SegmentConnector.hpp"

using tomo::draw::magick::Wrapper;
using tomo::geometry::prim::Ring;
using tomo::slicing::SegmentConnector;
using tomo::geometry::base::Point2f;
using tomo::geometry::comp::Compound;
using tomo::geometry::prim::Segment;

/// create demo circle
void makeCircle(Compound<Segment>& _segments,
                Point2f _center,
                float _radius,
                int _numSegments = 24,
                bool _inverse = false)
{
  float _angleNext = 0.0, _anglePrev = 0.0;
  for (int i = 0; i < _numSegments; i++)
  {
    _angleNext = float(i+1)/float(_numSegments)*M_PI*2.0;
    float _sinPrev = _radius*sin(_anglePrev),
          _cosPrev = _radius*cos(_anglePrev);
    float _sinNext = _radius*sin(_angleNext),
          _cosNext = _radius*cos(_angleNext);
    if (_inverse)
    {
      _sinNext = -_sinNext;
      _sinPrev = -_sinPrev;
    }
    Point2f _point0 = Point2f(_cosPrev,_sinPrev) + _center;
    Point2f _point1 = Point2f(_cosNext,_sinNext) + _center;
    _segments.add(Segment(_point0,_point1));
    _anglePrev = _angleNext;
  }
}


BOOST_AUTO_TEST_SUITE( LineSegmentConnectingTestSuite )

BOOST_AUTO_TEST_CASE( RingTest )
{
  // create output image
  Wrapper _w(1024,1024);
  // create input compound (three circles)
  Compound<Segment> _segments;
  {
    makeCircle(_segments,Point2f(256,256),200);
    makeCircle(_segments,Point2f(768,256),150);
    makeCircle(_segments,Point2f(512,600),400);
  }
  int i = 0;
  // draw input compound into output image
  _w.draw(_segments,Magick::Color("green"));
  // save output image
  _w.image().write(TOMO_TEST_OUTPUT_NAME("png",++i));
  // clear output image for reuse
  _w.clear();
  // prepare connector to connect segments
  SegmentConnector<> _segmentConnector;
  // connect segments
  std::vector<Ring>&& _rings =_segmentConnector(_segments);
  // check that we got the three circles back
  BOOST_CHECK(_rings.size() == 3);
  // draw ouput image
  _w.draw<Ring>(_rings,Magick::Color("black"));
  // save output image
  _w.image().write(TOMO_TEST_OUTPUT_NAME("png",++i));
}

BOOST_AUTO_TEST_SUITE_END()

