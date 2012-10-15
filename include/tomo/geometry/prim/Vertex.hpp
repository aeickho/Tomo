#pragma once

#include "tomo/geometry/prim/Primitive.hpp"

#include <vector>
#include <set>

namespace tomo
{
  namespace geometry
  {
    namespace prim
    {
      template<class MODEL>
      struct Vertex : public Primitive<MODEL>
      {
        TOMO_PRIMITIVE_TYPES(Primitive<MODEL>);

        Vertex(const point_type& _v = point_type(), const vec_type& _n = vec_type()) 
          : v_(_v), n_(_n)
        {}

        void set(const point_type& _v, const vec_type& _n = vec_type())
        {
          v_ = _v;
          n_ = _n;
        }

        bool intersect(ray_type& _ray, scalar_type& _tNear, scalar_type& _tFar, vec_type* _normal = NULL) const
        {
          return false;
        }

        SplitPlaneIntersect intersect(base::Axis _axis, scalar_type _splitPos,
                                      const bounds_type& _boundsLeft, const bounds_type& _boundsRight) const
        {
          return SplitPlaneIntersect(v_[_axis] <= _splitPos,v_[_axis] >= _splitPos); 
        }

        bounds_type bounds() const
        {
          return bounds_type(v_,v_);
        }
        point_type center() const
        {
          return v_;
        }
        
        bool hasNormal()
        {
          return (n_.sqrLength() != 0.0f);
        }

        TBD_PROPERTY_REF(point_type,v);
        TBD_PROPERTY_REF(vec_type,n);
      };

      typedef Vertex<Model2f> Vertex2f;
      typedef Vertex<Model3f> Vertex3f;
      typedef Vertex<Model2us> Vertex2us;
    }
  }
}
