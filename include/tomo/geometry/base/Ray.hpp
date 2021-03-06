#pragma once

#include "Vector.hpp"
#include <vector>

namespace tomo
{
  namespace geometry
  {
    namespace base
    {
      template<class MODEL>
      struct Ray : MODEL
      {
        TOMO_MODEL_TYPES(MODEL)
        typedef base::Point<MODEL> point_type;
        typedef base::Vec<MODEL> vec_type;

        Ray(const point_type _org = point_type(), const vec_type _dir = vec_type(),
            scalar_type _tNear = 0.0, scalar_type _tFar = MODEL::scalarMax()) :
          org_(_org), dir_(_dir), tNear_(_tNear), tFar_(_tFar), primitive_(NULL)  {  }

        bool intersection(void* _primitive, scalar_type _t, scalar_type _tNear, scalar_type _tFar)
        {
          if (_t >= _tNear && _t <= _tFar && _t >= tNear_ && _t <= tFar_)
          {
            primitive_ = _primitive;
            t_ = _t;
            return true;
          }
          return false;
        }

        inline bool intersection(void* _primitive, scalar_type _t)
        {
          return intersection(_primitive,_t,tNear_,tFar_);
        }

        point_type intersectionPoint() const
        {
          return org_ + tFar_ * dir_;
        }

        void params(point_type _org, vec_type _dir)
        {
          org_=_org;
          dir_=_dir;
        }

        TBD_PROPERTY_REF(point_type,org);
        TBD_PROPERTY_REF(vec_type,dir);
        TBD_PROPERTY_REF(scalar_type,t);
        TBD_PROPERTY_REF(scalar_type,tNear);
        TBD_PROPERTY_REF(scalar_type,tFar);

      private:
        void* primitive_;
      };

      typedef Ray<Model2f> Ray2f;
      typedef Ray<Model3f> Ray3f;
    }
  }
}
