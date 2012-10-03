#include <tbd/log.h>
LOG_INIT;
#include <boost/program_options.hpp>
#include "MagickWrapper.hpp"
#include "tomo/slicing/LineFilling.hpp"
#include "tomo/slicing/Perimeter.hpp"
#include <iostream>
#include <list>


using namespace std;
using namespace boost::program_options;
using namespace tomo::geometry::base;
using namespace tomo::geometry::aux;
using namespace tomo::geometry::prim;
using namespace tomo::slicing;
using namespace Magick;

string inputFile, outputFile;
int resX = 1024, resY = 1024;
float fillingWidth_ = 0.05;

struct State
{
  float speed_, extrudedLength_;
  float temperature_;
  TBD_PROPERTY_REF(Point3f,pos);
};

#define IMAGE_SETTINGS Geometry(resX,resY), Color("black")

void fillingTest(Polygon& _polygon)
{
  Magick::Image _image( IMAGE_SETTINGS );
  tomo::magick::Wrapper _wrapper(_image);
  _wrapper.drawEndings(true);

  State _state;
  list<Polygon> _output;

  LineFilling<State> _filling;
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
  cout 
    << "PolygonTest -- written by Wilston Oreo." << endl
    << "Tests filling and perimeter with Boost::Geometry" << endl
    ;

  stringstream descStr;
  descStr << "Allowed options";

  // Declare the supported options.
  options_description desc(descStr.str());

  desc.add_options()
  ("help,h", "Display help message.")
  ("output,o", value<string>(&outputFile), "Output image file")
  ("resx,x", value<int>(&resX), "Resolution X")
  ("resy,y", value<int>(&resY), "Resolution Y")
  ("fillingwidth,w", value<float>(&fillingWidth_), "Filling width")
  ("filling", "Test filling")
  ("perimeter", "Test filling")
  ("polygonsort", "Test filling")
  ;

  // Parse the command line arguments for all supported options
  variables_map vm;
  store(parse_command_line(ac, av, desc), vm);
  notify(vm);

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

