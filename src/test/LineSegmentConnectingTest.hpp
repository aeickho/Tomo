#pragma once

#include "tomo/slicing/SegmentConnector.hpp"

void makeCircle(std::vector<tomo::geometry::prim::ConnectableSegment>& _lineSegments, 
    tomo::geometry::base::Point2f _center, 
    float _radius, 
    int _segments = 24, 
    bool _inverse = false)
{
  using tomo::geometry::prim::ConnectableSegment;
  using tomo::geometry::base::Point2f;
  
  float _angleNext = 0.0, _anglePrev = 0.0;
  for (int i = 0; i < _segments; i++)
  {
    _angleNext = float(i+1)/float(_segments)*M_PI*2.0;
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
    _lineSegments.push_back(ConnectableSegment(_point0,_point1));
    _anglePrev = _angleNext;
  }
}


BOOST_AUTO_TEST_SUITE( LineSegmentConnectingTestSuite )

BOOST_AUTO_TEST_CASE( RingTest )
{
  using tomo::draw::magick::Wrapper;
  using tomo::geometry::prim::ConnectableSegment;
  using tomo::geometry::prim::Ring;
  using tomo::slicing::SegmentConnector;
  using tomo::geometry::base::Point2f;

  typedef std::vector<ConnectableSegment> LineSegments;

  Magick::Image _image( Magick::Geometry(1024,1024), Magick::Color("black") );
  Wrapper _w(_image);

  LineSegments _lineSegments;
  makeCircle(_lineSegments,Point2f(256,256),200);
  makeCircle(_lineSegments,Point2f(768,256),150);
  makeCircle(_lineSegments,Point2f(512,600),400);

  _w.draw(_lineSegments,Magick::Color("green"));
  _image.write("data/LineSegmentConnectingTest_RingTest_001.png");
  _w.clear();

  std::vector<Ring> _rings;
  SegmentConnector _segmentConnector;
  _segmentConnector(_lineSegments,_rings);
  BOOST_CHECK(_rings.size() == 3);

  _w.draw<Ring>(_rings,Magick::Color("black"));
  _image.write("data/LineSegmentConnectingTest_RingTest_002.png");
}

BOOST_AUTO_TEST_SUITE_END()

