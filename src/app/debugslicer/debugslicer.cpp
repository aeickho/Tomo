#include <iostream>

#include <tbd/log.h>
#include "../../test/MagickWrapper.hpp"
#include <boost/program_options.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <list>

using namespace boost;
namespace po = program_options;

using namespace std;

int main(int ac, char* av[])
{
  cout << "Debug Slicer 0.1 -- written by Wilston Oreo." << endl;
  cout << "A command line tool which generates geometric primities with different parameter sets." << endl;

//  srand((long long)av);
//

  string _meshFile, _gcodeFile, _outputFile, _printerSettingsFile;
  vector<string> _inputFiles;

  float _printHeight = 20.0;
  float _sliceHeight = 0.2;
  float _firstSlice = 2.0;

  stringstream descStr;
  descStr << "Allowed options";

  // Declare the supported options.
  po::options_description desc(descStr.str());

  desc.add_options()
  ("help,h", "Display help message.")
  ("input,i", po::value< vector<string> >(&_inputFiles)->multitoken(), "Input parameter files")
  ("gcode,g", po::value<string>(&_gcodeFile), "GCode file to be generated")
  ("output,o", po::value<string>(&_outputFile), "Output image files")

  ("printer,p", po::value<string>(&_printerSettingsFile), "Printer settings file to load")
  
  ("printheight,H", po::value<float>(&_printHeight), "Print height in mm (default = 20.0)")
  ("sliceheight,h", po::value<float>(&_sliceHeight), "Slice height in mm (default = 0.2)")
  ("firstslice,f", po::value<float>(&_firstSlice), "Scale factor for first slice height (default = 2.0)")

  ("box", "Use boxes as objects")
  ("tube", "Use tubes as objects")
  ("sphere", "Use spheres as objects")
// ("mesh", po::value<string>(&_meshFile), "Use mesh file")
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
