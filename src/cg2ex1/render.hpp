#include <tomo/misc.hpp>
#include <tomo/Mesh.hpp>
#include <cmath>

template<class COORD> void glRotate(COORD _phi, COORD _theta)
{
  glRotatef(_phi, 0.0, 0.0, 1.0);
  glRotatef(_theta, 0.0, 1.0, 0.0);
}

template<class POINT> void glRotate(const POINT& _direction)
{
  typedef typename POINT::Coord Coord;
  Coord length = _direction.length();
  Coord phi = tomo::rad2deg( atan2(_direction.y(), _direction.x()) );
  Coord theta = (0.0 != length) ? tomo::rad2deg(acos( (_direction.z() / length) )) : 0.0;
  glRotate(phi,theta);
}

inline void drawBackground()
{
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glDisable(GL_DEPTH_TEST);
  glBegin(GL_QUADS);

  glColor3f(0.0,0.0,0.0);
  glVertex2f(-1.0,-1.0);
  glVertex2f(1.0,-1.0);

  glColor3f(54.0/255.0,110.0/255.0,201.0/255.0);
  glVertex2f(1.0, 1.0);
  glVertex2f(-1.0, 1.0);
  glEnd();

  glEnable(GL_DEPTH_TEST);

  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
}

// realize camera
template<class CAMERA> void realizeCamera(const CAMERA& _camera)
{
  // LOG_MSG << fmt("eye    = %,%,%") % camera_.eye().x() % camera_.eye().y() % camera_.eye().z();
  // LOG_MSG << fmt("center = %,%,%") % camera_.center().x() % camera_.center().y() % camera_.center().z();

  // realize coordinates
  gluLookAt(
    _camera.eye().x(),
    _camera.eye().y(),
    _camera.eye().z(),
    _camera.center().x(),
    _camera.center().y(),
    _camera.center().z(),
    _camera.up().x(),
    _camera.up().y(),
    _camera.up().z()
  );
}


inline void drawBed()
{
  // draw bed
  GLfloat bedXsize = 210;
  GLfloat bedYsize = 210;
  GLfloat bedXmin = -bedXsize/2;
  GLfloat bedXmax = bedXsize/2;
  GLfloat bedYmin = -bedYsize/2;
  GLfloat bedYmax = bedYsize/2;
  GLfloat bedZ = 0;//-mesh_.bounds().size().z()/2.0;
  {
    glBegin(GL_QUADS);
    {
      glColor4f(0.5,0.5,0.5,0.8);
      glVertex3f(bedXmin,bedYmin,bedZ);
      glVertex3f(bedXmax,bedYmin,bedZ);
      glVertex3f(bedXmax,bedYmax,bedZ);
      glVertex3f(bedXmin,bedYmax,bedZ);
    }
    glEnd();
  }

  // draw grid
  {
    GLfloat tick=1.0;
    {
      // lines X axis
      glLineWidth( 1.0 );
      glBegin(GL_LINES);
      {
        for (int i = 0; tick*i <= bedXsize/2; i++)
        {
          glColor4f(0.2,0.6,0.2,(i%10)?0.4:0.8);
          glVertex3f(tick*i, bedYmin, bedZ);
          glVertex3f(tick*i, bedYmax, bedZ);
          glVertex3f(-tick*i, bedYmin, bedZ);
          glVertex3f(-tick*i, bedYmax, bedZ);
        }
      }
      glEnd();

      // lines Y axis
      glBegin(GL_LINES);
      {
        for (int i = bedYmin; i <= bedYsize/2; i++)
        {
          glColor4f(0.6,0.0,0.2,(i%10)?0.4:0.8);
          glVertex3f(bedXmin, tick*i, bedZ);
          glVertex3f(bedXmax, tick*i, bedZ);
          glVertex3f(bedXmin, -tick*i, bedZ);
          glVertex3f(bedXmax, -tick*i, bedZ);
        }
      }
      glEnd();
    }
  }

}

template<class POINT, class COLOR> void drawArrow( const std::string& _label, const POINT& _p1, const POINT& _p2, const COLOR& _color, GLfloat _width=1.0, GLfloat _arrowR=2.0, GLfloat _arrowH=1.0 )
{
  glLineWidth(_width);
  glBegin(GL_LINES);
  {
    glColor4fv(_color);
    glVertex4fv(_p1);
    glVertex3fv(_p2);
  }
  glEnd();

  glPushMatrix();
  {
    glTranslatef(COORDS(_p2));
    glPushMatrix();
    {
      glRotate(_p2-_p1);
      glColor4fv(_color);
      glutSolidCone(_arrowH, _arrowR, 16.0, 1.0);
    }
    glPopMatrix();
    glRotatef(90.0,1.0,0.0,0.0);
    glScalef(0.05,0.05,0.05);
    BOOST_FOREACH(char ch, _label)
      glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN,ch);
  }
  glPopMatrix();
}

inline void drawAxis()
{
  glDisable(GL_DEPTH_TEST);
  {
    int length = 10.0;
    // draw axis
    GLfloat axisAlpha=0.7;
    {
      drawArrow("x", GLWidget::Point(0.0,0.0,0.0), GLWidget::Point(length,0.0,0.0), GLWidget::Color(1.0,0.0,0.0,axisAlpha), 2.0);
      drawArrow("y", GLWidget::Point(0.0,0.0,0.0), GLWidget::Point(0.0,length,0.0), GLWidget::Color(0.0,1.0,0.0,axisAlpha), 2.0);
      drawArrow("z", GLWidget::Point(0.0,0.0,0.0), GLWidget::Point(0.0,0.0,length), GLWidget::Color(0.0,0.0,1.0,axisAlpha), 2.0);
    }
  }
  glEnable(GL_DEPTH_TEST);
}

template<class LIGHT> void drawLight( LIGHT _light)
{
  // move light
  glLightfv(GL_LIGHT1, GL_POSITION, _light.eye());
  glEnable(GL_LIGHTING);
}

inline void drawObject( tomo::TriangleMesh& mesh )
{
  glPushMatrix();
  {
    tomo::Vec3f c = 0.5*(mesh.bounds().max().vec() + mesh.bounds().min().vec());
    glTranslatef(-c.x(),-c.y(),-c.z());
    glColor3f(0.8,0.8,0.8);
    glBegin(GL_TRIANGLES);
    BOOST_FOREACH( const tomo::Triangle& tri, mesh.triangles() )
    {
      glNormal3f(COORDS(tri.normal()));
      for (int i  = 0; i < 3; i++)
        glVertex3fv(tri.v[i].p());
    }
    glEnd();
  }
  glPopMatrix();
}

template<class POINT, class COLOR> void drawSelection( const POINT& _selection, const COLOR& _color )
{
  glPointSize(4.0);
  glBegin(GL_POINTS);
  {
    glColor3fv(_color);
    glVertex3fv(_selection);
  }
  glEnd();
}

