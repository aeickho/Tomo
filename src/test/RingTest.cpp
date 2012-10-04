#include <iostream>

#include <tbd/log.h>
#include <boost/program_options.hpp>

#include "tomo/geometry/prim/Ring.hpp"

using namespace boost;
namespace po = program_options;

using namespace std;

LOG_INIT;


int main(int ac, char* av[])
{
  cout << "PolygonTest -- written by Wilston Oreo." << endl;
  cout << "Tests filling and perimeter with Boost::Geometry" << endl;

  stringstream descStr;
  descStr << "Allowed options";


  // Declare the supported options.
  po::options_description desc(descStr.str());

  desc.add_options()
  ("help,h", "Display help message.")
/*  ("output,o", po::value<string>(&outputFile), "Output image file")
  ("resx,x", po::value<int>(&resX), "Resolution X")
  ("resy,y", po::value<int>(&resY), "Resolution Y")
  ("fillingwidth,w", po::value<float>(&fillingWidth_), "Filling width")
  ("filling", "Test filling")
  ("perimeter", "Test filling")
  ("polygonsort", "Test filling")*/
  ;

  // Parse the command line arguments for all supported options
  po::variables_map vm;
  po::store(po::parse_command_line(ac, av, desc), vm);
  po::notify(vm);

  if (vm.count("help"))
  {
    cout << desc << endl;
    return 1;
  }
  return EXIT_SUCCESS;
}

