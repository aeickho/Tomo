#pragma once

#include <boost/geometry.hpp>
#include "tomo/slicing/Skirt.hpp"


BOOST_AUTO_TEST_CASE( SkirtTest )
{
  // create output image
  Wrapper _w(1024,1024);
  int i = 0;

  using tomo::geometry::prim::Ring; 
  using tomo::geometry::prim::Polygon; 
  using tomo::slicing::Skirt;
  using tomo::slicing::Slice;
  using tomo::slicing::SliceStack;
  using tomo::geometry::base::Bounds3f;
  using tomo::geometry::base::Point3f;
  using tomo::geometry::base::Point2f;
  using tomo::geometry::base::Vec2f;

  Ring _ring;
  makePolygon(_ring,Point2f(0.5,0.5),0.4);

  Polygon _polygon(_ring);
  
  makeCircle(_ring,Point2f(0.3,0.7),0.1);
  _polygon.holes().push_back(_ring);
  makeCircle(_ring,Point2f(0.6,0.35),0.07);
  _polygon.holes().push_back(_ring);

  SliceStack _sliceStack(0.5,Bounds3f(Point3f(0,0,0),Point3f(1,1,1)));
  std::vector<Slice*> _slices = _sliceStack.fetch(); 
  _w.scale(Vec2f(1024,1024));

  _w.draw(_polygon,Magick::Color("green"));

  for ( Slice* _slice : _slices )
  {
    _slice->add(_polygon);
  }

  Ring&& _skirt = Skirt()(0.05,_sliceStack);
  _w.draw(_skirt,Magick::Color("red"));
  _w.image().write(TOMO_TEST_OUTPUT_NAME(".png",++i));
}
