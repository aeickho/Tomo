#ifndef GLWIDGET_H
#define GLWIDGET_H

#include "tomo/PointCloud.hpp"
#include "tomo/Camera.hpp"
#include "tomo/Light.hpp"
#include "tomo/Color.hpp"
#include <QtOpenGL/QGLWidget>
#include <GL/gl.h>

class GLWidget : public QGLWidget
{
  Q_OBJECT
public:
  typedef tomo::SphericalCamera<4,float> Camera;
  typedef tomo::Color<4,float> Color;
  typedef tomo::Light<Color,GLfloat,4> Light;
  typedef tomo::Point<4,float> Point;

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
  tomo::Mesh mesh_;
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
