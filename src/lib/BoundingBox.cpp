#include "tomo/BoundingBox.hpp"

#include <GL/glut.h>

using namespace std;

namespace tomo 
{
  bool BoundingBox::pointInBox(const Point3f p) const
  {
    return p.x() > min.x() && p.x() < max.x() &&
      p.y() > min.y() && p.y() < max.y() &&
      p.z() > min.z() && p.z() < max.z();
  }

  bool BoundingBox::intersect(Ray& ray) const
  {
    float tnear = INF, tfar = -INF;
    for (int i = 0; i < 3; i++)
    {
      if (abs(ray.dir_[i]) < 0.001) continue;
      float minV = min[i], maxV = max[i];
      if (ray.dir_[i] < 0) swap(minV,maxV);

      float tn = (minV - ray.org_[i]) / ray.dir_[i];
      float tf = (maxV - ray.org_[i] ) / ray.dir_[i];

      if (tn < tnear) tnear = tn;
      if (tf > tfar) tfar = tf;
    }

    if (tnear < tfar)
    {
      ray.tMin_ = tnear;
      ray.tMax_ = tfar;
    }
    return (tnear < tfar);
  }

  void BoundingBox::set(const Point3f& _min, const Point3f& _max) 
    { 
      FOREACH_AXIS 
        if (min[axis] > max[axis]) 
          std::swap(min[axis],max[axis]);
      min = _min; 
      max = _max; 
    }


  Axis BoundingBox::dominantAxis() const
  {
    Vec3f d = size(); 
    if (d.x() > d.y())
    { 	if (d.x() > d.z()) return X;
    } else
      if (d.y() > d.z()) return Y;
    return Z;
  }

  void BoundingBox::split(float splitPos, Axis axis, BoundingBox& boxLeft, BoundingBox& boxRight) const
  {
    boxLeft.set(min,max);
    boxRight.set(min,max);
    boxLeft.max[axis] = splitPos;
    boxRight.min[axis] = splitPos;
  }

  void BoundingBox::draw(Color color) const
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

}
