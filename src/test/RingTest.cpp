#include <iostream>

#include <tbd/log.h>
#include <boost/program_options.hpp>
#include <boost/geometry.hpp>
#include <boost/foreach.hpp>
#include <math.h>

#include <list>

#include "MagickWrapper.hpp"
#include "tomo/geometry/prim/MultiPolygon.hpp"
#include <boost/intrusive/list.hpp>

using namespace boost;
namespace po = program_options;

using namespace std;

using namespace tomo::geometry;
using prim::Ring;
using prim::BoostRing;
using prim::Polygon;
using prim::MultiPolygon;
using prim::LineSegment;
using base::Point2f;
using tomo::magick::Wrapper;

LOG_INIT;

float resX = 1024, resY = 1024;

void makeCircle(Point2f _pos, float _radius, int _segments, bool _inverse, Ring& _ring)
{
  for (int i = 0; i < _segments; i++)
  {
    float _angle = float(i)/float(_segments)*M_PI*2.0;
    float _sin = _radius*sin(_angle),
          _cos = _radius*cos(_angle);

    if (_inverse) _sin = -_sin;

    Point2f _point = Point2f(_cos,_sin) + _pos;
    _ring.add(_point);
  }
}


Magick::Color ringOrientationColor(const Ring& _ring)
{
  return (_ring.orientation() == Ring::CCW) ?
         Magick::Color("red") :
         Magick::Color("blue");
}


void polygonSortTest()
{
  Magick::Image _image( Magick::Geometry(resX,resY), Magick::Color("black") );
  Wrapper _wrapper(_image);
  _wrapper.drawNormals(false);

  Ring _ring1(Ring::OUTER);
  makeCircle(Point2f(resX/2,resY/2),resX/2.6,24,false,_ring1);
  Ring _ring2(Ring::INNER);
  makeCircle(Point2f(resX/3*2,resY/3*2),resX/8,24,true,_ring2);
  Ring _ring3(Ring::OUTER);
  makeCircle(Point2f(resX/3,resY/3),resX/8,24,false,_ring3);
  Ring _ring4(Ring::OUTER);
  makeCircle(Point2f(resX/4,resY/5),resX/7,24,false,_ring4);


  _wrapper.draw(_ring1,ringOrientationColor(_ring1));
  _wrapper.draw(_ring2,ringOrientationColor(_ring2));
  _wrapper.draw(_ring3,ringOrientationColor(_ring3));
  _wrapper.draw(_ring4,ringOrientationColor(_ring4));

  //_wrapper.draw(_rings[0],Magick::Color("purple"));

  vector<prim::Vertex2f> _vertices;
  _ring1.fetchVertices(_vertices);

  BOOST_FOREACH( prim::Vertex2f& _vertex, _vertices )
  _wrapper.draw(_vertex,Magick::Color("white"));

  vector<Ring> _rings;
  _rings.push_back(_ring1);
  _rings.push_back(_ring2);
  _rings.push_back(_ring3);
  _rings.push_back(_ring4);


  vector<BoostRing> _newRings;
  boost::geometry::intersection(_ring1(),_ring4(),_newRings);

  _wrapper.drawNormals(true);
  BOOST_FOREACH( const BoostRing& _newRing, _newRings )
  {
    Ring _ring(_newRing);
    _wrapper.draw(_ring,Magick::Color("yellow"));
  }


  _image.display();

}

void intersectTest()
{
  Magick::Image _image( Magick::Geometry(resX,resY), Magick::Color("black") );
  Wrapper _wrapper(_image);

  Ring _ring(Ring::OUTER);
  makeCircle(Point2f(resX/2,resY/2),resX/3.0,24,false,_ring);
  LineSegment _lineSegment(Point2f(resX/5,resY/5),Point2f(resX*4/5,resY*4/5));

  _wrapper.draw(_ring,ringOrientationColor(_ring));
  _wrapper.draw(_lineSegment,Magick::Color("green"));

  _image.display();
}

typedef std::vector<Ring> Rings;

void unify(const Rings& _input, Rings& _output)
{
  typedef list<BoostRing> Container;
  Container _inputRings;

  BOOST_FOREACH( const Ring& _ring, _input)
  {
    BoostRing _newRing(_ring());
    boost::geometry::correct(_newRing);
    _inputRings.push_back(_newRing);
  }

  typedef Container::iterator iterator;
  for (iterator i = _inputRings.begin(); i != _inputRings.end(); ++i)
  {
    iterator j = i;
    ++j;
    for (; j != _inputRings.end(); ++j)
    {
      vector<BoostRing> _newRings;
      boost::geometry::union_(*i,*j,_newRings);
      if (_newRings.size() == 1)
      {
        *i = _newRings.back();
        _inputRings.erase(j);
        j = i;
      }
    }
  }

  BOOST_FOREACH ( const BoostRing& _inputRing, _inputRings )
  {
      _output.push_back(Ring(_inputRing));
  }
}




void unifyTest()
{
  Magick::Image _image( Magick::Geometry(resX,resY), Magick::Color("black") );
  Wrapper _wrapper(_image);

  vector<Ring> _rings, _output;
  Point2f _pos(resX/6,resY/6);
  for (int i = 0; i < 8; i++)
  {
    Ring _ring(Ring::OUTER);

    if (i != 2 && i != 4)
    {
      makeCircle(_pos,resX/10,24,false,_ring);
    }
    _pos += Point2f(resX/10,resY/10);
    _rings.push_back(_ring);
  }

  unify(_rings,_output);

  BOOST_FOREACH( const Ring& _ring, _rings )
  _wrapper.draw(_ring,Magick::Color("green"));
  BOOST_FOREACH( const Ring& _ring, _output )
  _wrapper.draw(_ring,Magick::Color("red"));

  _image.display();
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
  ("intersect","Test intersection")
  ("unify","Test unify")
  ("sort","Test ring sorting into MultiPolygon")
  /*  ("output,o", po::value<string>(&outputFile), "Output image file")
    ("resx,x", po::value<int>(&resX), "Resolution X")
    ("resy,y", po::value<int>(&resY), "Resolution Y")
    ("fillingwidth,w", po::value<float>(&fillingWidth_), "Filling width")
    ("union", "Test union")
    ("perimeter", "Test filling")
    ("polygonsort", "Test filling")*/
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

  if (vm.count("intersect")) intersectTest();
  if (vm.count("unify")) unifyTest();
  if (vm.count("sort")) polygonSortTest();

  return EXIT_SUCCESS;
}

