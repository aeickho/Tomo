#include "tomo/Ray.hpp"
#include "tomo/Primitive.hpp"

#include <GL/glut.h>


namespace tomo
{
/*
  void Ray::drawIntersectionPoint() 
  {
    if (!obj) return;
    if (!obj->shader()) return;

    Vec3f color = obj->shader->shade(*this);
    Vec3f iPoint = getIntersectionPoint();

    glDisable(GL_LIGHTING);
    //	glColor3f(color.x,color.y,color.z);

    glBegin(GL_POINTS);
    glVertex3f(iPoint.x,iPoint.y,iPoint.z);
    glEnd();

    glEnable(GL_DEPTH_TEST);
  }*/


  Ray Ray::reflect()
  {
/*
    Vec3f iPoint = getIntersectionPoint(); 
    Vec3f dirN = dir.normalized();
    Vec3f refl = -2*(normal * dirN)*normal+dirN;

    Ray reflRay(iPoint,refl.normalized(),scrPosX,scrPosY);
    reflRay.bounce = bounce + 1;
    return reflRay;
*/
    return Ray();
  }

  Ray Ray::refract(float index)
  {
/*
    tmax += 0.01;
    Vec3f iPoint = getIntersectionPoint();
    Vec3f dirN = dir.normalized();
    float k = 1.0 - index * index * (1.0 - (dirN*normal) * (dirN*normal));
    //	if (k < 0.0) return Ray(); 
    Vec3f refrDir = index * dirN - (index * (dirN*normal) + sqrt(k)) * normal;

    Ray refrRay(iPoint,refrDir.normalized(),scrPosX,scrPosY);
    refrRay.bounce = bounce + 1;
    refrRay.obj = obj;
    return refrRay;
*/
      return Ray();
    }


}
