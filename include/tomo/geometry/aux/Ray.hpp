#pragma once 
#include "tomo/geometry/base/Vector.hpp"

#include <vector>

namespace tomo
{
  namespace geometry
  { 
    namespace aux
    {
  template<int DIMENSIONS, typename SCALAR = base::DEFAULT_TYPE>  
  struct Ray
  {
    typedef SCALAR Scalar;
    typedef base::Point<DIMENSIONS,Scalar> point_type;
    typedef base::Vec<DIMENSIONS,Scalar> vector_type;

    Ray(const point_type _org = point_type(), const vector_type _dir = vector_type(), 
        Scalar _tNear = 0.002, Scalar _tFar = std::numeric_limits<Scalar>::max()) : 
      org_(_org), dir_(_dir), tNear_(_tNear), tFar_(_tFar), primitive_(NULL)  {  }

    bool intersection(void* _primitive, Scalar _t, Scalar _tNear, Scalar _tFar) 
    {
      if (_t >= _tNear && _t < _tFar)
      {
      primitive_ = _primitive;
      tFar_ = _t;
      return true;
      }
      return false;
    }

    inline bool intersection(void* _primitive, Scalar _t)
    {
      return intersection(_primitive,_t,tNear_,tFar_);
    }

    point_type intersectionPoint() const
    {
      return org_ + dir_ * tNear_;
    }

    void params(point_type _org, vector_type _dir) 
    {
      org_=_org; dir_=_dir;
    }

    TBD_PROPERTY_REF(point_type,org);
    TBD_PROPERTY_REF(vector_type,dir);
    TBD_PROPERTY_REF(Scalar,tNear);
    TBD_PROPERTY_REF(Scalar,tFar);

  private:
    void* primitive_;
  };

  typedef Ray<2,float> Ray2f; 
  typedef Ray<3,float> Ray3f;

}
}}
