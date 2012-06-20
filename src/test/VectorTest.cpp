#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <boost/program_options.hpp>
#include <boost/foreach.hpp>

#include "tomo/Vector.hpp"
#include "tbd/log.h"

using namespace boost;
namespace po = program_options;

using namespace std;
using namespace tomo;

LOG_INIT;


int main(int ac, char* av[])
{
  Vec<4,int> x;
  LOG_MSG << fmt("% % % %") % x[0] % x[1] % x[2] % x[3];

  LOG_MSG << "Test: Vec<3,float>";
  Vec<3,float> v(1.0,1.0,1.0);
  LOG_MSG << fmt("% % %") % v.x() % v.y() % v.z();
  v(42,42,42);
  LOG_MSG << fmt("% % %") % v.x() % v.y() % v.z();

  Vec<3,float> w(2.0,2.0,2.0);
  LOG_MSG << fmt("v . w = %") % (v.dot(w));

  v += w;
  LOG_MSG << fmt("% % %") % v.x() % v.y() % v.z();
  v -= w;
  LOG_MSG << fmt("% % %") % v.x() % v.y() % v.z();
  v = w * v;
  LOG_MSG << fmt("% % %") % v.x() % v.y() % v.z();

  w *= 3.0;

  v.normalize();
  v = w.normalized();


  LOG_MSG << "Test: Vec<2,int>";
  Vec<2,int> u(1,1);
  LOG_MSG << fmt("% %") % u.x() % u.y();
  v(42,42,42);
  LOG_MSG << fmt("% %") % u.x() % u.y();



  return EXIT_SUCCESS;
}

