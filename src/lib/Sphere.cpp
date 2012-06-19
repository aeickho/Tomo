#include "tomo/Sphere.hpp"

#include <GL/glut.h>
#include <cmath>

using namespace std; 

namespace tomo 
{
/*	void Sphere::draw(Color color) const
	{
		glPushMatrix();
		glTranslatef(center_[0], center_[1], center_[2]);
		glutSolidSphere(radius_, 32, 32);
		glPopMatrix();
	}
*/
	bool Sphere::intersect(Ray& ray) const
	{
		Vec3f o = ray.org_ - center_;
		float a = ray.dir_ * ray.dir_;
		float b = 2.0f * (ray.dir_ * o);
		float c = o*o - radius()*radius();
		float disc = b*b - 4*a*c;

		if (disc < 0) return false;

		float discSqrt = sqrtf(disc);
		float q = (b < 0) ? (-b - discSqrt) : (-b + discSqrt);
		q *= 0.5f;

		float t0 = q/a, t1 = c/q;
		if (t0 > t1) std::swap(t0,t1);
		if (t1 < 0) return false;

    return ray.intersection(pointer(),(t0 < 0) ? t1 : t0,0,0);
	}

	Vec3f Sphere::normal(const Ray& ray) const
	{
    Vec3f n = ray.intersectionPoint() - center_;
    return n.normalized();    
	}

	TexCoords Sphere::texCoords(const Ray& ray) const
	{
		// from http://www.cse.msu.edu/~cse872/tutorial4.html
		Vec3f n = normal(ray);
		float u = atan2(n.x(),n.z()) / (2.0*M_PI) + 0.5;
		float v = acosf(n.y())/M_PI;

		return TexCoords(u,v);
	}


}
