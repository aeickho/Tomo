#ifndef GLWIDGET_H
#define GLWIDGET_H

#include "tomo/PointCloud.hpp"
#include "tomo/Camera.hpp"
#include <QtOpenGL/QGLWidget>
#include <GL/gl.h>

class GLWidget : public QGLWidget
{
  Q_OBJECT
  public:
    explicit GLWidget(QWidget *parent = 0);
    typedef enum { SELECT_KNEAREST, SELECT_RADIUS } SelectionMode;
  protected:
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void wheelEvent(QWheelEvent *event);
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();
  public:
    /// @todo replace with a scene
    tomo::Mesh pointCloud_;
    /// selection parameters
    float pointSize_;
    float radius_;
    int kNearest_;
    SelectionMode selectionMode_;
    tomo::Point3f selection_;

    /// last mouse position in window
    QPoint mousePosition_;
    /// animation timer
    QTimer* timer_;
    /// camera position/view
    /// @todo move into scene
    tomo::Camera<GLdouble> camera_;
    /// perspective
    public slots:
      /// triggered by the animation timer
      virtual void tick();
};

#endif // GLWIDGET_H
