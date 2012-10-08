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
      template<int DIMENSIONS, typename SCALAR = base::DEFAULT_TYPE>
      struct Vertex : public Primitive<DIMENSIONS,SCALAR>
      {
        typedef SCALAR scalar_type;
        typedef Primitive<DIMENSIONS,scalar_type> primitive_type;
        typedef typename primitive_type::vec_type vec_type;
        typedef typename primitive_type::point_type point_type;
        typedef typename primitive_type::ray_type ray_type;
        typedef typename primitive_type::bounds_type bounds_type;

        Vertex(point_type _v = point_type(), vec_type _n = vec_type()) 
          : v_(_v), n_(_n)
        {}

        void set(point_type _v, vec_type _n = vec_type())
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

      typedef Vertex<2,float> Vertex2f;
      typedef Vertex<3,float> Vertex3f;
      typedef Vertex<2,unsigned short> Vertex2us;
    }
  }
}
