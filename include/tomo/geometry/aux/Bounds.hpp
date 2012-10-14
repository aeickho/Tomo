#pragma once

#include "tomo/geometry/base/Vector.hpp"
#include "tomo/misc.hpp"

#include <vector>
#include <limits>

namespace tomo
{
  namespace geometry
  {
    namespace aux
    {
      template<int DIMENSIONS, typename SCALAR = base::DEFAULT_TYPE>
      struct Bounds
      {
        typedef SCALAR scalar_type;
        typedef base::Point<DIMENSIONS,scalar_type> point_type;
        typedef base::Vec<DIMENSIONS,scalar_type> vector_type;

        Bounds()
        {
          min_.vectorize(std::numeric_limits<scalar_type>::max());
          max_.vectorize(std::numeric_limits<scalar_type>::min());
        }
        Bounds(point_type _min, point_type _max)
        {
          this->operator()(_min,_max);
        }

        /// Retrieve the corners of bounds with as a vector of points
        std::vector<point_type> corners() const
        {
          int _nCorners = 1 << DIMENSIONS;
          std::vector<point_type> _corners(_nCorners);
          for (int _corner = 0; _corner < _nCorners; _corner++)
            TOMO_FOREACH_DIM
            _corners[_corner][i] = (_corner & (1 << i)) ? max_[i] : min_[i];
          return _corners;
        }

        void extend(const Bounds& _bounds)
        {
          TOMO_FOREACH_DIM
          {
            min_[i] = std::min(_bounds.min_[i],min_[i]);
            max_[i] = std::max(_bounds.max_[i],max_[i]);
          }
        }

        void extend(const point_type& _point)
        {
          TOMO_FOREACH_DIM
          {
            if (min_[i] > _point[i]) min_[i] = _point[i];
            if (max_[i] < _point[i]) max_[i] = _point[i];
          }
        }

        /// Test if point is inside bounds
        bool inside(point_type _p) const
        {
          TOMO_FOREACH_DIM
          {
            if (_p[i] < min_[i] || _p[i] > max_[i]) return false;
          }
          return true;
        }


        /// Test if two bounds overlap
        friend bool overlap(const Bounds& _a, const Bounds& _b)
        {
          TOMO_FOREACH_DIM
          {
            if (_a.min()[i] > _b.max()[i] || _a.max()[i] < _b.min()[i]) return false;
          }
          return true;
        }

        /// Return axis which largest extent
        base::Axis dominantAxis() const
        {
          return size().dominantAxis() ;
        }

        /// Set new values and validate
        void operator()(const point_type& _min, const point_type& _max)
        {
          min_ = _min;
          max_ = _max;
          validate();
        }

        /// Return the size vector
        vector_type size() const
        {
          return max_ - min_;
        }

        /// Return bounds' radius
        scalar_type radius() const
        {
          return size().length()/2;
        }

        /// Return bounds' center
        point_type center() const
        {
          return 0.5*(max().vec() + min().vec());
        }

        /// Split bounds in two halves
        void split(scalar_type _splitPos, base::Axis _axis, Bounds& _left, Bounds& _right) const
        {
          point_type _min = min(), _max = max();
          _min[_axis] = _splitPos;
          _max[_axis] = _splitPos;
          _right(_min,max());
          _left(min(),_max);
        }

        void validate()
        {
          TOMO_FOREACH_DIM
          {
            if (min_[i] != std::numeric_limits<scalar_type>::max() &&
            max_[i] != std::numeric_limits<scalar_type>::min())
              if (min_[i] > max_[i])
                std::swap(min_[i],max_[i]);
          }
        }

        TBD_PROPERTY_MON(point_type,min,validate);
        TBD_PROPERTY_MON(point_type,max,validate);
      };

      typedef Bounds<2,float> Bounds2f;
      typedef Bounds<3,float> Bounds3f;
    }
  }
}
