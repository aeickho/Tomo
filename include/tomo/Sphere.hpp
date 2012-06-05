#include "tomo/SceneObject.hpp"

namespace tomo 
{
	struct Sphere : public SceneObject 
	{
		Sphere(const Point3f& _center, float _radius)
			: center_(_center), radius_(_radius)
		{
		}

		bool intersect(Ray& ray) const;

		Vec3f normal(const Point3f& iPoint) const;
		TexCoords texCoords(const Point3f& iPoint) const;
		
        void draw();

        TBD_DECLARE_PROPERTY_REF(Point3f,center);
        TBD_DECLARE_PROPERTY(float,radius);
	};

}
