#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <boost/program_options.hpp>
#include <boost/foreach.hpp>

#include "tomo/Mesh.hpp"
#include "tomo/Image.hpp"
#include "tbd/log.h"


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

  float _tNear = 0.0; float _tFar = 1.0;

  cout << endl;
  for (int slice = 0; slice < nSlices; slice++)
  {
    Image image(resX,resY);
    for (int x = 0; x < resX; x++)
    {

      Ray ray(Point3f(mesh.bounds().min().x() + mesh.bounds().size().x() / resX *(0.5+x),
                      mesh.bounds().min().y(),
                      mesh.bounds().min().z() + mesh.bounds().size().z() / nSlices *(0.5+slice)),
              Vec3f(0,mesh.bounds().size().y(),0),
              0.0,
              1.0);

      vector<float> ts;

      while (mesh.intersect(ray,_tNear,_tFar))
      {
        ts.push_back(ray.tFar());
        ray.tNear(ray.tFar() + 0.0001);
        ray.tFar(1.0);
    //    LOG_MSG << fmt("% %") % ray.tNear() % ray.tFar();
      }
      if (ts.empty()) continue;
     
      if (ts.size() % 2 != 0) ts.push_back(ts[ts.size()-1]);

      for (int i = 0; i < ts.size(); i+=2)
        for (int j = int(ts[i]*resY); j < int(ts[i+1]*resY); j++)
          image.set(x,j,Color3f(1.0,1.0,1.0));
    }

    stringstream ss; ss << "test" << slice << ".ppm";
    image.save_ppm(ss.str());
  }

/*
  Image image(resX,nSlices);
  for (int slice = 0; slice < nSlices; slice++)
  {
    for (int x = 0; x < resX; x++)
    {
      Ray ray(Point3f(mesh.bounds().min().x() + mesh.bounds().size().x() / resX *(0.5+x),
                      mesh.bounds().min().y(),
                      mesh.bounds().min().z() + mesh.bounds().size().z() / nSlices *(0.5+slice)),
              Vec3f(0,mesh.bounds().size().y(),0),
              0.0,
              1.0);

      if (mesh.intersect(ray,_tNear,_tFar))
          image.set(x,slice,Color3f(1.0,1.0,1.0));
    }
  }

 
  image.save_ppm(outputFile);
*/
  return EXIT_SUCCESS;
}

