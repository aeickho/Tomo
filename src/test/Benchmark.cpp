#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE Tomo

// wrap boost test includes for clang compatibility (see https://svn.boost.org/trac/boost/ticket/5986)
#include "wrap_boost_test_begin.hpp"
#include <boost/test/included/unit_test.hpp>
#include "wrap_boost_test_end.hpp"

#include "TomoBenchmark.hpp"
#include "generators/Skirt.hpp"

LOG_INIT;

TOMO_BENCHMARK_BEGIN( Skirt )
{
}
TOMO_BENCHMARK_END

