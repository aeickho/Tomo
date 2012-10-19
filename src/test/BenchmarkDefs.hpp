#include <boost/timer/timer.hpp>
#include <ostream>
#include "tomo/misc.hpp"

std::ostream* __outputStream;

#define NUM_ITERATIONS 10000000
#define ACCURACY 5

#define BENCHMARK_BEGIN(name) \
        void (name)() {\
          std::string _formatStr(std::string(#name)+";%w");
          boost::timer::cpu_timer t; \
      
#define DO_ITERATIONS \
          for (int _iteration = 0; _iteration < NUM_ITERATIONS; _iteration++) \


#define BENCHMARK_END() \
          (*__outputStream) << boost::timer::format(t.elapsed(),ACCURACY,_formatStr) << std::endl; \
        }
