#include "tomo/Vertex.hpp"

namespace tomo
{
  Vec3f Polygon::normal()
  {
  	Vec3f n; int count = 0;
	  for (unsigned i = 0; i < size()-2; i++)
	  {
		  Point3f a = at(i)->v,b = at(i+2)->v,c = at(i+1)->v;
		  n += cross(a-b,c-b);		
		  count++;
	  }
	  if (count > 0) n.normalize();
	  return n;
  }
}
