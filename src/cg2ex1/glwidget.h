#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QtOpenGL/QGLWidget>

#include "tomo/PointCloud.hpp"

class GLWidget : public QGLWidget
{
  Q_OBJECT
public:
  explicit GLWidget(QWidget *parent = 0);

  tomo::Mesh pointCloud_;

  void mouseMoveEvent(QMouseEvent *event);
  void mousePressEvent(QMouseEvent *event);

  typedef enum { SELECT_KNEAREST, SELECT_RADIUS } SelectionMode;

  float pointSize_;
  float radius_;
  int kNearest_;
  SelectionMode selectionMode_;

  tomo::Point3f selection_;

protected:
  virtual void initializeGL();
  virtual void resizeGL(int w, int h);
  virtual void paintGL();

private:
  // some stateholders for mouse motion
  // last mouse position in window
  int oldX_, oldY_;
  // is left mouse button pressed
  bool leftButton_;
  float angle_;
  QTimer* timer_;
signals:
public slots:
  virtual void tick();
};

#endif // GLWIDGET_H
