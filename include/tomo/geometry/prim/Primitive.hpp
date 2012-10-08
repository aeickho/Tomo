#pragma once

#include "tomo/geometry/aux/Ray.hpp"
#include "tomo/geometry/aux/Bounds.hpp"

namespace tomo
{
  namespace geometry
  {
    namespace prim
    {
      /// Struct which holds the result of an intersection test between split plane and primitive
      struct SplitPlaneIntersect
      {
      public:
        SplitPlaneIntersect(bool _left = false, bool _right = false) : result_(0)
        {
          set(_left,_right);
        }

        void set(bool _left, bool _right)
        {
          result_ = _left | (_right << 1);
        }

        bool left()
        {
          return result_ & 1;
        }
        bool right()
        {
          return result_ & 2;
        }
        bool both()
        {
          return result_ != 0;
        }
      private:
        unsigned char result_;
      };

      /// A primitive is an object which has an extent and for which an intersection point can be found
      template<int DIMENSIONS, typename SCALAR = base::DEFAULT_TYPE>
      struct Primitive
      {
        typedef SCALAR scalar_type;
        typedef base::Point<DIMENSIONS,scalar_type> point_type;
        typedef base::Vec<DIMENSIONS,scalar_type> vec_type;
        typedef aux::Ray<DIMENSIONS,scalar_type> ray_type;
        typedef aux::Bounds<DIMENSIONS,scalar_type> bounds_type;

        /** @brief Return pointer to object
         */
        void* pointer() const
        {
          return (void*)const_cast<Primitive*>(this);
        }
      };

      typedef Primitive<2,float> Primitive2f;
      typedef Primitive<3,float> Primitive3f;
    }
  }
}
