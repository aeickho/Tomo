#pragma once

#include "TomoTestCase.hpp"
#include <boost/geometry.hpp>

struct TomoBenchmark : TomoTestCase
{
  static int accuracy() { return 5; }
};


#define TOMO_BENCHMARK_BEGIN( test_name ) \
  BOOST_FIXTURE_TEST_CASE( test_name##_benchmark, TomoBenchmark ) \
  {\
    do {\
      std::string _formatStr(std::string(#test_name)+";%w");\
      typedef tomo::test::generators::test_name generator_type;\
      generator_type _generator;\
      generator_type::ObjectList&& _objs = _generator.generate();\
      int _nObj = 0;\
      for ( const generator_type::Object& _obj : _objs )\
      {\

    /**@todo Bring boost timer to work... */
      //  boost::timer::cpu_timer t;

#define TOMO_BENCHMARK_END }} while(0);}
//        std::cout << boost::timer::format(t.elapsed(),accuracy(),_formatStr) << std::endl; }
