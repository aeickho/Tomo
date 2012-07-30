#pragma once

#include "tomo/Primitive.hpp"

#include <vector>
#include <set>

namespace tomo 
{
  template<int DIMENSIONS, typename SCALAR = DEFAULT_TYPE>  
  struct Vertex : public Primitive<DIMENSIONS,SCALAR>
  {
    typedef SCALAR Scalar;
    typedef Bounds<DIMENSIONS,Scalar> bounds_type;
    typedef Vec<DIMENSIONS,Scalar> vector_type;
    typedef Point<DIMENSIONS,Scalar> point_type;
    typedef Ray<DIMENSIONS,Scalar> ray_type;
    typedef Primitive<DIMENSIONS,Scalar> primitive_type;

    Vertex(point_type _v = point_type(), vector_type _n = vector_type()) { v(_v); n(_n); }
    void set(point_type _v, vector_type _n = vector_type()) { v = _v; n = _n; }

    bool intersect(ray_type& _ray, Scalar& _tNear, Scalar& _tFar, vector_type* _normal = NULL) const 
    {
      return false;
    }

    SplitPlaneIntersect intersect(Axis _axis, Scalar _splitPos, 
      const bounds_type& _boundsLeft, const bounds_type& _boundsRight) const
    {
      return primitive_type::intersect(_axis,_splitPos,_boundsLeft,_boundsRight);
    }

    bounds_type bounds() const { return bounds_type(v,v); }

    point_type v;
    vector_type n;
    bool hasNormal() { return (n.length() != 0.0f); }
  };

  typedef Vertex<2,float> Vertex2f;
  typedef Vertex<3,float> Vertex3f;
  typedef Vertex<2,unsigned short> Vertex2us;
}
