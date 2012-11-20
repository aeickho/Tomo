#pragma once

#include <wrap_boost_geometry_begin.hpp>
#include <boost/geometry.hpp>
#include <wrap_boost_geometry_end.hpp>
#include "tomo/slicing/Perimeter.hpp"
#include "tomo/slicing/filling/Filling.hpp"
#include "../generators/Perimeter.hpp"

TOMO_TEST_CASE( Perimeter, 1024 )
{
  using tomo::slicing::Perimeter;
  using tomo::slicing::filling::LineFilling;
  using tomo::slicing::filling::LinePattern;
  using tomo::geometry::prim::MultiPolygon;
  using tomo::geometry::prim::MultiLineString;

  struct State {};
  Perimeter<State> _perimeter;

  _perimeter.nozzleWidth(0.007);
  _perimeter.number(3);

  MultiPolygon _output;

  MultiPolygon&& _perimeters = _perimeter(_obj,_output);

  _w.drawBounds(false);
  _w.draw(_obj,Magick::Color("green"));
  _w.draw(_perimeters,Magick::Color("red"));
  writeImage();
  _w.draw(_output,Magick::Color("maroon"));

  typedef LineFilling<State,LinePattern> Filling;
  LinePattern _pattern(45.0,0.01);
  Filling _lineFilling(_pattern);

  _w.drawEndings(false);
  MultiLineString&& _fillingOutput = _lineFilling(_output,_output);
  _w.draw(_fillingOutput,Magick::Color("yellow"));
  writeImage();  
}
TOMO_TEST_CASE_END

