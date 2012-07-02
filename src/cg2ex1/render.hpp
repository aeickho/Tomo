#include <tomo/misc.hpp>
#include <tomo/Mesh.hpp>
#include <tomo/PrintBounds.hpp>
#include <cmath>

/** @brief executes glRotate() for the given angles
 * @tparam COORD coordinate and angle type
 * @param _phi rotation arround the z-axis
 * @param _theta rotation arround the y-axis
 */
template<class COORD> void glRotate(COORD _phi, COORD _theta)
{
  glRotatef(_phi, 0.0, 0.0, 1.0);
  glRotatef(_theta, 0.0, 1.0, 0.0);
}

/** @brief rotate in the given direction
 * @tparam VEC point type including POINT::Coord as coord type
 * @param _direction vector which points in the desired direction
 */
template<class VEC> void glRotate(const VEC& _direction)
{
  // get coordinate type from vector type
  typedef typename VEC::Coord Coord;
  // pre-calculate vector length
  Coord length = _direction.length();
  // calculate phi and theta (@link http://de.wikipedia.org/wiki/Kugelkoordinaten#.C3.9Cbliche_Konvention)
  Coord phi = tomo::rad2deg( atan2(_direction.y(), _direction.x()) );
  Coord theta = (0.0 != length) ? tomo::rad2deg(acos( (_direction.z() / length) )) : 0.0;
  // rotate GL world
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
}

// realize camera
template<class CAMERA> void realizeCamera(const CAMERA& _camera)
{
  glMatrixMode(GL_MODELVIEW);
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

inline void drawBed( const tomo::PrintBounds& _bounds )
{
  glMatrixMode(GL_MODELVIEW);
  // draw _bounds.min().
  glBegin(GL_QUADS);
  {
    glColor4f(0.5,0.5,0.5,0.8);
    glVertex3f(_bounds.min().x(), _bounds.min().y(), _bounds.min().z());
    glVertex3f(_bounds.max().x(), _bounds.min().y(), _bounds.min().z());
    glVertex3f(_bounds.max().x(), _bounds.max().y(), _bounds.min().z());
    glVertex3f(_bounds.min().x(), _bounds.max().y(), _bounds.min().z());
  }
  glEnd();
}

inline void drawPrintRange( const tomo::PrintBounds& _bounds )
{
  // draw printable range
  glBegin(GL_QUADS);
  {
    glColor4f(1.0,1.0,1.0,0.1);
    glVertex3f(_bounds.max().x(), _bounds.min().y(), _bounds.min().z());
    glVertex3f(_bounds.min().x(), _bounds.min().y(), _bounds.min().z());
    glVertex3f(_bounds.min().x(), _bounds.min().y(), _bounds.max().z());
    glVertex3f(_bounds.max().x(), _bounds.min().y(), _bounds.max().z());

    glVertex3f(_bounds.max().x(), _bounds.max().y(), _bounds.min().z());
    glVertex3f(_bounds.max().x(), _bounds.min().y(), _bounds.min().z());
    glVertex3f(_bounds.max().x(), _bounds.min().y(), _bounds.max().z());
    glVertex3f(_bounds.max().x(), _bounds.max().y(), _bounds.max().z());

    glVertex3f(_bounds.min().x(), _bounds.max().y(), _bounds.min().z());
    glVertex3f(_bounds.max().x(), _bounds.max().y(), _bounds.min().z());
    glVertex3f(_bounds.max().x(), _bounds.max().y(), _bounds.max().z());
    glVertex3f(_bounds.min().x(), _bounds.max().y(), _bounds.max().z());

    glVertex3f(_bounds.min().x(), _bounds.min().y(), _bounds.min().z());
    glVertex3f(_bounds.min().x(), _bounds.max().y(), _bounds.min().z());
    glVertex3f(_bounds.min().x(), _bounds.max().y(), _bounds.max().z());
    glVertex3f(_bounds.min().x(), _bounds.min().y(), _bounds.max().z());

    glVertex3f(_bounds.min().x(), _bounds.min().y(), _bounds.max().z());
    glVertex3f(_bounds.min().x(), _bounds.max().y(), _bounds.max().z());
    glVertex3f(_bounds.max().x(), _bounds.max().y(), _bounds.max().z());
    glVertex3f(_bounds.max().x(), _bounds.min().y(), _bounds.max().z());
  }
  glEnd();
}

inline void drawGrid( const tomo::PrintBounds& _bounds )
{
  // draw grid
  GLfloat tick=1.0;
  {
    // lines X axis
    glLineWidth( 1.0 );
    glBegin(GL_LINES);
    {
      for (int i = 0; tick*i <= _bounds.size().x()/2; i++)
      {
        glColor4f(0.2,0.6,0.2,(i%10)?0.4:0.8);
        glVertex3f(tick*i, _bounds.min().y(), _bounds.min().z());
        glVertex3f(tick*i, _bounds.max().y(), _bounds.min().z());
        glVertex3f(-tick*i, _bounds.min().y(), _bounds.min().z());
        glVertex3f(-tick*i, _bounds.max().y(), _bounds.min().z());
      }
    }
    glEnd();

    // lines Y axis
    glBegin(GL_LINES);
    {
      for (int i = _bounds.min().y(); i <= _bounds.size().y()/2; i++)
      {
        glColor4f(0.6,0.0,0.2,(i%10)?0.4:0.8);
        glVertex3f(_bounds.min().x(), tick*i, _bounds.min().z());
        glVertex3f(_bounds.max().x(), tick*i, _bounds.min().z());
        glVertex3f(_bounds.min().x(), -tick*i, _bounds.min().z());
        glVertex3f(_bounds.max().x(), -tick*i, _bounds.min().z());
      }
    }
    glEnd();
  }
}

template<class POINT, class COLOR> void drawArrow(
  const std::string& _label,
  const POINT& _p1,
  const POINT& _p2,
  const COLOR& _color,
  bool _drawLabel,
  bool _drawCoords,
  GLfloat _width=1.0,
  GLfloat _arrowR=1.0,
  GLfloat _arrowH=2.0 )
{
  glLineWidth(_width);
  glBegin(GL_LINES);
  {
    glColor4fv(_color);
    glVertex3fv(_p1);
    glVertex3fv(_p2);
  }
  glEnd();

  glPushMatrix();
  {
    glTranslatef(COORDS(_p2));
    glRotate(_p2-_p1);
    glColor4fv(_color);
    glTranslatef(0.0,0.0,-_arrowH);
    glutSolidCone(_arrowR, _arrowH, 16.0, 1.0);
  }
  glPopMatrix();

  glPushMatrix();
  {
    glTranslatef(COORDS(_p2));
    glRotatef(90.0,1.0,0.0,0.0);
    glScalef(0.01,0.01,0.01);
    if( _drawLabel )
    {
      BOOST_FOREACH(char ch, _label)
      glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN,ch);
    }
    if( _drawCoords )
    {
      glScalef(0.5,0.5,0.5);
      BOOST_FOREACH(char ch, (std::string)_p2)
      glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN,ch);
    }
  }
  glPopMatrix();

  if( _drawCoords )
  {
    glPushMatrix();
    {
      glTranslatef(COORDS(_p1));
      glRotatef(90.0,1.0,0.0,0.0);
      glScalef(0.01,0.01,0.01);
      glScalef(0.5,0.5,0.5);
      BOOST_FOREACH(char ch, (std::string)_p1)
      glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN,ch);
    }
  }
  glPopMatrix();
}

inline void drawAxis(bool _drawLabel, bool _drawCoords)
{
  glMatrixMode(GL_MODELVIEW);
  glDisable(GL_LIGHTING);
  glDisable(GL_DEPTH_TEST);
  {
    int length = 10.0;
    // draw axis
    GLfloat axisAlpha=0.7;
    {
      drawArrow("x", GLWidget::Point(0.0,0.0,0.0), GLWidget::Point(length,0.0,0.0), GLWidget::Color(1.0,0.0,0.0,axisAlpha), _drawLabel, _drawCoords, 2.0);
      drawArrow("y", GLWidget::Point(0.0,0.0,0.0), GLWidget::Point(0.0,length,0.0), GLWidget::Color(0.0,1.0,0.0,axisAlpha), _drawLabel, _drawCoords, 2.0);
      drawArrow("z", GLWidget::Point(0.0,0.0,0.0), GLWidget::Point(0.0,0.0,length), GLWidget::Color(0.0,0.0,1.0,axisAlpha), _drawLabel, _drawCoords, 2.0);
    }
  }
  glEnable(GL_DEPTH_TEST);
}

template<class LIGHT> void drawLight( LIGHT _light)
{
  glMatrixMode(GL_MODELVIEW);
  // move light
  glLightfv(GL_LIGHT1, GL_POSITION, _light.eye4());
  //glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, _light.center());
  glEnable(GL_LIGHTING);
}


template<class TRACKER, class COLOR> void drawTracker( const std::string& _name, TRACKER _tracker, const COLOR& _color, bool _drawLabel, bool _drawCoords, typename TRACKER::Coord _width=1.0)
{
  glMatrixMode(GL_MODELVIEW);
  glDisable(GL_LIGHTING);
  glDisable(GL_DEPTH_TEST);
  drawArrow(_name, _tracker.eye(), _tracker.center(), _color, _drawLabel, _drawCoords, _width);
  glEnable(GL_DEPTH_TEST);
}

inline void drawObject( tomo::TriangleMesh& mesh )
{
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  {
    tomo::Vec3f c = mesh.bounds().center();
    glTranslatef(-c.x(),-c.y(),0.0);
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
  glMatrixMode(GL_MODELVIEW);
  glDisable(GL_LIGHTING);
  glPointSize(4.0);
  glBegin(GL_POINTS);
  {
    glColor3fv(_color);
    glVertex3fv(_selection);
  }
  glEnd();
}

