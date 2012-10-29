#pragma once

#include "tomo/geometry/kd/visitor/VertexFunctors.hpp"
#include "tomo/geometry/kd/visitor/Nearest.hpp"
#include "tomo/geometry/kd/visitor/KNearest.hpp"
#include "tomo/geometry/kd/visitor/InRadius.hpp"
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

BOOST_AUTO_TEST_CASE( NearestVisitorsTest )
{
  using tomo::geometry::base::Vec2f;
  using tomo::geometry::base::Point2f;
  using tomo::geometry::prim::Vertex2f;

  Wrapper _w(1024,1024);
  _w.scale(Vec2f(1024,1024));

  typedef Compound<Vertex2f> Vertices;
  Vertices _vertices;

  int n = 1000;
  for (int i = 0; i < n; i++)
  {
    _vertices.add(Vertex2f(Point2f(RND,RND)));
  }
  
  typedef tomo::geometry::kd::visitor::VertexNodeIntersector<Vertex2f> VNIntersector;
  typedef tomo::geometry::kd::visitor::VertexVertexSqrDistance<Vertex2f> VVDist;
  typedef tomo::geometry::kd::visitor::VertexBoundsSqrDistance<Vertex2f> VNodeDist;
  _vertices.validate<VNIntersector>();

  const Vertex2f& _vertex = _vertices.objs()[n/2];
  int i = 0;
  _w.draw<>(_vertices,"green");
  _w.draw(_vertices.kdTree(),"white");
  _w.image().write(TOMO_TEST_OUTPUT_NAME("_test.png",++i));

  typedef std::multimap<float,const Vertex2f*> MapType;
  MapType _groundTruth;

  BOOST_FOREACH( const Vertex2f& _v, _vertices.objs() )
  {
    if (&_v == &_vertex) continue;
    _groundTruth.insert(std::make_pair<>(VVDist()(_v,_vertex),&_v));
  }
  MapType::const_iterator it1, it2;


  // Test KNearest
  {
    typedef tomo::geometry::kd::visitor::KNearest<Vertices::kdtree_type,VVDist,VNodeDist> KNearest;
    KNearest _kNearest(_vertices.kdTree(),_vertex,100);
    _vertices.kdTree().traversal<>(_kNearest);

    // Perform check 
    it1 = _kNearest.nearest().begin();
    it2 = _groundTruth.begin();
    BOOST_CHECK(_kNearest.nearest().size() <= _groundTruth.size());
    for (; it1 != _kNearest.nearest().end(); ++it1)
    {
      BOOST_CHECK(it1->first == it2->first);
      BOOST_CHECK(it1->second == it2->second);
      ++it2;
    }

    // Draw nearest
    BOOST_FOREACH( const KNearest::pair_type& _distPrim, _kNearest.nearest() )
      _w.draw(*_distPrim.second,Magick::Color("red"));
    
    // Write image output
    _w.image().write(TOMO_TEST_OUTPUT_NAME("_kNearest.png",++i));
  }

  // Test InRadius
  {
    typedef tomo::geometry::kd::visitor::InRadius<Vertices::kdtree_type,VVDist,VNodeDist> InRadius;
    InRadius _inRadius(_vertices.kdTree(),_vertex,0.1);
    _vertices.kdTree().traversal<>(_inRadius);
    
    // Perform check 
    it1 = _inRadius.nearest().begin();
    it2 = _groundTruth.begin();
    BOOST_CHECK(_inRadius.nearest().size() <= _groundTruth.size());
    for (; it1 != _inRadius.nearest().end(); ++it1)
    {
      BOOST_CHECK(it1->first == it2->first);
      BOOST_CHECK(it1->second == it2->second);
      ++it2;
    }

    BOOST_FOREACH( const InRadius::pair_type& _distPrim, _inRadius.nearest() )
      _w.draw(*_distPrim.second,Magick::Color("orange"));
    
    _w.image().write(TOMO_TEST_OUTPUT_NAME("_inRadius.png",++i));
  }

  // Test Nearest
  {
    typedef tomo::geometry::kd::visitor::Nearest<Vertices::kdtree_type,VVDist,VNodeDist> Nearest;
    Nearest _nearest(_vertices.kdTree(),_vertex);
    _vertices.kdTree().traversal<>(_nearest);

    // Perform check 
    BOOST_CHECK(_nearest.nearest().first == _groundTruth.begin()->first);
    BOOST_CHECK(_nearest.nearest().second == _groundTruth.begin()->second);

    _w.draw(*_nearest.nearest().second,Magick::Color("yellow"));
    _w.image().write(TOMO_TEST_OUTPUT_NAME("_nearest.png",++i));
  }
}

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
  _w.image().write(TOMO_TEST_OUTPUT_NAME(".png",++i));
  // clear output image for reuse
  _w.clear();
  // prepare connector to connect segments
  SegmentConnector<> _segmentConnector;
  // connect segments
  std::vector<Ring>&& _rings =_segmentConnector(_segments);
  // check that we got the three circles back
  BOOST_CHECK(_rings.size() == 3);
  // draw ouput image
  _w.draw<Ring>(_rings,Magick::Color("red"));
  // save output image
  _w.image().write(TOMO_TEST_OUTPUT_NAME(".png",++i));
}

BOOST_AUTO_TEST_SUITE_END()

