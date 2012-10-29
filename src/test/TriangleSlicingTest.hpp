#pragma once

#include "tomo/slicing/SliceStack.hpp"
#include "tomo/slicing/TriangleSlicer.hpp"


BOOST_AUTO_TEST_SUITE( TriangleSlicingTestSuite )

BOOST_AUTO_TEST_CASE( MatrixTest )
{
  using tomo::geometry::base::Point3f;
  using tomo::geometry::base::Matrix4f;

  // Matrix is identity matrix
  Matrix4f _m;
  Point3f _p0(1,2,3);

  // A point multiplied with identity matrix should remain the same...
  Point3f _p1 = _p0 * _m;
  BOOST_CHECK(_p0 == _p1);

  ///@todo Make more tests here: I.e. for rotation, translation and scaling
}

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

  int _res = 1024;
  // Draw output segments...
  tomo::draw::magick::Wrapper _w(_res,_res);
  _w.drawEndings(true);
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

BOOST_AUTO_TEST_SUITE_END()

