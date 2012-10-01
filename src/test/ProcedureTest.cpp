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

LOG_INIT;

using namespace std;

  using tomo::geometry::base::Point2f;
  using tomo::geometry::base::Vec2f;
  using tomo::geometry::aux::Ray2f;
  using tomo::geometry::base::Point3f;
  using tomo::slicing::Filling;
  using tomo::geometry::prim::Polygon;
  using tomo::geometry::prim::Ring;
  using tomo::geometry::prim::PointXYf;
  using tomo::geometry::prim::LineSegment;


  struct State
  {
    float speed_, extrudedLength_;
    float temperature_;
    TBD_PROPERTY_REF(Point3f,pos);
  };


int main(int ac, char* av[])
{
  cout << "Procedure Test 0.1 -- written by Wilston Oreo." << endl;
  cout << "A command line tool which generates geometric primities with different parameter sets." << endl;

//  srand((long long)av);
//
  string _outputFile;

  stringstream descStr;
  descStr << "Allowed options";

  // Declare the supported options.
  po::options_description desc(descStr.str());

  desc.add_options()
  ("help,h", "Display help message.")
  ("output,o", po::value<string>(&_outputFile), "Output image file")
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


  Filling<State> _filling;
  _filling.gap(10);
  _filling.angle(10.0);
  Filling<State>::ActionGroup _actionGroup;

  State _state;
  std::list<Polygon> _output;
  _actionGroup = _filling(_polygon,_output,_state);

  Magick::Image _image( Magick::Geometry(1024,1024), Magick::Color("black") );

  tomo::magick::Wrapper _wrapper(_image);
  
  Ray2f _ray(Point2f(50.0,150.0),Vec2f(800.0,100.0),0.0,1.0);
 
  vector<LineSegment> _polygonSegments = _polygon.fetchLineSegments();
  BOOST_FOREACH( LineSegment& _lineSegment, _polygonSegments )
  {
    _wrapper.draw(_lineSegment,Magick::Color("red"));
  }

  BOOST_FOREACH( LineSegment& _lineSegment, _filling.lineSegments_ )
  {
    _wrapper.draw(_lineSegment,Magick::Color("gray"));
  }

  _image.display();

  if (!_outputFile.empty())
    _image.write(_outputFile);

  return EXIT_SUCCESS;
}
