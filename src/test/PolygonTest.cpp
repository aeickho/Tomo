#include <iostream>
#include <boost/assign.hpp>

#include <tbd/log.h>
#include <boost/program_options.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <list>
#include "tomo/Slice.hpp"

using namespace tomo;

  /*
// Generate Filling

// Generate Three Perimeters
/// A procedure generates a sequence of actions
template<class State, class Action> class Procedure
{
public:
//typedef Action<State> Action;
    /// Polygon = Isle
    virtual std::vector<Action> operator()( const Polygon& _in, std::list<Polygon>& _out, State& _state );
}

template<class State, class Action> class Filling : public Procedure<State,Action>
{
public:
  virtual std::vector<Action> operator()( const Polygon& _in, std::list<Polygon>& _out, State& _state );

  TBD_PROPERTY(float,lineWidth);
  TBD_PROPERTY(float,lineDistance);
  TBD_PROPERTY(bool,connectors);
  TBD_PROPERTY(float,angle);
  TBD_PROPERTY(FillingStrategy,strategy);
}
  Magick::Image image(Magick::Geometry(resX,resY), Magick::Color());

  float points[][2] = {{200, 130}, {410, 300}, {530, 260}, {290, 70}, {200, 130}};


  typedef boost::geometry::model::d2::point_xy<float> PolygonPoint ;

  boost::geometry::model::polygon< PolygonPoint >  _polygon;
  
    using boost::assign::tuple_list_of;
    using boost::make_tuple;
    using boost::geometry::append;

  boost::geometry::append(_polygon, points);

  CoordinateWrapper<PolygonPoint,Magick::Coordinate> _coordWrap;
  std::list<Magick::Coordinate> _coords;
  _coordWrap.coords_ = &_coords;
  
  boost::geometry::for_each_point( _polygon, _coordWrap );

  image.strokeColor(Magick::Color("red")); // Outline color 

  LOG_MSG << _coords.size();
  image.draw( Magick::DrawablePolyline( _coords )) ;

  image.write(outputFile);*/

using namespace boost;
namespace po = program_options;

using namespace std;

LOG_INIT;
/*
void generateFilling()
{

}

void paintFillingLine(LineString& _lineString)
{

}
*/
/*
template <typename Point, typename Coord>
struct CoordinateWrapper
{
  void operator()(const Point& p)
  { 
    using boost::geometry::get;
    coords_->push_back(Coord(int(get<0>(p)),int(get<1>(p))));
    LOG_MSG << coords_->size(); 
  }

  std::list<Coord>* coords_;
};*/





int main(int ac, char* av[])
{
  cout << "PolygonTest -- written by Wilston Oreo." << endl;
  cout << "Tests filling and perimeter with Boost::Geometry" << endl;

  stringstream descStr;
  descStr << "Allowed options";

  string inputFile, outputFile;
  int resX = 100, resY = 100;
  float fillingWidth = 10;

  // Declare the supported options.
  po::options_description desc(descStr.str());

  desc.add_options()
    ("help,h", "Display help message.")
    ("output,o", po::value<string>(&outputFile), "Output image file")
    ("resx,x", po::value<int>(&resX), "Resolution X")
    ("resy,y", po::value<int>(&resY), "Resolution Y")
    ("filling", po::value<float>(&fillingWidth), "Filling width")
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

  tomo::Polygon _polygon;





  return EXIT_SUCCESS;
}

