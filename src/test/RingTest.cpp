#include <iostream>

#include <tbd/log.h>
#include <boost/program_options.hpp>
#include "MagickWrapper.hpp"

#include "tomo/geometry/prim/Ring.hpp"

#include <math.h>

using namespace boost;
namespace po = program_options;

using namespace std;

using namespace tomo::geometry;
using prim::Ring;
using prim::LineSegment;
using base::Point2f;
using tomo::magick::Wrapper;

LOG_INIT;

float resX = 1024, resY = 1024;

void makeCircle(Point2f _pos, float _radius, int _segments, Ring& _ring)
{
  for (int i = 0; i < _segments; i++)
  {   
    float _angle = float(i)/float(_segments)*M_PI*2.0;
    float _sin = _radius*sin(_angle),
          _cos = _radius*cos(_angle);
    Point2f _point = Point2f(_cos,_sin) + _pos;
    _ring.add(_point);
  }
}




void intersectTest()
{
  Magick::Image _image( Magick::Geometry(resX,resY), Magick::Color("black") );
  Wrapper _wrapper(_image);
//  _wrapper.drawEndings(true);
  _wrapper.drawNormals(true);

  Ring _ring(Ring::OUTER);
  makeCircle(Point2f(resX/2,resY/2),resX/3,3,_ring);

  _wrapper.draw(_ring,Magick::Color("red"));

  vector<Ring> _rings;
  _ring.resize(30,_rings);
  //_wrapper.draw(_rings[0],Magick::Color("purple"));

  vector<prim::Vertex2f> _vertices;
  _ring.fetchVertices(_vertices);

  BOOST_FOREACH( prim::Vertex2f& _vertex, _vertices )
    _wrapper.draw(_vertex,Magick::Color("white"));
  
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

  return EXIT_SUCCESS;
}

