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



int main(int ac, char* av[])
{
  cout << "CompoundTest -- written by Wilston Oreo." << endl;

//  srand((long long)av);
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
  // Parse the command line arguments for all supported options
  po::variables_map vm;
  po::store(po::parse_command_line(ac, av, desc), vm);
  po::notify(vm);

  if (vm.count("help"))
  {
    cout << desc << endl;
    return 1;
  }

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
  image.write(outputFile);

  return EXIT_SUCCESS;
}
