#pragma once

#include <boost/geometry.hpp>
#include "tomo/slicing/Skirt.hpp"
#include "../generators/Skirt.hpp"

TOMO_TEST_CASE( Skirt, 1024 )
{
  using namespace tomo::slicing;
  using tomo::geometry::base::Bounds3f;
  using tomo::geometry::base::Point3f;

  SliceStack _sliceStack(0.5,Bounds3f(Point3f(0,0,0),Point3f(1,1,1)));
  std::vector<Slice*> _slices = _sliceStack.fetch();

  for ( Slice* _slice : _slices )
  {
    _slice->polygons(_obj);
  }

  _w.draw(_obj,Magick::Color("green"));

  Ring&& _skirt = Skirt(0.05)(_sliceStack);
  _w.draw(_skirt,Magick::Color("red"));

  writeImage();
}
TOMO_TEST_CASE_END
