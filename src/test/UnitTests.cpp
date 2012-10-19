#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE Tomo

#include <boost/test/included/unit_test.hpp>
#include "MagickWrapper.hpp"

LOG_INIT;

#include "LineSegmentConnectingTest.hpp"
#include "PolygonSortingTest.hpp"
#include "ToolchainTest.hpp"
#include "SequencingTest.hpp"
#include "GCodeTest.hpp"
