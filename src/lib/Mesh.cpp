#include "tomo/Mesh.hpp"

#include <boost/foreach.hpp>

#include <tbd/log.h>

#include <GL/glu.h>

namespace tomo
{
  void Mesh::calcBoundingBox()
  {
    boundingBox().min.set(INF,INF,INF);
    boundingBox().max.set(-INF,-INF,-INF);

    BOOST_FOREACH( Vertex& vertex, vertices_ )
    {
      Point3f v = vertex.v;
      for (int j = 0; j < 3; j++)
      {
        if (v.cell[j] > boundingBox().max.cell[j]) boundingBox().max.cell[j] = v.cell[j];
        if (v.cell[j] < boundingBox().min.cell[j]) boundingBox().min.cell[j] = v.cell[j];
      }
    }
  }

  void Mesh::displayNormals()
  {
    BOOST_FOREACH( Vertex& vertex, vertices_ )
    {
      Vec3f n = boundingBox().size().length()*0.02f*vertex.n; 

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
