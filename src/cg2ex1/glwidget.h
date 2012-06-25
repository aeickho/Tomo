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

class GLWidget : public QGLWidget
{
  Q_OBJECT
public:
  typedef tomo::Color<4,float> Color;
  typedef tomo::Point<4,float> Point;
  typedef tomo::Vec<4,float> Vec;
  typedef tomo::PolarVec<float> PolarVec;
  typedef tomo::Tracker<Point,PolarVec,true> LightTracker; 
  typedef tomo::Light<LightTracker,Color> Light;
  typedef tomo::Tracker<Point,PolarVec> CameraTracker; 
  typedef tomo::Camera<CameraTracker> Camera;

  explicit GLWidget(QWidget *parent = 0);
  enum SelectionMode { SELECT_KNEAREST, SELECT_RADIUS };
protected:
  virtual void mouseMoveEvent(QMouseEvent *event);
  virtual void mousePressEvent(QMouseEvent *event);
  virtual void wheelEvent(QWheelEvent *event);
  virtual void initializeGL();
  virtual void resizeGL(int w, int h);
  virtual void paintGL();
  void paintCamera();
  void paintObjects();
  void paintSelection();
public:
  /// @todo replace with an vector<tomo::SceneObjects>
  tomo::TriangleMesh mesh_;
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
  /// perspective
public slots:
  /// triggered by the animation timer
  virtual void tick();
};

#endif // GLWIDGET_H
