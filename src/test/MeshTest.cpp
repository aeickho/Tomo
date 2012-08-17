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

/*
 *       tomo::Point2f _p0,_p1;
      tomo::Vec3f _normal;
      if (_slice->getSegment(_seg,_p0,_p1,_normal))
      {
        _p0((_p0.x() - _slice->bounds_.min().x())/_slice->bounds_.size().x(),(_p0.y() - _slice->bounds_.min().y())/_slice->bounds_.size().y());
        _p1((_p1.x() - _slice->bounds_.min().x())/_slice->bounds_.size().x(),(_p1.y() - _slice->bounds_.min().y())/_slice->bounds_.size().y());
        LOG_MSG << fmt("% %") % _normal.x() % _normal.y();

        image.fillColor(Magick::Color(0,0,0,0));
        image.fillPattern(image);
      image.strokeColor("red"); // Outline color 
      
      Point2i _drawP0(resX*_p0.x(),resY*_p0.y());
      Point2i _drawP1(resX*_p1.x(),resY*_p1.y());

      image.draw( Magick::DrawableLine(_drawP0.x(),_drawP0.y(),_drawP1.x(),_drawP1.y()) );      
      image.strokeColor("yellow");
      image.draw( Magick::DrawableCircle(_drawP0.x(),_drawP0.y(),4+_drawP0.x(),_drawP0.y() ));
      image.strokeColor("blue");
      image.draw( Magick::DrawableCircle(_drawP1.x(),_drawP1.y(),7+_drawP1.x(),_drawP1.y() ));
  
      image.strokeColor("green"); // Outline color 
      image.draw( Magick::DrawableLine(resX*_p0.x(),resY*_p0.y(),
            resX*_p0.x() + resX*_normal.x()/50,
            resY*_p0.y() + resX*_normal.y()/50 ));
      }
*/

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

  tomo::Slices _slices(mesh.bounds().size().z()/nSlices,mesh.bounds());

  LOG_MSG << "Slicing mesh...";

  mesh.slice(_slices);

  std::vector<tomo::Slice*> _allSlices = _slices.get();

  unsigned nSlice = 0;
  LOG_MSG << fmt("Got % slices, %") % _allSlices.size();

  BOOST_FOREACH ( tomo::Slice* _slice, _allSlices)
  {
   // if (nSlice != _allSlices.size()/2) { nSlice++; continue; }
    LOG_MSG << fmt("% %") % nSlice % _slice->polylines_.size();
    Magick::Image image(Magick::Geometry(resX,resY), Magick::Color());

    BOOST_FOREACH ( const tomo::Polyline& _polyline, _slice->polylines_  )
    {
      std::list<Magick::Coordinate> _coordinates;
      BOOST_FOREACH ( const tomo::SliceVertex& _sv, _polyline )
        _coordinates.push_back(Magick::Coordinate(_sv.x() / float(Slice::resolution_)*resX,
                                                  _sv.y() / float(Slice::resolution_)*resY));
     
   //   _coordinates.erase(--_coordinates.end());


      image.strokeColor(Magick::Color(RND*65535,RND*65535,RND*65535)); // Outline color  
  //      image.fillColor(Magick::Color(RND*65535,RND*65535,RND*65535));      
     // image.draw( Magick::DrawablePolygon( _coordinates )) ;
      image.draw( Magick::DrawablePolyline( _coordinates )) ;

    }
    
    //image.d

    stringstream ss; ss << outputFile << nSlice << ".ppm";
    image.write(ss.str());
    nSlice++;
  }

  return EXIT_SUCCESS;
}

