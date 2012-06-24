#include "glwidget.h"
#include "helper.h"
#include "tbd/log.h"
#include "tomo/Vector.hpp"
#include <algorithm>
#include <boost/foreach.hpp>
#include <GL/glut.h>
#include "render.hpp"

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
  mesh_.read("cow.off");

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
//glClearColor(1.0, 1.0, 1.0, 1.0);
//glEnable(GL_DEPTH_TEST);
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
    //glEnable(GL_LIGHTING);
  
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

  drawBackground();

  glPointSize(pointSize_);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  realizeCamera(camera_);

  // rotate world coordinate system from XZ to XY
  glRotatef(-90.0,1,0,0);
  glRotatef(-180.0,0,0,1);

  drawBed();
  drawLight(light_);
  drawObject(mesh_);
  drawTracker("light",light_, Color(1.0,1.0,1.0,0.8));
  drawTracker("cam",camera_, Color(0.0,0.0,0.0,0.8));
  drawAxis();
  drawSelection(selection_,Color(1.0,0.0,1.0));
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
  camera_.track( 0, 0, (double)event->delta()/100.0 );
  update();
}

