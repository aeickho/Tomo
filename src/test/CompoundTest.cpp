#include <iostream>

#include "MagickWrapper.hpp"
#include <tbd/log.h>
#include <boost/program_options.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <list>

using namespace boost;
namespace po = program_options;

using namespace std;

LOG_INIT;


string inputFile, outputFile;
int resX = 1024, resY = 1024;
int nPoints = 100;

void vertexCompoundTest()
{
  namespace tg = tomo::geometry;
  using tg::prim::Vertex2f;
  using tg::base::Point2f;
  using tg::base::Point2us;
  using tg::aux::Compound;
  using tomo::magick::Wrapper;

  Compound<Vertex2f,2,float> _compound;

  // Generate point randomly
  for (int i = 0; i < nPoints; i++)
  {
    _compound.objs().push_back(Vertex2f(Point2f(RND*resX,RND*resY)));
  }
  // Generate bounds + kdTree
  _compound.update();

  Magick::Image _image( Magick::Geometry(resX,resY), Magick::Color("black") );
  Wrapper _wrapper(_image);

  // Draw KDTree
  _wrapper.draw<Vertex2f,2,float>(_compound.kdTree(),"gray");

  // Draw all points
  _wrapper.vertexWidth(4.0);
  _wrapper.draw<Vertex2f,2,float>(_compound,"red");

  // Draw chosen point
  _wrapper.draw(_compound.objs()[nPoints/2],"yellow");

/*
  // Draw points which inside radius of chosen point
  std::vector<Vertex2f*> _inRadius = _compound.collectInRadius(&_compound.objs()[nPoints/2],sqrt((resX*resX + resY*resY))*0.125);
  _wrapper.vertexWidth(5.0);
  _wrapper.draw<Vertex2f>(_inRadius,"green");
  // Draw nearest k points of chose point
  std::vector<Vertex2f*> _kNearest = _compound.collectKNearest(&_compound.objs()[nPoints/2],10);
  _wrapper.vertexWidth(6.0);
  _wrapper.draw<Vertex2f>(_inRadius,"blue");

  // Draw nearest point
  _wrapper.vertexWidth(10.0);
  Vertex2f* _nearest = _compound.nearest(&_compound.objs()[nPoints/2]);
  _wrapper.draw(*_nearest,"white");
*/
  _image.display();
  if (!outputFile.empty())
    _image.write(outputFile);
}

void getPointFromStr(string _token, tomo::geometry::base::Point2f& _point)
{
  vector<string> _tokens;
  boost::split(_tokens,_token,boost::is_any_of(","), boost::token_compress_on);

  if (_tokens.size() != 2) return;
  _point(atof(_tokens[0].c_str()),atof(_tokens[1].c_str()));
}

void readLineDescFile(string _filename, tomo::slicing::LineSegmentPlane& _plane, int& _resX, int& _resY)
{
  ifstream ifs(_filename.c_str());

  bool found = false;

  while (ifs.good())
  {
    string _buf;
    getline(ifs,_buf);

    vector<string> _tokens;
    boost::split(_tokens,_buf,boost::is_any_of("\t "), boost::token_compress_on);

    if (_tokens.size() == 1)
      if (_tokens[0] == "END") return;

    if (_tokens.size() < 3) continue;

    if (_tokens[0] == "IMAGE" && _tokens.size() == 4)
    {
      if (found) return;
      _resX = atoi(_tokens[2].c_str());
      _resY = atoi(_tokens[3].c_str());
      found = true;
    }
    else
    {
      tomo::geometry::base::Point2f _p0,_p1;
      getPointFromStr(_tokens[1],_p0);
      getPointFromStr(_tokens[2],_p1);
      _plane.addSegment(_p0,_p1);
    }
  }

  ifs.close();
}


void lineSegmentTest()
{
  namespace tg = tomo::geometry;
  using tg::prim::LineSegment;
  using tg::base::Point2f;
  using tg::base::Point2us;
  using tomo::slicing::LineSegmentPlane;
  using tomo::magick::Wrapper;

  LineSegmentPlane _plane;
  int resX,resY;
  readLineDescFile(inputFile,_plane,resX,resY);
  _plane.update();

  Magick::Image _image( Magick::Geometry(resX,resY), Magick::Color("black") );
  Wrapper _wrapper(_image);

  // Draw KDTree
  _wrapper.draw<LineSegment,2,float>(_plane.kdTree(),"gray");

  _wrapper.drawEndings(true);
  _wrapper.draw<LineSegment,2,float>(_plane,"red");

  _wrapper.drawEndings(false);
  LineSegment* _chosenSegment = &_plane.objs()[_plane.objs().size()/3];
  _wrapper.draw(*_chosenSegment,"yellow");

  // Draw nearest k segments of chose segment
  std::vector<LineSegment*> _kNearest = _plane.collectKNearest(_chosenSegment,10);
  _wrapper.draw<LineSegment>(_kNearest,"blue");

  // Draw nearest segment
  LineSegment* _nearest = _plane.nearest(_chosenSegment);
  if (_nearest) _wrapper.draw(*_nearest,"white");

  _image.display();
  if (!outputFile.empty())
    _image.write(outputFile);
}

void polygonTest()
{
  namespace tg = tomo::geometry;
  using tg::prim::LineSegment;
  using tg::base::Point2f;
  using tg::base::Point2us;
  using tg::prim::Polygon;
  using tg::prim::MultiPolygon;
  using tomo::slicing::LineSegmentPlane;
  using tomo::magick::Wrapper;

  LineSegmentPlane _plane;
  int resX,resY;
  readLineDescFile(inputFile,_plane,resX,resY);
  _plane.update();

  Magick::Image _image( Magick::Geometry(resX,resY), Magick::Color("black") );
  Wrapper _wrapper(_image);

  // Draw KDTree
  _wrapper.draw<LineSegment,2,float>(_plane.kdTree(),"gray");

  MultiPolygon _polygons = _plane.makePolygons();
  BOOST_FOREACH( Polygon& _polygon, _polygons )
  {
    std::stringstream ss;
    //  ss << "rgb(" << RND << "," << RND << "," << RND << ")";
    _wrapper.draw(_polygon,Magick::Color(RND*65535,RND*65535,RND*65535));
  }

  _image.display();
  if (!outputFile.empty())
    _image.write(outputFile);
}



int main(int ac, char* av[])
{
  cout << "CompoundTest -- written by Wilston Oreo." << endl;

//  srand((long long)av);
  stringstream descStr;
  descStr << "Allowed options";

  // Declare the supported options.
  po::options_description desc(descStr.str());

  desc.add_options()
  ("help,h", "Display help message.")
  ("input,i", po::value<string>(&inputFile), "Input descriptor file")
  ("output,o", po::value<string>(&outputFile), "Output image file")
  ("points,n", po::value<int>(&nPoints), "Number of points")
  ("resx,x", po::value<int>(&resX), "Resolution X")
  ("resy,y", po::value<int>(&resY), "Resolution Y")
  ("vertex", "Perform vertex compound test")
  ("linesegment", "Perform line segment test")
  ("polygon","Perform polygon test")
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

  if (vm.count("vertex")) vertexCompoundTest();
  if (vm.count("linesegment")) lineSegmentTest();
  if (vm.count("polygon")) polygonTest();

  return EXIT_SUCCESS;
}
