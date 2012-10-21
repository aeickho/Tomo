#ifndef GLWIDGET_H
#define GLWIDGET_H

#include "tomo/geometry/comp/Mesh.hpp"
#include "tomo/scene/Camera.hpp"
#include "tomo/scene/Tracker.hpp"
#include "tomo/scene/Light.hpp"
#include "tomo/geometry/base/Color.hpp"
#include "tomo/geometry/base/PolarVec.hpp"
#include "tomo/scene/PrintBounds.hpp"
#include "tomo/slicing/Slice.hpp"
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
      draw(*this);
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
  typedef tomo::geometry::base::Color4f Color;
  typedef tomo::geometry::base::Point3f Point;
  typedef tomo::geometry::base::Vec3f Vec;
  typedef tomo::geometry::base::PolarVec<float,false> PolarVec;
  typedef tomo::scene::Tracker<float,false,true> LightTracker; 
  typedef tomo::scene::Light<LightTracker,Color> Light;
  typedef tomo::scene::Tracker<float,true,true> CameraTracker; 
  typedef tomo::scene::Camera<CameraTracker> Camera;
  typedef tomo::scene::PrintBounds PrintBounds;

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
  DisplayListed<tomo::geometry::comp::Mesh> mesh_;

  /// @todo Obsolete since tomo::Slices does not have a default constructor anymore
  /// DisplayListed<tomo::Slices> slices_;


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
  PrintBounds printRange_;
public slots:
  /// triggered by the animation timer
  virtual void tick();
  virtual void light2cam();
};

#endif // GLWIDGET_H
