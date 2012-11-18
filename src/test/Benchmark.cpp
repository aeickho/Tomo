#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE Tomo

#include <boost/test/included/unit_test.hpp>
#include "TomoBenchmark.hpp"

LOG_INIT;

TOMO_BENCHMARK_BEGIN( Skirt, MultiPolygon )
{
}
TOMO_BENCHMARK_END

