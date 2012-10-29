#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE Tomo

#include <boost/test/included/unit_test.hpp>
#include <tbd/log.h>
#include "tomo/draw/magick/Wrapper.hpp"
#include "tomo/misc.hpp"

LOG_INIT;

using namespace tomo;
using draw::magick::Wrapper;

static const std::string testOutputName(std::string _testName, 
                                  std::string _ext,
                                  int n = -1)
{
  std::stringstream ss;
  ss << std::string("./testdata/") << _testName;
  if (n >= 0) 
  {
    ss << std::setw(3) << std::setfill('0') << n;
  }
  ss << _ext;
  return ss.str();
}

#define TOMO_TEST_OUTPUT_NAME(EXT,N) \
  testOutputName(boost::unit_test::framework::current_test_case().p_name,EXT,N)

#define TOMO_TEST_OUTPUT_NAME_(EXT) \
  TOMO_TEST_OUTPUT_NAME(EXT,-1)

#include "MatrixTest.hpp"
#include "TriangleSlicerTest.hpp"
#include "NearestVisitorsTest.hpp"
#include "RingTest.hpp"

