#include <iostream>
#include <tbd/log.h>
#include <boost/program_options.hpp>
#include <boost/geometry.hpp>
#include <boost/archive/text_oarchive.hpp> 

#include "generators/Segments.hpp"
#include "generators/Skirt.hpp"


using namespace boost;
namespace po = program_options;

using namespace std;

LOG_INIT;

int main(int ac, char* av[])
{
  cout << "Tomo Test Data Generator -- written by Wilston Oreo." << endl;

  bool _writeImages = false;
  std::string _outputDir("testdata/input");

  stringstream descStr;
  descStr << "Allowed options";

  // Declare the supported options.
  po::options_description desc(descStr.str());

  desc.add_options()
  ("images,i", "Produce output images of test data.")
  ("output,o", po::value<string>(&_outputDir), "Output dir (default: testdata/input)")
  ("help,h", "Display this help message.");

  // Parse the command line arguments for all supported options
  po::variables_map vm;
  po::store(po::parse_command_line(ac, av, desc), vm);
  po::notify(vm);

  _writeImages = vm.count("images");
  if (vm.count("help"))
  {
    cout << desc << endl;
    return 1;
  }
 
  using namespace tomo::test::generators;
  RingGenerator(_writeImages,_outputDir);
  PolygonGenerator(_writeImages,_outputDir);
  MultiPolygonGenerator(_writeImages,_outputDir);
  

  return EXIT_SUCCESS;
}


