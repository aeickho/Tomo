#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE Tomo

#include <boost/test/included/unit_test.hpp>
#include "TomoBenchmark.hpp"
#include "generators/Skirt.hpp"

LOG_INIT;

TOMO_BENCHMARK_BEGIN( Skirt )
{
}
TOMO_BENCHMARK_END

