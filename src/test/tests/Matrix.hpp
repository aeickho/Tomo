#pragma once

#include "tomo/geometry/base/Point.hpp"
#include "../generators/Matrix.hpp"

TOMO_TEST_CASE( Matrix, 0 )
{
  using tomo::geometry::base::Point3f;
  using tomo::geometry::base::Matrix4f;

  // Matrix is identity matrix
  Point3f _p0(1,2,3);

  // A point multiplied with identity matrix should remain the same...
  Point3f _p1 = _p0 * _obj;
  BOOST_CHECK(_p0 == _p1);

  ///@todo Make more tests here: I.e. for rotation, translation and scaling
}
TOMO_TEST_CASE_END

