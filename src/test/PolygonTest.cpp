#include <iostream>

#include <tbd/log.h>
#include <boost/program_options.hpp>

#include <list>
#include "MagickWrapper.hpp"

#include "tomo/slicing/Filling.hpp"
#include "tomo/slicing/Perimeter.hpp"

using namespace tomo;

string inputFile, outputFile;
int resX = 1024, resY = 1024;
float fillingWidth_ = 0.05;


using namespace boost;
namespace po = program_options;

using namespace std;

using tomo::geometry::prim::Polygon;
using Magick::Image;
using tomo::geometry::base::Point2f;
using tomo::geometry::base::Vec2f;
using tomo::geometry::aux::Ray2f;
using tomo::geometry::base::Point3f;
using tomo::geometry::prim::Polygon;
using tomo::geometry::prim::Ring;
using tomo::geometry::prim::LineSegment;
using tomo::slicing::Filling;
using tomo::slicing::Perimeter;

LOG_INIT;

struct State
{
  float speed_, extrudedLength_;
  float temperature_;
  TBD_PROPERTY_REF(Point3f,pos);
};

#define IMAGE_SETTINGS Magick::Geometry(resX,resY), Magick::Color("black")

void fillingTest(Polygon& _polygon)
{
  Magick::Image _image( IMAGE_SETTINGS );
  tomo::magick::Wrapper _wrapper(_image);
  _wrapper.drawEndings(true);

  State _state;
  std::list<Polygon> _output;

  Filling<State> _filling;
  _filling.gap(10);
  _filling.angle(30);
  _filling(_polygon,_output,_state);

  vector<LineSegment> _polygonSegments = _polygon.fetchLineSegments();

  BOOST_FOREACH( LineSegment& _lineSegment, _polygonSegments )
  {
    _wrapper.draw(_lineSegment,Magick::Color("red"));
  }
  _wrapper.drawEndings(false);
  BOOST_FOREACH( LineSegment& _lineSegment, _filling.lineSegments_ )
  {
    _wrapper.draw(_lineSegment,Magick::Color("gray"));
  }

  _image.display();
}

void perimeterTest(Polygon& _polygon)
{
}

void polygonSortTest(Polygon& _polygon)
{
}



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
  Ring _ring1(Ring::OUTER);
  _ring1.add(_min);
  _ring1.add(Point2f(_max.x(),_min.y()));
  _ring1.add(_max);
  _ring1.add(Point2f(_min.x(),_max.y()));

  Ring _ring2(Ring::INNER);
  _ring2.add(Point2f(300,300));
  _ring2.add(Point2f(600,300));
  _ring2.add(Point2f(600,600));
  _ring2.add(Point2f(300,600));

  Ring _ring3(Ring::INNER);
  _ring3.add(Point2f(150,150));
  _ring3.add(Point2f(250,150));
  _ring3.add(Point2f(250,250));
  _ring3.add(Point2f(200,300));
  _ring3.add(Point2f(150,250));

  _polygon.add(_ring1);
  _polygon.add(_ring2);
  _polygon.add(_ring3);

  if (vm.count("filling")) fillingTest(_polygon);
  if (vm.count("perimeter")) perimeterTest(_polygon);
  if (vm.count("polygonsort")) polygonSortTest(_polygon);

  return EXIT_SUCCESS;
}

