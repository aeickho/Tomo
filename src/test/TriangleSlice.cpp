#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <boost/program_options.hpp>
#include <boost/foreach.hpp>

#include "cg2/Plane.hpp"
#include "cg2/Triangle.hpp"
#include "tbd/log.h"

using namespace boost;
namespace po = program_options;

using namespace std;
using namespace cg2;

LOG_INIT;

Triangles splitTriangle(const Triangle& tri, const Plane& plane)
{
  Triangles triangles;

  Vertex* A(tri->v0), B(tri->v1), C(tri->v2);
  
  Ray rayAB(A->v,B->v - A->v), 
      rayAC(A->v,C->v - A->v), 
      rayBC(B->v,C->v - B->v);
  rayAB.tmin = 0.0f; rayAB.tmax = 1.0f;
  rayAC.tmin = 0.0f; rayAC.tmax = 1.0f;
  rayBC.tmin = 0.0f; rayBC.tmax = 1.0f;

  typedef enum { AB, AB, BC, NONE } Edge;

  unsigned idx = 0; 
  Point3f iPoint[2]; Edge edge[2];
  edge[0] = NONE; edge[1] = NONE;

  if (plane.intersect(rayAB))
  {
    iPoint[idx] = rayAB.getIntersectionPoint(); 
    edge[idx] = AB;
    idx++;
  }
  
  if (plane.intersect(rayAC))
  {
    iPoint[idx] = rayAC.getIntersectionPoint(); 
    edge[idx] = AC;
    idx++;
  }

  if (idx < 2)
    if (plane.intersect(rayBC))
    {
      iPoint[idx] = rayBC.getIntersectionPoint(); 
      edge[idx] = BC;
      idx++;
    }

  switch (idx)
  {
    // No intersection point found, return input triangle
    case 0: 
      triangles.push_back(tri); break;
    case 1:
    {
      Vertex* v = new Vertex(iPoint[0]);
      switch (edge[0])
      {
        case AB: 
          triangles.push(Triangle(A,v,C));
          triangles.push(Triangle(v,B,C));
          v->n = rayAB.tmax*A->n + (1.0f-rayAB.tmax)*B->n; 
          break;
        case AC: 
          triangles.push(Triangle(A,B,v));
          triangles.push(Triangle(v,B,C));
          v->n = rayAC.tmax*A->n + (1.0f-rayAC.tmax)*C->n; 
          break;
        case BC: 
          triangles.push(Triangle(A,v,C));
          triangles.push(Triangle(A,B,v));
          v->n = rayBC.tmax*B->n + (1.0f-rayBC.tmax)*C->n; 
          break;
      }
    }
    case 2: 
    {
      Vertex* v0 = new Vertex(iPoint[0]);
      Vertex* v1 = new Vertex(iPoint[1]);
      switch (edge[0])
      {
        case AB: v0->n = rayAB.tmax*A->n + (1-rayAB.tmax)*B->n; break;
        case AC: v0->n = rayAC.tmax*A->n + (1-rayAC.tmax)*C->n; break;
        case BC: v0->n = rayBC.tmax*B->n + (1-rayBC.tmax)*C->n; break;
      }
      switch (edge[1])
      {
        case AB: v1->n = rayAB.tmax*A->n + (1-rayAB.tmax)*B->n; break;
        case AC: v1->n = rayAC.tmax*A->n + (1-rayAC.tmax)*C->n; break;
        case BC: v1->n = rayBC.tmax*B->n + (1-rayBC.tmax)*C->n; break;
      }

      if (edge[0] == AB && edge[1] == AC)
      {
         triangles.push_back(Triangle(A,v0,v1));
         triangles.push_back(Triangle(v0,v1,C));
         triangles.push_back(Triangle(v0,B ,C));
         return triangles;
      }
      if (edge[0] == AB && edge[1] == BC)
      {
         triangles.push_back(Triangle(v0,B,v1));
         triangles.push_back(Triangle(A,v0, C));
         triangles.push_back(Triangle(v0,v1,C));
         return triangles;
      }
      if (edge[0] == AC && edge[1] == BC)
      {
         triangles.push_back(Triangle(v0,v1,C));
         triangles.push_back(Triangle(A,B, v0));
         triangles.push_back(Triangle(A,v0,v1));
         return triangles;
      }

    default: 
  }

  return triangles;
}

int main(int ac, char* av[])
{
  cout << "MeshTest -- written by Wilston Oreo." << endl;

  stringstream descStr;
  descStr << "Allowed options";

  // Declare the supported options.
  po::options_description desc(descStr.str());

  desc.add_options() ("help,h", "Display help message.");

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

  Vertex v0,v1,v2;
  Triangle tri(&v0,&v1,&v2);

  Plane plane();
  Triangles triangles = splitTriangle(tri,plane);

  return EXIT_SUCCESS;
}

