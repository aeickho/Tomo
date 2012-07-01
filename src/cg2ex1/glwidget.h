#ifndef GLWIDGET_H
#define GLWIDGET_H

#include "tomo/Mesh.hpp"
#include "tomo/PointCloud.hpp"
#include "tomo/Camera.hpp"
#include "tomo/Tracker.hpp"
#include "tomo/Light.hpp"
#include "tomo/Color.hpp"
#include "tomo/PolarVec.hpp"
#include <QtOpenGL/QGLWidget>
#include <GL/gl.h>
#include <GL/glu.h>

template<class OBJECT> 
struct DisplayListed : OBJECT 
{
  DisplayListed() : displayList_(0) { }
  void initDL()
  {
    if( !displayList_ )
    {
      displayList_ = glGenLists(1);
      glNewList(displayList_,GL_COMPILE);
      drawObject(*this);
      glEndList();
    }
  }
  void drawDL()
  {
    glCallList(displayList_);
  }
  GLuint displayList_;
};

class GLWidget : public QGLWidget
{
  Q_OBJECT
public:
  typedef tomo::Color<4,float> Color;
  typedef tomo::Point<3,float> Point;
  typedef tomo::Vec<3,float> Vec;
  typedef tomo::PolarVec<float,false> PolarVec;
  typedef tomo::Tracker<Point,PolarVec,false,true> LightTracker; 
  typedef tomo::Light<LightTracker,Color> Light;
  typedef tomo::Tracker<Point,PolarVec,true,true> CameraTracker; 
  typedef tomo::Camera<CameraTracker> Camera;

  struct Config 
  {
    bool drawObjects_;
    bool drawCamera_;
    bool drawLight_;
    bool drawAxis_;
    bool drawLabels_;
    bool drawCoords_;
    bool lockLight_;
  };

  Config config_;

  explicit GLWidget(QWidget *parent = 0);
  enum SelectionMode { SELECT_KNEAREST, SELECT_RADIUS };
protected:
  virtual void mouseMoveEvent(QMouseEvent *event);
  virtual void mousePressEvent(QMouseEvent *event);
  virtual void wheelEvent(QWheelEvent *event);
  virtual void keyPressEvent(QKeyEvent* even);
  virtual void initializeGL();
  virtual void resizeGL(int w, int h);
  virtual void paintGL();
  void paintCamera();
  void paintObjects();
  void paintSelection();
public:
  /// @todo replace with an vector<tomo::SceneObjects>
  DisplayListed<tomo::TriangleMesh> mesh_;
  /// @brief selection parameters
  float pointSize_;
  float radius_;
  int kNearest_;
  SelectionMode selectionMode_;
  Point selection_;

  /// last mouse position in window
  QPoint mousePosition_;
  /// animation timer
  QTimer* timer_;
  /// camera position/view
  Camera camera_;
  /// light parameters
  Light light_;
public slots:
  /// triggered by the animation timer
  virtual void tick();
  virtual void light2cam();
};

#endif // GLWIDGET_H
