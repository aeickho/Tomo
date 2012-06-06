#include "tomo/Mesh.hpp"

#include <boost/foreach.hpp>

#include <tbd/log.h>

#include <GL/glu.h>

namespace tomo
{
  void Mesh::calcBoundingBox()
  {
    boundingBox.min.set(INF,INF,INF);
    boundingBox.max.set(-INF,-INF,-INF);

    BOOST_FOREACH( Vertex& vertex, vertices )
    {
      Point3f v = vertex.v;
      FOREACH_AXIS
      {
        if (v.cell[axis] > boundingBox.max.cell[axis]) boundingBox.max.cell[axis] = v.cell[axis];
        if (v.cell[axis] < boundingBox.min.cell[axis]) boundingBox.min.cell[axis] = v.cell[axis];
      }
    }
  }

  void Mesh::displayNormals()
  {
    BOOST_FOREACH( Vertex& vertex, vertices )
    {
      Vec3f n = boundingBox.size().length()*0.02f*vertex.n; 

      glPushMatrix();
      glTranslatef(COORDS(vertex.v));
      glBegin(GL_LINES);
      glColor3f( 0.0, 1.0, 0.0);
      glVertex3f( 0.0 , 0.0 , 0.0 );
      glVertex3fv(n.p());
      glColor3f( 0.0, 0.0, 0.0);
      glEnd();
      glPopMatrix();
    }
  }


}
