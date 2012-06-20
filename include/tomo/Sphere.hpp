#include "tomo/SceneObject.hpp"

namespace tomo 
{
	struct Sphere : public SceneObject, public Primitive 
	{
		Sphere(const Point3f& _center, float _radius)
			: center_(_center), radius_(_radius)
		{
		}

    bool intersect(Ray& _ray, Vec3f* _normal = NULL, TexCoords* _texCoords = NULL) const;
	
    TBD_PROPERTY_REF(Point3f,center);
    TBD_PROPERTY(float,radius);
	};

}
