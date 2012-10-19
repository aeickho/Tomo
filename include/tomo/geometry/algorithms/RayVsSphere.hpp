namespace tomo
{
  namespace geometry
  {
    namespace prim
    {
      namespace intersect
      {
        struct RayVsSphere : RayIntersector<Sphere>
        {
          bool operator()(primitive_type& _sphere, ray_type& _ray)
          {
            vec_type o = _ray.org() - _prim.center();
            scalar_type a = dot(_ray.dir(),_ray.dir());
            scalar_type b = 2.0f * dot(_ray.dir(),o);
            scalar_type c = dot(o,o) - _sphere.radius()*_sphere.radius();
            scalar_type disc = b*b - 4*a*c;

            if (disc < 0) return false;

            scalar_type discSqrt = sqrtf(disc);
            scalar_type q = (b < 0) ? (-b - discSqrt) : (-b + discSqrt);
            q *= 0.5f;

            scalar_type t0 = q/a, t1 = c/q;
            if (t0 > t1) std::swap(t0,t1);
            if (t1 < 0) return false;

            if (_normal)
            {
              (*_normal)((_ray.intersectionPoint() - _sphere.center()).normalized());
            }
            return _ray.intersection(Primitive3f::pointer(),(t0 < 0) ? t1 : t0,_tNear,_tFar);
          }
        };
      }
    }
  }
}
