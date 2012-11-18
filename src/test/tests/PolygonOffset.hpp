#pragma once

#include <boost/geometry.hpp>
#include "tomo/geometry/algorithm/PolygonOffset.hpp"
#include "../generators/PolygonOffset.hpp"

TOMO_TEST_CASE( PolygonOffset, 1024 )
{
  using namespace tomo::geometry;
  using prim::MultiPolygon;
  using tomo::geometry::algorithm::PolygonOffset;

  for (float _offset = 0.02; _offset < 0.10; _offset += 0.02)
  {
    _w.clear();
    MultiPolygon _outputPolygons;
    PolygonOffset<>()(_obj,_offset,_outputPolygons);
    _w.draw(_obj,Magick::Color("green"));
    _w.draw<>(_outputPolygons,Magick::Color("white"));
    writeImage();
  }

  for (float _offset = 0.02; _offset < 0.10; _offset += 0.02)
  {
    _w.clear();
    MultiPolygon _outputPolygons;
    PolygonOffset<>()(_obj,-_offset,_outputPolygons);
    _w.draw(_obj,Magick::Color("green"));
    _w.draw<>(_outputPolygons,Magick::Color("white"));
    writeImage();
  }
}
TOMO_TEST_CASE_END

