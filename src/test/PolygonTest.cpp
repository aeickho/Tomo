#include <iostream>
#include <boost/assign.hpp>

#include <tbd/log.h>


#include <boost/mpl/assert.hpp>
#include <boost/static_assert.hpp>

#include <boost/assign.hpp>
#include <boost/geometry/geometry.hpp> 


#include <boost/program_options.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <list>
#include "tomo/Slice.hpp"
#include <Magick++.h>

#include <boost/geometry/geometries/linestring.hpp>
#include <boost/geometry/multi/geometries/multi_linestring.hpp>
#include <boost/geometry/multi/geometries/register/multi_linestring.hpp>
#include <boost/geometry/multi/geometries/register/multi_polygon.hpp>

using namespace tomo;

string inputFile, outputFile;
int resX = 100, resY = 100;
float fillingWidth_ = 0.05;


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


  image.write(outputFile);*/

using namespace boost;
namespace po = program_options;

using namespace std;

LOG_INIT;


template <typename Point, typename Coord>
struct CoordinateWrapper
{
  CoordinateWrapper(int _resX, int _resY) : resX_(_resX), resY_(_resY)
  {
  }

  void operator()(const Point& p)
  { 
    using boost::geometry::get;
    coords_->push_back(Coord(int(get<0>(p)*resX_),int(get<1>(p)*resY_)));
  }

  std::list<Coord>* coords_;
  
  TBD_PROPERTY(int,resX);
  TBD_PROPERTY(int,resY);
};

typedef boost::geometry::model::linestring< tomo::PointXYf > LineString;
typedef std::vector<LineString> MultiLineString;
typedef std::vector<tomo::Polygon> MultiPolygon;
BOOST_GEOMETRY_REGISTER_MULTI_LINESTRING(MultiLineString);
BOOST_GEOMETRY_REGISTER_MULTI_POLYGON(MultiPolygon);

void paintPolygon(const tomo::Polygon& _polygon, std::string _colorStr, Magick::Image& _image)
{
  CoordinateWrapper<tomo::PointXYf,Magick::Coordinate> _coordWrap(resX,resY);
  std::list<Magick::Coordinate> _coords;
  _coordWrap.coords_ = &_coords;

  _image.fillColor("none");
  boost::geometry::for_each_point( _polygon, _coordWrap );

  _image.strokeColor(Magick::Color(_colorStr)); // Outline color 
  LOG_MSG << _coords.size();
  _image.draw( Magick::DrawablePolyline( _coords )) ;
}

void paintMultiPolygon(const MultiPolygon _multiPolygon, std::string _colorStr, Magick::Image& _image)
{
  BOOST_FOREACH( const tomo::Polygon& _polygon, _multiPolygon) 
    paintPolygon(_polygon,_colorStr,_image);
}


//typedef boost::geometry::model::multi_linestring< LineString > MultiLineString;

void paintLineString(const LineString& _lineString, std::string _colorStr, Magick::Image& _image)
{
  CoordinateWrapper<tomo::PointXYf,Magick::Coordinate> _coordWrap(resX,resY);
  std::list<Magick::Coordinate> _coords;
  _coordWrap.coords_ = &_coords;

  boost::geometry::for_each_point( _lineString, _coordWrap );

  _image.strokeColor(Magick::Color(_colorStr)); // Outline color 
  _image.draw( Magick::DrawablePolyline( _coords )) ;
}

void paintMultiLineString(const MultiLineString& _multiLineString, std::string _colorStr, Magick::Image& _image)
{
  BOOST_FOREACH( const LineString& _lineString, _multiLineString) 
    paintLineString(_lineString,_colorStr,_image);
}

void generateFilling(MultiLineString& _multiLineString)
{
  float _posX = 0.0;
  tomo::PointXYf _p0(0.0,0.0), _p1(0.0,1.0);
  
  while (_posX < 1.0)
  {
    using boost::geometry::set;
    using boost::geometry::append;
    set<0>(_p0,_posX);
    set<0>(_p1,_posX);
    LineString _lineString;
    append(_lineString,_p0);
    append(_lineString,_p1);
    _posX += fillingWidth_;
    _multiLineString.push_back(_lineString);
  }
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
    ("filling", po::value<float>(&fillingWidth_), "Filling width")
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

  Magick::Image _image( Magick::Geometry(resX,resY), Magick::Color("black") );

  tomo::Polygon _polygon1, _polygon2;
 /*   boost::geometry::read_wkt(
        "POLYGON((0.4 0.26,0.48 0.34,0.58 0.36,0.68 0.24,0.74 0.32,0.68 0.4,0.82 0.6,1.0 0.52,1.0 0.24,0.98 0.16,0.58 0.14,0.4 0.26)"
            "(0.8 0.4, 0.82 0.28, 0.96 0.38, 0.88 0.44, 0.8 0.4))", _polygon);
*/


  boost::geometry::read_wkt(
    "POLYGON((0.5 0.0 , 0.4 0.4 , 0.0 0.5 , 0.4 0.6 , 0.5 1.0 , 0.6 0.6 , 1.0 0.5 , 0.6 0.4 , 0.5 0.0))", _polygon1);
  boost::geometry::read_wkt(
    "POLYGON((0.5 0.0 , 0.3 0.3 , 0.0 0.5 , 0.3 0.7 , 0.5 1.0 , 0.7 0.7 , 0.8 0.5 , 0.7 0.3 , 0.5 0.0))", _polygon2);

  paintPolygon(_polygon1,"red",_image);

  MultiLineString _fillingLines, _filling;
  generateFilling(_fillingLines);


  std::vector<Polygon> _polygons;

  boost::geometry::difference(_polygon2,_polygon1,_polygons);
  paintMultiPolygon(_polygons,"green",_image);

  //boost::geometry::difference(_polygons,_fillingLines,_filling);
  //paintMultiLineString(_filling,"blue",_image);
//  boost::geometry::union_<MultiLineString,tomo::Polygon,MultiLineString>(_fillingLines,_polygon,_filling);    
//  paintMultiLineString(_filling,_image);

  _image.display();

  return EXIT_SUCCESS;
}

