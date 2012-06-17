#ifndef GLWIDGET_H
#define GLWIDGET_H

#include "tomo/PointCloud.hpp"
#include <QtOpenGL/QGLWidget>
#include <boost/assert.hpp>
#include <GL/gl.h>

template<class COORD> struct Camera
{
  typedef COORD Coord;
  typedef Coord Angle;
  typedef tomo::Point<3,Coord> Point;

  bool valid() const { return valid_; }

  const Point& center() const { return center_; }
  void center( const Point& _center ) { center_ = _center; valid_ = false; }

  Angle latitude() const { return latitude_; }
  void latitude( Angle _latitude ) { latitude_ = _latitude; valid_ = false; }

  Angle longitude() const { return longitude_; }
  void longitude( Angle _longitude ) { longitude_ = _longitude; valid_ = false; }

  Coord distance() const { return distance_; }
  void distance( Coord _distance ) { distance_ = _distance; valid_ = false; }

  Camera() : valid_(false), center_(0,0,0), latitude_(0), longitude_(0), distance_(0) {} 

  friend void project( Camera& _camera )
  {
    // update coordinates
    _camera.update();
    // realize coordinates
    gluLookAt(
        _camera.eye_.x(),_camera.eye_.y(),_camera.eye_.z(),
        _camera.center_.x(),_camera.center_.y(),_camera.center_.z(),
        _camera.up_.x(),_camera.up_.y(),_camera.up_.z() 
        );
  }

  void update()
  {
    if( !valid_ ) 
    {
      /// @todo optimize: move the division to getter/setter of meridian and latiude?!
      eye_.x( center_.x() + distance_ * cos( latitude_ / 100.0 ) * cos( longitude_ / 100.0 ) );
      eye_.y( center_.y() + distance_ * sin( latitude_ / 100.0 ) );
      eye_.z( center_.z() + distance_ * cos( latitude_ / 100.0 ) * sin( longitude_ / 100.0 ) ); 
      up_.x(0);
      up_.y(1);
      up_.z(0);
      valid_ = true;
    }
  }

  protected:
  /// calculations are done
  bool valid_;
  /// point of interest
  Point center_;
   /// latitude of the camera postition on a sphere arround center
  Angle latitude_;
  /// longitude/meridian of the camera position on a sphere arround center
  Angle longitude_;
  /// distance of the camera from center
  Coord distance_;
  /// camera position
  Point eye_;
  /// camera orientation
  Point up_;
};

class GLWidget : public QGLWidget
{
  Q_OBJECT
  public:
    explicit GLWidget(QWidget *parent = 0);
    typedef enum { SELECT_KNEAREST, SELECT_RADIUS } SelectionMode;
  protected:
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
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
    // some stateholders for mouse motion
    // last mouse position in window
    int oldX_, oldY_;
    // is left mouse button pressed
    bool leftButton_;
    float angle_;
    QTimer* timer_;
    /// camera position/view
    Camera<GLdouble> camera_;

signals:
    public slots:
      virtual void tick();
};

#endif // GLWIDGET_H
