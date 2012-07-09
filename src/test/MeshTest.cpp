#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <boost/program_options.hpp>
#include <boost/foreach.hpp>
#include <Magick++.h>
#include <tbd/log.h>

#include "tomo/Slice.hpp"
#include "tomo/Mesh.hpp"

using namespace boost;
namespace po = program_options;

using namespace std;
using namespace tomo;

LOG_INIT;

int main(int ac, char* av[])
{
  cout << "MeshTest -- written by Wilston Oreo." << endl;

  stringstream descStr;
  descStr << "Allowed options";

  string inputFile, outputFile;
  int nSlices = 100, resX = 100, resY = 100;

  // Declare the supported options.
  po::options_description desc(descStr.str());

  desc.add_options()
    ("help,h", "Display help message.")
    ("input,i", po::value<string>(&inputFile), "Input OFF file")
    ("output,o", po::value<string>(&outputFile), "Output PPM image file")
    ("slices,n", po::value<int>(&nSlices), "Number of slices")
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

  LOG->level(2);

  tomo::Mesh mesh;
  mesh.read(inputFile);

  tomo::Slices _slices(nSlices,mesh.bounds());

  LOG_MSG << "Slicing mesh...";

  mesh.slice(_slices);

  std::vector<const tomo::Slice*> _allSlices = _slices.get();

  unsigned nSlice = 0;

  LOG_MSG << fmt("Got % slices, %") % _allSlices.size();

  BOOST_FOREACH ( const tomo::Slice* _slice, _allSlices)
  {
    LOG_MSG << nSlice;
    Magick::Image image(Magick::Geometry(resX,resY), Magick::Color());
    image.strokeColor("red"); // Outline color 

    BOOST_FOREACH ( const tomo::LineSegment& _seg, _slice->lineSegments_ )
    {
    image.strokeColor("red"); // Outline color 
      image.draw( Magick::DrawableLine(resX*_seg.p0_.x(),resY*_seg.p0_.y(),resX*_seg.p1_.x(),resY*_seg.p1_.y()));
    
    image.strokeColor("green"); // Outline color 
      image.draw( Magick::DrawableLine(resX*_seg.p0_.x(),resY*_seg.p0_.y(),
                                       resX*_seg.p0_.x() + resX*_seg.normal_.x()/50,
                                       resY*_seg.p0_.y() + resX*_seg.normal_.y()/50 ));
    }

    stringstream ss; ss << "test" << nSlice << ".ppm";
    image.write(ss.str());
    nSlice++;
  }

  return EXIT_SUCCESS;
}

