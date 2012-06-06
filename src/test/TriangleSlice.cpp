#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <boost/program_options.hpp>
#include <boost/foreach.hpp>

#include "tomo/Plane.hpp"
#include "tomo/Triangle.hpp"
#include "tbd/log.h"

using namespace boost;
namespace po = program_options;

using namespace std;
using namespace tomo;

LOG_INIT;


int main(int ac, char* av[])
{
  cout << "MeshTest -- written by Wilston Oreo." << endl;

  stringstream descStr;
  descStr << "Allowed options";

  // Declare the supported options.
  po::options_description desc(descStr.str());

  desc.add_options() ("help,h", "Display help message.");

  // Parse the command line arguments for all supported options
  po::variables_map vm;
  po::store(po::parse_command_line(ac, av, desc), vm);
  po::notify(vm);

  if (vm.count("help"))
  {
    cout << desc << endl;
    return 1;
  }

  LOG->level(2);

  Vertex v0,v1,v2;
  Triangle tri(&v0,&v1,&v2);

  Plane plane();
  Triangles triangles = splitTriangle(tri,plane);

  return EXIT_SUCCESS;
}

