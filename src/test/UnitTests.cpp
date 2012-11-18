#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE Tomo

#include <boost/test/included/unit_test.hpp>
#include <tbd/log.h>
#include "TomoTestCase.hpp"

LOG_INIT;

using namespace tomo;

//#include "tests/Matrix.hpp"
#include "tests/TriangleSlicer.hpp"
//#include "tests/NearestVisitors.hpp"

#include "tests/PolygonOffset.hpp"
#include "tests/Skirt.hpp"
#include "tests/LineFilling.hpp"

