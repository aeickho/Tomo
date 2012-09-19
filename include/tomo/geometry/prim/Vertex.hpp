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
    typedef base::Vec<DIMENSIONS,scalar_type> vector_type;
    typedef base::Point<DIMENSIONS,scalar_type> point_type;
    typedef aux::Ray<DIMENSIONS,scalar_type> ray_type;
    typedef aux::Bounds<DIMENSIONS,scalar_type> bounds_type;
    typedef Primitive<DIMENSIONS,scalar_type> primitive_type;

    Vertex(point_type _v = point_type(), vector_type _n = vector_type()) { v(_v); n(_n); }
    void set(point_type _v, vector_type _n = vector_type()) { v = _v; n = _n; }

    bool intersect(ray_type& _ray, scalar_type& _tNear, scalar_type& _tFar, vector_type* _normal = NULL) const 
    {
      return false;
    }

    SplitPlaneIntersect intersect(base::Axis _axis, scalar_type _splitPos, 
      const bounds_type& _boundsLeft, const bounds_type& _boundsRight) const
    {
      return primitive_type::intersect(_axis,_splitPos,_boundsLeft,_boundsRight);
    }

    bounds_type bounds() const { return bounds_type(v,v); }
    point_type center() const { return v; }

    point_type v;
    vector_type n;
    bool hasNormal() { return (n.length() != 0.0f); }
  };

  typedef Vertex<2,float> Vertex2f;
  typedef Vertex<3,float> Vertex3f;
  typedef Vertex<2,unsigned short> Vertex2us;
    }}}
