#include <iostream>

#include <tbd/log.h>
#include "MagickWrapper.hpp"
#include <boost/program_options.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <list>

#include "tomo/slicing/Filling.hpp"

using namespace boost;
namespace po = program_options;

using namespace std;

int main(int ac, char* av[])
{
  cout << "Procedure Test 0.1 -- written by Wilston Oreo." << endl;
  cout << "A command line tool which generates geometric primities with different parameter sets." << endl;

//  srand((long long)av);
//

  stringstream descStr;
  descStr << "Allowed options";

  // Declare the supported options.
  po::options_description desc(descStr.str());

  desc.add_options()
  ("help,h", "Display help message.")
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

  using tomo::geometry::base::Point2f;
  using tomo::geometry::base::Point3f;
  using tomo::slicing::Filling;
  using tomo::geometry::prim::Polygon;
  using tomo::geometry::prim::PointXYf;

  struct State
  {
    float speed_, extrudedLength_;
    float temperature_;
    TBD_PROPERTY_REF(Point3f,pos);
  };

  Point2f _min(10.0,10.0);
  Point2f _max(20.0,20.0);

  Polygon _polygon;
  _polygon.outer().push_back(PointXYf(_min.x(),_min.y()));
  _polygon.outer().push_back(PointXYf(_max.x(),_min.y()));
  _polygon.outer().push_back(PointXYf(_max.x(),_max.y()));
  _polygon.outer().push_back(PointXYf(_min.x(),_max.y()));

  Filling<State> _filling;
  Filling<State>::ActionGroup _actionGroup;

  State _state;
  std::list<Polygon> _output;
  _actionGroup = _filling(_polygon,_output,_state);


  return EXIT_SUCCESS;
}
