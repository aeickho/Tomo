#include "glwidget.h"
#include "helper.h"
#include "tbd/log.h"
#include "tomo/Vector.hpp"
#include <algorithm>
#include <boost/foreach.hpp>
#include <GL/glut.h>

LOG_INIT;

GLWidget::GLWidget(QWidget *parent) :
  QGLWidget(
    QGLFormat(
      QGL::DoubleBuffer |
      QGL::DepthBuffer |
      QGL::Rgba |
      QGL::AlphaChannel |
      QGL::DirectRendering),
    parent),
  mousePosition_(0,0)
{
  pointSize_ = 2.0;
  kNearest_ = 10;
  radius_ = 2.0;
  selectionMode_ = SELECT_RADIUS;
}
void GLWidget::initializeGL()
{
 mesh_.read("yoda.off");

  LOG_MSG << fmt("mesh_.bounds().radius() = %") % mesh_.bounds().radius();

  // setup camera
  camera_ = Camera(
    Tracker(
      // target to track (origin)
      Point(0.0,0.0,0.0),
      // set tracking position from spheric coordinates
      PolarVec(-45.0, 45.0, mesh_.bounds().radius() * 1.5) 
      ),
    // near, far
    1.0, 1000.0,
    Point(0.0, 1.0, 0.0)
  );

  // setup light source
  light_ = Light(
    Tracker(
      // target to track (origin)
      Point(0.0,0.0,0.0),
      // set tracking position from spheric coordinates
      PolarVec(-45.0, 45.0, mesh_.bounds().radius() * 10) 
      ),
    // ambient color
    Color(0.1, 0.1, 0.1, 1.0),
    // diffuse color
    Color(1.0, 1.0, 1.0, 1.0),
    // specular color,
    Color(0.6, 0.6, 0.6, 1.0),
    // intensity
    1.0,
    // shadows
    0.0,
    // radius
    1.0
  );

  // Set up the rendering context, define display lists etc.:
  glClearColor(1.0, 1.0, 1.0, 1.0);
  glEnable(GL_DEPTH_TEST);
  //glDepthFunc(GL_LEQUAL);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glEnable(GL_LINE_SMOOTH);
  glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

  glEnable(GL_POINT_SMOOTH);
  glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

  glEnable(GL_CULL_FACE);

  // set light
  {
    glEnable(GL_LIGHTING);
  
    // light and material
    glEnable(GL_COLOR_MATERIAL);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_.diffuse());
    glLightfv(GL_LIGHT1, GL_POSITION, light_.eye());
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_.ambient());
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHT1);
  }
  glEnable(GL_NORMALIZE);

  // fix outlines z-fighting withthe quads
  glPolygonOffset(1, 1);
  glEnable(GL_POLYGON_OFFSET_FILL);

  setAutoBufferSwap(true);

  int fps = 25;
  timer_ = new QTimer(this);
  timer_->setSingleShot(false);
  connect(timer_, SIGNAL(timeout()), this, SLOT(tick()));
  timer_->start(1000/fps);
}
void GLWidget::tick()
{
  /*  static float t = 0;
      t+=2.1;
      camera_.longitude(camera_.longitude()+1);
      camera_.latitude(camera_.latitude()+1);
      camera_.distance(1.5 * mesh_.boundingBox_.size().length() + mesh_.boundingBox_.size().length() * sin(t/100.0));
      update();
      */
}
void GLWidget::resizeGL(int w, int h)
{
  w = w & ~1;
  h = h & ~1;
  // setup viewport, projection etc.:
  glViewport(0, 0, (GLint)w, (GLint)h);
  // reshaped window aspect ratio
  float aspect = (float) w / (float) h;
  // restore view definition after window reshape
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  // perspective projection
  gluPerspective(60.0, aspect, 1.0/*camera_.near()*/, camera_.far());

  glClearColor(0.0,0.0,0.0,1.0);
  glMatrixMode(GL_MODELVIEW);
}
GLWidget::Point unProject(QPoint const & pos)
{
  GLdouble projection[16];
  glGetDoublev(GL_PROJECTION_MATRIX, projection);

  GLdouble modelView[16];
  glGetDoublev(GL_MODELVIEW_MATRIX, modelView);

  GLint viewport[4];
  glGetIntegerv(GL_VIEWPORT, viewport);

  double winX = pos.x();
  double winY = viewport[3] - pos.y();

  GLfloat winZ;
  glReadPixels(winX, winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

  GLdouble x,y,z;
  gluUnProject(winX, winY, winZ, modelView, projection, viewport, &x, &y, &z);
  return GLWidget::Point(x,y,z);
}
void GLWidget::paintGL()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


  // draw background
  {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glBegin(GL_QUADS);
    
    glColor3f(0.0,0.0,0.0);
    glVertex2f(-1.0,-1.0);
    glVertex2f(1.0,-1.0);
    
    glColor3f(54.0/255.0,110.0/255.0,201.0/255.0);
    glVertex2f(1.0, 1.0);
    glVertex2f(-1.0, 1.0);
    glEnd();

    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);    
  }

  glPointSize(pointSize_);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // realize camera
  {
    // LOG_MSG << fmt("eye    = %,%,%") % camera_.eye().x() % camera_.eye().y() % camera_.eye().z();
    // LOG_MSG << fmt("center = %,%,%") % camera_.center().x() % camera_.center().y() % camera_.center().z();

    // realize coordinates
    gluLookAt(
      camera_.eye().x(),
      camera_.eye().y(),
      camera_.eye().z(),
      camera_.center().x(),
      camera_.center().y(),
      camera_.center().z(),
      camera_.up().x(),
      camera_.up().y(),
      camera_.up().z()
    );
  }

  // rotate world coordinate system from XZ to XY
  glRotatef(-90.0,1,0,0);
  glRotatef(-180.0,0,0,1);
 
  LOG_MSG << fmt("Light %,%,%,%") % light_.eye().x() % light_.eye().y() % light_.eye().z() % light_.eye().w();
 
  glLightfv(GL_LIGHT1, GL_POSITION, light_.eye());

  // draw bed 
  GLfloat bedXsize = 210;
  GLfloat bedYsize = 210;
  GLfloat bedXmin = -bedXsize/2;
  GLfloat bedXmax = bedXsize/2;
  GLfloat bedYmin = -bedYsize/2;
  GLfloat bedYmax = bedYsize/2;
  GLfloat bedZ = -mesh_.bounds().size().z()/2.0; 
  GLfloat bedZmax = bedZ + 210;
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

  // draw axis
  GLfloat axisAlpha=0.7;
  {
    glLineWidth(3.0);
    glBegin(GL_LINES);
    {
      glColor4f(1.0,0.0,0.0,axisAlpha);
      glVertex3f(0.0,0.0,0.0);
      glVertex3f(bedXmax,0.0,0.0);

      glColor4f(0.0,1.0,0.0,axisAlpha);
      glVertex3f(0.0,0.0,0.0);
      glVertex3f(0.0,bedYmax,0.0);

      glColor4f(0.0,0.0,1.0,axisAlpha);
      glVertex3f(0.0,0.0,0.0);
      glVertex3f(0.0,0.0,bedZmax);
    }
    glEnd();

    glPushMatrix();
    {
      glTranslatef(0.0, 0.0, bedZmax); 
      glColor4f(0.0, 0.0, 1.0,axisAlpha);
      glutSolidCone(1.0, 2.0, 16.0, 1.0); 
      glTranslatef(0.0, 0.0, -bedZmax);
      glTranslatef(bedXmax, 0.0, 0.0); 
      glRotatef(90.0, 0.0, 1.0, 0.0);
      glColor4f(1.0, 0.0, 0.0, axisAlpha);
      glutSolidCone(1.0, 2.0, 16.0, 1.0); 
      glRotatef(-90.0, 0.0, 1.0, 0.0);
      glTranslatef(-bedXmax, 0.0, 0.0); 
      glTranslatef(0.0, bedYmax, 0.0); 
      glRotatef(-90.0, 1.0, 0.0, 0.0);
      glColor4f(0.0,1.0,0.0,axisAlpha);
      glutSolidCone(1.0,2.0,16.0,1.0); 
    }
    glPopMatrix();
  }


  // draw grid
  {
    glDisable(GL_LIGHTING);
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
    glEnable(GL_LIGHTING);
  }

  // draw axis label
  {
    glPushMatrix();
    glColor4f(1.0, 0.0, 0.0,axisAlpha);
    glLineWidth( 3.0 );
    glTranslatef(bedXmax,0.0,3.0);
    glRotatef(90.0,1.0,0.0,0.0);
    glScalef(0.05,0.05,0.05);
    glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN,'x');
    glPopMatrix();

    glPushMatrix();
    glColor4f(0.0, 1.0, 0.0,axisAlpha);
    glLineWidth( 3.0 );
    glTranslatef(0.0,bedYmax,3.0);
    glRotatef(90.0,1.0,0.0,0.0);
    glScalef(0.05,0.05,0.05);
    glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN,'y');
    glPopMatrix();

    glPushMatrix();
    glColor4f(0.0, 0.0, 1.0,axisAlpha);
    glLineWidth( 3.0 );
    glTranslatef(3.0,0.0,bedZmax);
    glRotatef(90.0,1.0,0.0,0.0);
    glScalef(0.05,0.05,0.05);
    glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN,'z');
    glPopMatrix();
  }


  // draw objects
  {
    tomo::Vec3f c = 0.5*(mesh_.bounds().max().vec() + mesh_.bounds().min().vec());
    glTranslatef(-c.x(),-c.y(),-c.z());
    glColor3f(0.8,0.8,0.8);
    glBegin(GL_TRIANGLES);
    BOOST_FOREACH( const tomo::Triangle& tri, mesh_.triangles() )
    {
      glNormal3f(COORDS(tri.normal()));
      for (int i  = 0; i < 3; i++)
        glVertex3fv(tri.v[i].p());
    }
    glEnd();
  }
  
  // draw selection
  {
    glPointSize(pointSize_*4.0);
    glBegin(GL_POINTS);
    {
      glColor3f(1.0,0.0,0.0);
      glVertex3f(selection_.x(),selection_.y(),selection_.z());
    }
    glEnd();
  }
}
void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
  if (0 != (event->buttons() & Qt::LeftButton))
  {
    if( event->modifiers() == Qt::ControlModifier )
      light_.track( event->pos().x() - mousePosition_.x(), event->pos().y() - mousePosition_.y(), 0 );
    else
      camera_.track( event->pos().x() - mousePosition_.x(), event->pos().y() - mousePosition_.y(), 0 );
    update();
    mousePosition_ = event->pos();
  }
}
void GLWidget::mousePressEvent(QMouseEvent *event)
{
  mousePosition_ = event->pos();
  switch (event->button())
  {
  case Qt::LeftButton:
    selection_ = unProject(event->pos());
    update();
    break;
  default:
    ;
  }
}
void GLWidget::wheelEvent(QWheelEvent* event)
{
  if( event->modifiers() == Qt::ControlModifier )
   camera_.track( 0, 0, (double)event->delta()/100.0 );
  else
   light_.track( 0, 0, (double)event->delta()/100.0 );
  update();
}

