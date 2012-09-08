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
#include "tomo/LineSegment.hpp"
#include <Magick++.h>

using namespace boost;
namespace po = program_options;

using namespace std;

LOG_INIT;


string inputFile, outputFile;
int resX = 100, resY = 100;
int nPoints = 100;


template <typename PRIMITIVE, int DIMENSIONS , typename SCALAR>
void paintKDNode(const tomo::KDTree<PRIMITIVE,DIMENSIONS,SCALAR>& _kdTree,
                 const tomo::KDNode<PRIMITIVE,DIMENSIONS,SCALAR>* _node,
                 tomo::Bounds<DIMENSIONS,SCALAR> _bounds,
                 Magick::Image& _image)
{
  if (_node->isLeaf()) return;

  tomo::Bounds<DIMENSIONS,SCALAR> _left, _right;
  tomo::Axis _axis = _node->inner_.axis();
  SCALAR _splitPos = _node->inner_.splitPos();
  _bounds.split(_splitPos,_axis,_left,_right);

  switch (_axis)
  {
  case tomo::X:
    _image.draw( Magick::DrawableLine(_splitPos,_bounds.min().y(),_splitPos,_bounds.max().y()) );
    break;
  case tomo::Y:
    _image.draw( Magick::DrawableLine(_bounds.min().x(),_splitPos,_bounds.max().x(),_splitPos) );
    break;
  default:
    break;
  }

  paintKDNode(_kdTree,_kdTree.node(_node->inner_.left()),_left,_image);
  paintKDNode(_kdTree,_kdTree.node(_node->inner_.right()),_right,_image);
}

void vertexCompoundTest()
{
  using tomo::Vertex2f;
  using tomo::Point2f;
  using tomo::Point2us;
  using tomo::Compound;

  Compound<Vertex2f,2,float> _compound;

  // Generate point randomly
  for (int i = 0; i < nPoints; i++)
  {
    _compound.objs().push_back(Vertex2f(Point2f(RND*resX,RND*resY)));
  }
  // Generate bounds + kdTree
  _compound.update();

  Magick::Image image( Magick::Geometry(resX,resY), Magick::Color("black") );

  // Draw KDTree
  image.strokeColor("gray");
  paintKDNode<Vertex2f,2,float>(_compound.kdTree(),&_compound.kdTree().root(),_compound.bounds(),image);

  // Draw all points
  image.strokeColor("red");
  BOOST_FOREACH ( Vertex2f& _vertex, _compound.objs() )
  {
    Point2us _p(_vertex.v.x(),_vertex.v.y());
    image.draw( Magick::DrawableCircle(_p.x(),_p.y(),4+_p.x(),_p.y() ));
  }

  // Draw chosen point
  image.strokeColor("yellow");
  image.draw( Magick::DrawableCircle(_compound.objs()[nPoints/2].v.x(),_compound.objs()[nPoints/2].v.y(),
                                     7+_compound.objs()[nPoints/2].v.x(),_compound.objs()[nPoints/2].v.y()));

  // Draw points which inside radius of chosen point
  std::vector<Vertex2f*> _inRadius = _compound.collectInRadius(&_compound.objs()[nPoints/2],sqrt((resX*resX + resY*resY))*0.125);
  image.strokeColor("green");
  BOOST_FOREACH ( Vertex2f* _vertex, _inRadius )
  {
    Point2us _p(_vertex->v.x(),_vertex->v.y());
    image.draw( Magick::DrawableCircle(_p.x(),_p.y(),5+_p.x(),_p.y() ));
  }

  // Draw nearest k points of chose point
  std::vector<Vertex2f*> _kNearest = _compound.collectKNearest(&_compound.objs()[nPoints/2],10);
  image.strokeColor("blue");
  BOOST_FOREACH ( Vertex2f* _vertex, _kNearest )
  {
    Point2us _p(_vertex->v.x(),_vertex->v.y());
    image.draw( Magick::DrawableCircle(_p.x(),_p.y(),6+_p.x(),_p.y() ));
  }

  // Draw nearest point
  image.strokeColor("white");
  Vertex2f* _nearest = _compound.nearest(&_compound.objs()[nPoints/2]);
  Point2us _p(_nearest->v.x(),_nearest->v.y());
  image.draw( Magick::DrawableCircle(_p.x(),_p.y(),10+_p.x(),_p.y() ));

  image.display();
  if (!outputFile.empty())
    image.write(outputFile);
}

void getPointFromStr(string _token, tomo::Point2f& _point)
{
  vector<string> _tokens;
  boost::split(_tokens,_token,boost::is_any_of(","), boost::token_compress_on);

  if (_tokens.size() != 2) return;
  _point(atof(_tokens[0].c_str()),atof(_tokens[1].c_str()));

  LOG_MSG << fmt("% % = % %") % _tokens[0] % _tokens[1] % _point.x() % _point.y();
}

void readLineDescFile(string _filename, tomo::LineSegmentPlane& _plane, int& _resX, int& _resY)
{
  ifstream ifs(_filename.c_str());

  bool found = false;

  while (ifs.good())
  {
    string _buf;
    getline(ifs,_buf);

    LOG_MSG_(4) << _buf;

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
      tomo::Point2f _p0,_p1;
      getPointFromStr(_tokens[1],_p0);
      getPointFromStr(_tokens[2],_p1);
      _plane.addSegment(_p0,_p1);
    }
  }

  ifs.close();
}

void paintLineSegment(const tomo::LineSegment& _segment, Magick::Image& _image, string _colorStr = "red", bool paintEndings = true )
{
  tomo::Point2f _b = _segment.back();
  tomo::Point2f _f = _segment.front();

  _image.strokeColor(_colorStr);
  _image.draw( Magick::DrawableLine(_f.x(),_f.y(),_b.x(),_b.y()) );

  if (paintEndings)
  {
    _image.strokeColor("yellow");
    _image.draw( Magick::DrawableCircle(_f.x(),_f.y(),4+_f.x(),_f.y() ));
    _image.strokeColor("blue");
    _image.draw( Magick::DrawableCircle(_b.x(),_b.y(),7+_b.x(),_b.y() ));
  }
}

void lineSegmentTest()
{
  using tomo::LineSegment;
  using tomo::Point2f;
  using tomo::Point2us;
  using tomo::LineSegmentPlane;

  LineSegmentPlane _plane;
  int resX,resY;
  readLineDescFile(inputFile,_plane,resX,resY);
  _plane.update();

  Magick::Image image( Magick::Geometry(resX,resY), Magick::Color("black") );
  image.fillColor("none");

  // Draw KDTree
  image.strokeColor("gray");
  paintKDNode<LineSegment,2,float>(_plane.kdTree(),&_plane.kdTree().root(),_plane.bounds(),image);

  BOOST_FOREACH( LineSegment& _segment, _plane.objs() )
  paintLineSegment(_segment,image);

  LineSegment* _chosenSegment = &_plane.objs()[_plane.objs().size()/3];
  paintLineSegment(*_chosenSegment,image,"yellow",false);

  // Draw nearest k segments of chose segment
  std::vector<LineSegment*> _kNearest = _plane.collectKNearest(_chosenSegment,10);
  BOOST_FOREACH ( LineSegment* _segment, _kNearest )
  {
    paintLineSegment(*_segment,image,"blue",false);
  }

  // Draw nearest segment
  LineSegment* _nearest = _plane.nearest(_chosenSegment);
  if (_nearest)
  {
    paintLineSegment(*_nearest,image,"white",false);
  }

  image.display();
  if (!outputFile.empty())
    image.write(outputFile);
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

  return EXIT_SUCCESS;
}
