#include "tomo/Primitive.hpp"

namespace tomo 
{
	struct Sphere : public Primitive3f
	{
		Sphere(const Point3f& _center, float _radius)
			: center_(_center), radius_(_radius)
		{}

    bool intersect(Ray3f& _ray, float& _tNear, float &_tFar, Vec3f* _normal = NULL) const;
	
    Bounds3f bounds() const;

    TBD_PROPERTY(Point3f,center);
    TBD_PROPERTY(float,radius);
	};

}
