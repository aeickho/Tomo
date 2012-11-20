#pragma once 

#include <boost/geometry/geometry.hpp>
#include "tomo/geometry/prim/Ring.hpp"
#include "tomo/slicing/filling/Filling.hpp"
#include "tomo/slicing/filling/LinePattern.hpp"

#include "tomo/slicing/RepRapState.hpp"
#include "../generators/LineFilling.hpp"

#include <boost/geometry/algorithms/correct.hpp>

TOMO_TEST_CASE( LineFilling, 1024 )
{
  using namespace tomo::slicing; 
  using tomo::geometry::base::Bounds2f;
  using tomo::geometry::base::Point2f;

  RepRapState _state;
  _w.drawBounds(true);

  typedef filling::LineFilling<RepRapState,filling::LinePattern> LineFilling;
  LineFilling _lineFilling;
  MultiPolygon _out;
  MultiLineString _fillingPattern;
  filling::LinePattern _pattern(30.0,0.01);
  _pattern(_obj.bounds(),_fillingPattern);
  _w.draw(_fillingPattern,Magick::Color("orange"));
  writeImage();

  _w.clear();
  _w.drawEndings(true);
  _lineFilling(_obj,_out);
  _w.draw(_lineFilling.fillingPattern_,Magick::Color("green"));
  writeImage();  
}
TOMO_TEST_CASE_END

