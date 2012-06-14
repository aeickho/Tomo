#include "tomo/SceneObject.hpp"

namespace tomo 
{
	struct Sphere : public SceneObject, public Primitive 
	{
		Sphere(const Point3f& _center, float _radius)
			: center_(_center), radius_(_radius)
		{
		}

		bool intersect(Ray& ray) const;

		Vec3f normal(const Ray& ray) const;
		TexCoords texCoords(const Ray& ray) const;
		
    void draw(Color color=Color()) const;

    TBD_PROPERTY_REF(Point3f,center);
    TBD_PROPERTY(float,radius);
	};

}
