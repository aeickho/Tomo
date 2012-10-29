#pragma once

#include "tomo/geometry/base/Point.hpp"

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

