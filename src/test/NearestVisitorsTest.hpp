#pragma once

#include "tomo/geometry/kd/visitor/VertexFunctors.hpp"
#include "tomo/geometry/kd/visitor/Nearest.hpp"
#include "tomo/geometry/kd/visitor/KNearest.hpp"
#include "tomo/geometry/kd/visitor/InRadius.hpp"

BOOST_AUTO_TEST_CASE( NearestVisitorsTest )
{
  using tomo::geometry::base::Vec2f;
  using tomo::geometry::base::Point2f;
  using tomo::geometry::prim::Vertex2f;
  using tomo::geometry::comp::Compound;

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
  MapType _verificationData;

  BOOST_FOREACH( const Vertex2f& _v, _vertices.objs() )
  {
    if (&_v == &_vertex) continue;
    _verificationData.insert(std::make_pair<>(VVDist()(_v,_vertex),&_v));
  }
  MapType::const_iterator it1, it2;


  // Test KNearest
  {
    typedef tomo::geometry::kd::visitor::KNearest<Vertices::kdtree_type,VVDist,VNodeDist> KNearest;
    KNearest _kNearest(_vertices.kdTree(),_vertex,100);
    _vertices.kdTree().traversal<>(_kNearest);

    // Perform check 
    it1 = _kNearest.nearest().begin();
    it2 = _verificationData.begin();
    BOOST_CHECK(_kNearest.nearest().size() <= _verificationData.size());
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
    it2 = _verificationData.begin();
    BOOST_CHECK(_inRadius.nearest().size() <= _verificationData.size());
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
    Nearest _nearest(_vertices.kdTree());
    _nearest.primitive(const_cast<Vertex2f*>(&_vertex));
    _vertices.kdTree().traversal<>(_nearest);

    // Perform check 
    BOOST_CHECK(_nearest.nearest().first == _verificationData.begin()->first);
    BOOST_CHECK(_nearest.nearest().second == _verificationData.begin()->second);

    _w.draw(*_nearest.nearest().second,Magick::Color("yellow"));
    _w.image().write(TOMO_TEST_OUTPUT_NAME("_nearest.png",++i));
  }
}


