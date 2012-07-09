#include "tomo/BoundingBox.hpp"

using namespace std;

namespace tomo 
{
  bool BoundingBox::pointInBox(const Point3f p) const
  {
    return p.x() > min().x() && p.x() < max().x() &&
      p.y() > min().y() && p.y() < max().y() &&
      p.z() > min().z() && p.z() < max().z();
  }

  bool BoundingBox::intersect(Ray& _ray, float& _tNear, float &_tFar, Vec3f* _normal, Point2f* _texCoords) const
  {
    float tmin, tmax, tymin, tymax, tzmin, tzmax;

    float minV = min().x(), maxV = max().x();
    if (_ray.dir().x() < 0) swap(minV,maxV);

    tmin = (minV - _ray.org().x()) / _ray.dir().x();
    tmax = (maxV - _ray.org().x()) / _ray.dir().x();

    minV = min().y(), maxV = max().y();
    if (_ray.dir().y() < 0) swap(minV,maxV);

    tymin = (minV - _ray.org().y()) / _ray.dir().y();
    tymax = (maxV - _ray.org().y()) / _ray.dir().y();

    if ( (tmin > tymax) || (tymin > tmax) )  return false;

    if (tymin > tmin) tmin = tymin;
    if (tymax < tmax) tmax = tymax;

    minV = min().z(), maxV = max().z();
    if (_ray.dir().z() < 0) swap(minV,maxV);

    tzmin = (minV - _ray.org().z()) / _ray.dir().z();
    tzmax = (maxV - _ray.org().z()) / _ray.dir().z();

    if ( (tmin > tzmax) || (tzmin > tmax) ) return false;

    if (tzmin > tmin) tmin = tzmin;
    if (tzmax < tmax) tmax = tzmax;


    if ((tmin <= _ray.tNear()) && (tmax >= _ray.tFar()))
    {
      _tNear = (tmin > _ray.tNear()) ? tmin : _ray.tNear();
      _tFar = (tmax < _ray.tFar()) ? tmax : _ray.tFar();
      return true;
    }
    return false;  
  }

/*  void BoundingBox::draw(Color color) const
  {
    float x  = min.x(), y  = min.y(), z  = min.z();
    float xs = max.x(), ys = max.y(), zs = max.z();

    glColor3f(color.x(),color.y(),color.z());

    glBegin(GL_LINE_LOOP);
    // top side
    glVertex3f(x , ys, z );
    glVertex3f(xs, ys, z );
    glVertex3f(xs, ys, zs);
    glVertex3f(x , ys, zs);
    glEnd();

    glBegin(GL_LINE_LOOP);
    // bottom side
    glVertex3f(x , y, z );
    glVertex3f(xs, y, z );
    glVertex3f(xs, y, zs);
    glVertex3f(x , y, zs);
    glEnd();

    glBegin(GL_LINE_LOOP);
    // east side
    glVertex3f(x , y , z);
    glVertex3f(xs, y , z);
    glVertex3f(xs, ys, z);
    glVertex3f(x , ys, z);
    glEnd();

    glBegin(GL_LINE_LOOP);
    // west side
    glVertex3f(x , y ,zs);
    glVertex3f(xs, y ,zs);
    glVertex3f(xs, ys,zs);
    glVertex3f(x , ys,zs);
    glEnd();

    glBegin(GL_LINE_LOOP);
    // north side
    glVertex3f(x     , y     , z     );
    glVertex3f(x     , y     , zs);
    glVertex3f(x     , ys, zs);
    glVertex3f(x     , ys, z     );
    glEnd();

    glBegin(GL_LINE_LOOP);
    // south side
    glVertex3f( xs, y     , z     );
    glVertex3f( xs, y     , zs);
    glVertex3f( xs, ys, zs);
    glVertex3f( xs, ys, z     );
    glEnd();    
  }
*/
}
