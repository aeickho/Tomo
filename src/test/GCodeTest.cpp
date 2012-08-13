#include <iostream>
#include <boost/program_options.hpp>
#include <boost/foreach.hpp>
#include <tbd/log.h>

#include "tomo/GCode.hpp"

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

  string inputFile, outputFile;
  int nSlices = 100, resX = 100, resY = 100;

  // Declare the supported options.
  po::options_description desc(descStr.str());

  desc.add_options()
    ("help,h", "Display help message.")
    ("input,i", po::value<string>(&inputFile), "Input OFF file")
    ("output,o", po::value<string>(&outputFile), "Output PPM image file")
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

  LOG->level(2);

  using namespace tomo::gcode;

  LOG_MSG;

  G<1> G1;

  std::cout << G1;


  return EXIT_SUCCESS;
}

