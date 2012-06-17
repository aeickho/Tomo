#ifndef GLWIDGET_H
#define GLWIDGET_H

#include "tomo/PointCloud.hpp"
#include "tomo/Camera.hpp"
#include <QtOpenGL/QGLWidget>
#include <boost/assert.hpp>
#include <GL/gl.h>
#include <algorithm>

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
    tomo::Mesh pointCloud_;
    float pointSize_;
    float radius_;
    int kNearest_;
    SelectionMode selectionMode_;
    tomo::Point3f selection_;
    // last mouse position in window
    QPoint mousePosition_;
    QTimer* timer_;
    /// camera position/view
    tomo::Camera<GLdouble> camera_;

    public slots:
      virtual void tick();
};

#endif // GLWIDGET_H
