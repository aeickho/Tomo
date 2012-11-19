#pragma once

#include "TomoTestCase.hpp"
<<<<<<< HEAD
#include "BoostTimerWrapper.hpp"
=======
#include "wrap_boost_geometry_begin.hpp"
>>>>>>> 0cb00c0333132ca7d565566e14629a17f9dd215c
#include <boost/geometry.hpp>
#include "wrap_boost_geometry_end.hpp"

struct TomoBenchmark : TomoTestCase
{
  static int accuracy() { return 5; }
};


#define TOMO_BENCHMARK_BEGIN( test_name, n_iterations ) \
  BOOST_FIXTURE_TEST_CASE( test_name##_benchmark, TomoBenchmark ) \
  {\
    do {\
      std::string _formatStr(std::string(#test_name)+";%w");\
      typedef tomo::test::generators::test_name generator_type;\
      generator_type _generator;\
      generator_type::ObjectList&& _objs = _generator.generate();\
      for ( const generator_type::Object& _obj : _objs )\
      {\
        tomo::test::Timer _timer(accuracy());\
        for (int _iteration = 0; _iteration < n_iterations; ++_iteration)\
        {

#define TOMO_BENCHMARK_END } _timer.stop(); _timer.output(_formatStr); }} while(0);}
