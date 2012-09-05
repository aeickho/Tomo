#include <iostream>
#include <boost/assign.hpp>

#include <tbd/log.h>
#include <boost/program_options.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <list>
#include "tomo/Compound.hpp"
#include "tomo/Vertex.hpp"
#include <Magick++.h>

using namespace boost;
namespace po = program_options;

using namespace std;

LOG_INIT;

int main(int ac, char* av[])
{
  cout << "CompoundTest -- written by Wilston Oreo." << endl;

  stringstream descStr;
  descStr << "Allowed options";

  string inputFile, outputFile;
  int resX = 100, resY = 100;
  int nPoints = 100;

  // Declare the supported options.
  po::options_description desc(descStr.str());

  desc.add_options()
    ("help,h", "Display help message.")
    ("output,o", po::value<string>(&outputFile), "Output image file")
    ("points,n", po::value<int>(&nPoints), "Number of points")
    ("resx,x", po::value<int>(&resX), "Resolution X")
    ("resy,y", po::value<int>(&resY), "Resolution Y")
    ;


  using tomo::Vertex2f;
  using tomo::Point2f;
  using tomo::Point2us;
  using tomo::Compound;

  Compound<Vertex2f,2,float> _compound;

  for (int i = 0; i < nPoints; i++)
  {
    _compound.objs().push_back(Vertex2f(Point2f(RND*resX,RND*resY)));
  }

  Magick::Image image( Magick::Geometry(resX,resY), Magick::Color("black") );

  image.strokeColor("red");
  BOOST_FOREACH ( Vertex2f& _vertex, _compound.objs() )
  {
    Point2us _p(_vertex.v.x(),_vertex.v.y());
      image.draw( Magick::DrawableCircle(_p.x(),_p.y(),4+_p.x(),_p.y() ));
  }

  std::vector<Vertex2f*> _inRadius = _compound.collectInRadius(&_compound.objs()[nPoints/2],sqrt((resX*resX + resY*resY))*0.25);
  image.strokeColor("green");
  BOOST_FOREACH ( Vertex2f* _vertex, _inRadius )
  {
    Point2us _p(_vertex->v.x(),_vertex->v.y());
    image.draw( Magick::DrawableCircle(_p.x(),_p.y(),5+_p.x(),_p.y() ));
  }


  std::vector<Vertex2f*> _kNearest = _compound.collectKNearest(&_compound.objs()[nPoints/2],10);
  image.strokeColor("yellow");
  BOOST_FOREACH ( Vertex2f* _vertex, _kNearest )
  {
    Point2us _p(_vertex->v.x(),_vertex->v.y());
    image.draw( Magick::DrawableCircle(_p.x(),_p.y(),6+_p.x(),_p.y() ));
  }

  image.write(outputFile);

  return EXIT_SUCCESS;
}
