#include <tomo/geometry/prim/LineSegment.hpp>

namespace tomo
{
  namespace geometry
  {
    namespace prim
    {
      namespace intersect
      {
        struct RayVsBoundingBox : RayIntersector<BoundingBox>
        {
          /// @todo : That function is that ugly, make it template based and put it in Bounds class
          bool operator()(primitive_type& _boundingBox, ray_type& _ray)
          {
            scalar_type tmin, tmax, tymin, tymax, tzmin, tzmax;

            scalar_type minV = _boundingBox.bounds().min().x(),
                        maxV = _boundingBox.bounds().max().x();
            if (_ray.dir().x() < 0) swap(minV,maxV);

            tmin = (minV - _ray.org().x()) / _ray.dir().x();
            tmax = (maxV - _ray.org().x()) / _ray.dir().x();

            minV = _boundingBox.bounds().min().y();
            maxV = _boundingBox.bounds().max().y();

            if (_ray.dir().y() < 0) swap(minV,maxV);

            tymin = (minV - _ray.org().y()) / _ray.dir().y();
            tymax = (maxV - _ray.org().y()) / _ray.dir().y();

            if ( (tmin > tymax) || (tymin > tmax) )  return false;

            if (tymin > tmin) tmin = tymin;
            if (tymax < tmax) tmax = tymax;

            minV = _boundingBox.bounds().min().z();
            maxV = _boundingBox.bounds().max().z();

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
        };
      }
    }
  }
}
