#include <iostream>

#include <tbd/log.h>
#include <boost/program_options.hpp>

#include "tomo/geometry/prim/Ring.hpp"

using namespace boost;
namespace po = program_options;

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
  ("output,o", po::value<string>(&outputFile), "Output image file")
  ("resx,x", po::value<int>(&resX), "Resolution X")
  ("resy,y", po::value<int>(&resY), "Resolution Y")
  ("fillingwidth,w", po::value<float>(&fillingWidth_), "Filling width")
  ("filling", "Test filling")
  ("perimeter", "Test filling")
  ("polygonsort", "Test filling")
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

  Point2f _min(100.0,100.0);
  Point2f _max(700.0,700.0);

  Polygon _polygon;

  _polygon.addOuter(_min);
  _polygon.addOuter(Point2f(_max.x(),_min.y()));
  _polygon.addOuter(_max);
  _polygon.addOuter(Point2f(_min.x(),_max.y()));

  Ring _ring1;
  _ring1.push_back(PointXYf(300,300));
  _ring1.push_back(PointXYf(600,300));
  _ring1.push_back(PointXYf(600,600));
  _ring1.push_back(PointXYf(300,600));
  _polygon().inners().push_back(_ring1);

  Ring _ring2;
  _ring2.push_back(PointXYf(150,150));
  _ring2.push_back(PointXYf(250,150));
  _ring2.push_back(PointXYf(250,250));
  _ring2.push_back(PointXYf(200,300));
  _ring2.push_back(PointXYf(150,250));
  _polygon().inners().push_back(_ring2);


  unification(_ring1,_rings);

  difference(_ring2,_polygons);


  if (vm.count("filling")) fillingTest(_polygon);
  if (vm.count("perimeter")) perimeterTest(_polygon);
  if (vm.count("polygonsort")) polygonSortTest(_polygon);

  return EXIT_SUCCESS;
}

