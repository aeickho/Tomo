#pragma once

#include "../generators/VertexNearestVisitors.hpp"

#include "tomo/geometry/kd/build/Vertex.hpp"
#include "tomo/geometry/kd/visitor/Nearest.hpp"
#include "tomo/geometry/kd/visitor/KNearest.hpp"
#include "tomo/geometry/kd/visitor/InRadius.hpp"

#include "tomo/geometry/distance/VertexVertex.hpp"
#include "tomo/geometry/distance/VertexBounds.hpp"
#include "tomo/geometry/kd/build/HalfSplit.hpp"
#include "tomo/geometry/kd/build/SplitCost.hpp"



TOMO_TEST_CASE( VertexNearestVisitors, 1024 )
{
  using tomo::geometry::base::Vec2f;
  using tomo::geometry::prim::Vertex2f;
  using tomo::geometry::Model2f;
  using tomo::geometry::comp::Compound;

  typedef std::vector<Vertex2f> Vertices;
  
  typedef tomo::geometry::intersect::VertexNode<Model2f> VNIntersect;
  typedef tomo::geometry::distance::VertexVertex<Model2f> VVDist;
  typedef tomo::geometry::distance::VertexBounds<Model2f> VNodeDist;
  //typedef tomo::geometry::kd::build::HalfSplit<Vertices::kdtree_type,VNIntersect> BuildPolicy;
  typedef tomo::geometry::kd::build::VertexTree<Model2f> VertexKDTree;
  typedef tomo::geometry::kd::build::VertexTreeBuildPolicy<Model2f> BuildPolicy;

  VertexKDTree _kdTree;
  _kdTree.build<BuildPolicy>(_obj);

  const Vertex2f& _vertex = _obj[_obj.size()/2];

  int i = 0;
  _w.draw(_obj,"green");
  _w.draw(_kdTree,"white");
  writeImage();

  
/*
  typedef std::multimap<float,const Vertex2f*> MapType;
  MapType _verificationData;

  BOOST_FOREACH( const Vertex2f& _v, _vertices )
  {
    if (&_v == &_vertex) continue;
    _verificationData.insert(std::make_pair<>(VVDist()(_v,_vertex),&_v));
  }

  MapType::const_iterator it1, it2;


  // Test KNearest
  {
    typedef tomo::geometry::kd::visitor::KNearest<VertexKDTree,VVDist,VNodeDist> KNearest;
    KNearest _kNearest(_kdTree,_vertex,100);
    _kdTree.traversal<>(_kNearest);

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
    typedef tomo::geometry::kd::visitor::InRadius<VertexKDTree,VVDist,VNodeDist> InRadius;
    InRadius _inRadius(_kdTree,_vertex,0.1);
    _kdTree.traversal<>(_inRadius);
    
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
    typedef tomo::geometry::kd::visitor::Nearest<VertexKDTree,VVDist,VNodeDist> Nearest;
    Nearest _nearest(_kdTree);
    _nearest.primitive(const_cast<Vertex2f*>(&_vertex));
    _kdTree.traversal<>(_nearest);

    // Perform check 
    BOOST_CHECK(_nearest.nearest().first == _verificationData.begin()->first);
    BOOST_CHECK(_nearest.nearest().second == _verificationData.begin()->second);

    _w.draw(*_nearest.nearest().second,Magick::Color("yellow"));
    _w.image().write(TOMO_TEST_OUTPUT_NAME("_nearest.png",++i));
  }*/
}
TOMO_TEST_CASE_END


