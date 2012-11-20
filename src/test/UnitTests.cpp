#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE Tomo

// wrap boost test includes for clang compatibility (see https://svn.boost.org/trac/boost/ticket/5986)
#include "wrap_boost_test_begin.hpp"
#include <boost/test/included/unit_test.hpp>
#include "wrap_boost_test_end.hpp"

#include <tbd/log.h>
#include "TomoTestCase.hpp"

LOG_INIT;

using namespace tomo;

#include "tests/Matrix.hpp"
//#include "tests/TriangleSlicer.hpp"
//#include "tests/VertexNearestVisitors.hpp"
#include "tests/PolygonOffset.hpp"
#include "tests/LineFilling.hpp"
#include "tests/Skirt.hpp"

