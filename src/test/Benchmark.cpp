#include "BenchmarkDefs.hpp"
#include <fstream>

using namespace std;

BENCHMARK_BEGIN( KDTreeBuild )
{
  DO_ITERATIONS
  {
  }
}
BENCHMARK_END()

BENCHMARK_BEGIN( LineSegmentConnecting )
{
  DO_ITERATIONS
  {
  }
}
BENCHMARK_END()



int main(int ac, char* av[])
{
  TOMO_ASSERT(ac <= 2);

  if (ac == 1) 
  {
    __outputStream = &cout; 
  } else
  if (ac == 2)
  {
    __outputStream = new ofstream(av[1]);
  }

  (*__outputStream) << "#Iterations;" << NUM_ITERATIONS << endl;
  
  boost::timer::cpu_timer t; 
  KDTreeBuild();
  LineSegmentConnecting();
  (*__outputStream) << boost::timer::format(t.elapsed(),ACCURACY,"Overall;%w") << endl; 

  return EXIT_SUCCESS;
}
