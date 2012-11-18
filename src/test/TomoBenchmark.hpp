#pragma once

#include "TomoTestCase.hpp"

struct TomoBenchmark : TomoTestCase
{
  static int accuracy() { return 5; }
};


#define TOMO_BENCHMARK_BEGIN( test_name, object_name ) \
  BOOST_FIXTURE_TEST_CASE( test_name##_benchmark, TomoBenchmark ) \
  {\
    std::string _formatStr(std::string(#test_name)+";%w");\
    do {\
      std::list<std::string>&& _testFiles = getTestFiles();\
      for ( const std::string& _testFile : _testFiles )\
      {\
        object_name _obj;\
        std::ifstream ifs(_testFile);\
        boost::archive::text_iarchive ia(ifs);\
        ia >> _obj;
        /**@todo Bring boost timer to work... */
      //  boost::timer::cpu_timer t;

#define TOMO_BENCHMARK_END }} while(0);}
//        std::cout << boost::timer::format(t.elapsed(),accuracy(),_formatStr) << std::endl; }
