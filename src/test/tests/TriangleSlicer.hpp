#pragma once

#include "tomo/slicing/SliceStack.hpp"
#include "tomo/slicing/TriangleSlicer.hpp"

#include "../generators/TriangleSlicer.hpp"

TOMO_TEST_CASE( TriangleSlicer, 512 )
{
  using tomo::geometry::prim::Segment;
  using tomo::geometry::base::Matrix4f;
  using tomo::slicing::SegmentStack;
  using tomo::slicing::SliceStack;
  typedef geometry::comp::Compound<Segment> SegmentPlane;

  // Create a matrix (is identity matrix for now)
  Matrix4f _matrix;

  // Build slice stack
  SliceStack _sliceStack(/* Slice height in mm */ 5.0,_obj.bounds());
  
  // Build stack with segments from existing slices in _sliceStack
  SegmentStack _segmentStack(_sliceStack);

  // Slice all triangles in mesh
  tomo::slicing::TriangleSlicer _triangleSlicer;
  _triangleSlicer(_obj,_matrix,_segmentStack);

  // Draw output segments...
  _w.drawEndings(false);
  _w.scale(Vec2f(_res / _obj.bounds().size().x(), _res / _obj.bounds().size().y()));
  _w.offset(Vec2f(-_obj.bounds().min().x(),-_obj.bounds().min().y())*_w.scale());
  std::vector<SegmentPlane*> _segmentPlanes = _segmentStack.fetch();

  BOOST_FOREACH( SegmentPlane* _segmentPlane, _segmentPlanes )
  {
    _w.clear();
    _w.draw(*_segmentPlane,Magick::Color("green"));
    writeImage();
  }
}
TOMO_TEST_CASE_END
