#pragma once

#include "tomo/slicing/SliceStack.hpp"
#include "tomo/slicing/TriangleSlicer.hpp"

BOOST_AUTO_TEST_CASE( TriangleSlicerTest )
{
  using tomo::slicing::TriangleSlicer;
  using tomo::geometry::comp::Mesh;
  using tomo::geometry::base::Vec2f;
  using tomo::geometry::base::Matrix4f;
  using tomo::geometry::prim::Segment;
  using tomo::slicing::SegmentStack;
  using tomo::slicing::SliceStack;
  typedef geometry::comp::Compound<Segment> SegmentPlane;

  // Load a mesh
  Mesh _mesh("./yoda.stl");
  
  // Create a matrix (is identity matrix for now)
  Matrix4f _matrix;

  // Build slice stack
  SliceStack _sliceStack(/* Slice height in mm */ 5.0,_mesh.bounds());
  
  // Build stack with segments from existing slices in _sliceStack
  SegmentStack _segmentStack(_sliceStack);

  // Slice all triangles in mesh
  TriangleSlicer _triangleSlicer;
  _triangleSlicer(_mesh,_matrix,_segmentStack);

  int _res = 512;
  // Draw output segments...
  tomo::draw::magick::Wrapper _w(_res,_res);
  _w.drawEndings(false);
  _w.scale(Vec2f(_res / _mesh.bounds().size().x(), _res / _mesh.bounds().size().y()));
  _w.offset(Vec2f(-_mesh.bounds().min().x(),-_mesh.bounds().min().y())*_w.scale());

  int i = 0;
  std::vector<SegmentPlane*> _segmentPlanes = _segmentStack.fetch();

  BOOST_FOREACH( SegmentPlane* _segmentPlane, _segmentPlanes )
  {
    _w.clear();
    _w.draw<>(*_segmentPlane,Magick::Color("green"));
    _w.image().write(TOMO_TEST_OUTPUT_NAME(".png",i));
    i++;
  }
}
