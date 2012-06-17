#include "glwidget.h"
#include "helper.h"
#include "tbd/log.h"
#include "tomo/Vector.hpp"
#include <GL/glu.h>

LOG_INIT;

using namespace tomo;



GLWidget::GLWidget(QWidget *parent)
        : QGLWidget(QGLFormat(QGL::DoubleBuffer | QGL::DepthBuffer | QGL::Rgba | QGL::AlphaChannel | QGL::DirectRendering), parent)
{
  oldX_ = 0;
  oldY_ = 0;
  leftButton_ = false;
  angle_ = 0.0;
  pointSize_ = 2.0;
  kNearest_ = 10;
  radius_ = 2.0;
  selectionMode_ = SELECT_RADIUS;
}
void GLWidget::initializeGL()
{
  pointCloud_.read("cow.off");

  camera_.latitude(35.0);

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

  glEnable(GL_LIGHTING);
  // calculate normals clockwise
  glFrontFace(GL_CW);

  // light and material
  glEnable(GL_COLOR_MATERIAL);
//  GLfloat mat_ambient[] = {0.5, 0.5, 0.5, 1.0};
//  GLfloat mat_specular[] = {0.6, 0.6, 0.6, 1.0};
//  GLfloat mat_shininess[] = { 3.0 };
  GLfloat LightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f };
  GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };
  GLfloat model_ambient[] = { 0.3, 0.3, 0.3 };
  GLfloat light_position[] = { 0.0, 100.0, 0.0, 1.0 };
  /* glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);*/
  glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
  glLightfv(GL_LIGHT1, GL_POSITION, light_position);
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, model_ambient);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_LIGHT1);
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
  static float t = 0;
  t+=2.1;
  camera_.longitude(camera_.longitude()+1);
  camera_.latitude(camera_.latitude()+1);
  camera_.distance(1.5 * pointCloud_.boundingBox_.size().length() + pointCloud_.boundingBox_.size().length() * sin(t/100.0));
  update();
}
void GLWidget::resizeGL(int w, int h)
{
  w = w & ~1; h = h & ~1;
  // setup viewport, projection etc.:
  glViewport(0, 0, (GLint)w, (GLint)h);

	// reshaped window aspect ratio
	float aspect = (float) w / (float) h;

  // restore view definition after window reshape
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
  // perspective projection
	gluPerspective(60.0, aspect, 1.0, 100.0);
  
  // clear background and depth buffer
	glClearColor(0.0,0.0,0.0,1.0);
	glMatrixMode(GL_MODELVIEW);
}
Point3f unProject(QPoint const & pos)
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
  return Point3f(x,y,z);
}
void GLWidget::paintGL()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glPointSize(pointSize_);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  project(camera_);

  Vec3f c = 0.5*(pointCloud_.boundingBox_.max.vec() + pointCloud_.boundingBox_.min.vec());
  glTranslatef(-c.x(),-c.y(),-c.z());
  pointCloud_.draw(Color(0.8,0.8,0.8));

  glPointSize(pointSize_*4.0);
  
  glBegin(GL_POINTS);
  {
    glColor3f(1.0,0.0,0.0);
    glVertex3f(selection_.x(),selection_.y(),selection_.z());
  }
  glEnd();
}
void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
  if (event->buttons() != Qt::NoButton)
  {
    angle_ += event->x() - oldX_;
    //int motionY = event->y() - old_y;
    //mouseMotion(motionX, motionY);
    //
    resizeGL(this->width(),this->height());
    paintGL();

    oldX_ = event->x();
    oldY_ = event->y();
 }
}
void GLWidget::mousePressEvent(QMouseEvent *event)
{
  if (event->button() != Qt::NoButton)
  {
    oldX_ = event->x(); oldY_ = event->y();
    leftButton_ = !leftButton_;
    selection_ = unProject(event->pos());
    update();
  }
}


