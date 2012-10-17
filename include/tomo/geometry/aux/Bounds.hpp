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
      /** @brief Rectangular bounding area
       * @tparam DIMENSIONS number of dimensions of the underlying space
       * @tparam SCALAR scalar type for each dimension
       */
      template<class MODEL>
      struct Bounds : MODEL
      {
        TOMO_MODEL_TYPES(MODEL);
        typedef base::Point<MODEL> point_type;
        typedef base::Vec<MODEL> vector_type;

        /// Maximum corner
        static const scalar_type maxmax()
        {
          return MODEL::scalarMax();
        }
        /// Minimum corner
        static const scalar_type minmin()
        {
          return MODEL::scalarMin();
        }

        /// Default constructor
        Bounds()
        {
          min_.vectorize(maxmax());
          max_.vectorize(minmin());
        }
        /** @brief Initialize bounds by two opposite corner points
         * @param _min minimum coordinate
         * @param _max maximum coordinate
         */
        Bounds(const point_type& _min, const point_type& _max)
        {
          operator()(_min,_max);
        }
        
        /** @brief Extend this bounds so that it includes the given bounds
         * @param _that Bounds to include
         * @attention assertion when this or _that are not valid
         */
        void extend(const Bounds& _that)
        {
          BOOST_ASSERT(valid() && _that.valid());
          TOMO_FOREACH_DIM(i)
          {
            min_[i] = std::min(_that.min_[i],min_[i]);
            max_[i] = std::max(_that.max_[i],max_[i]);
          }
        }
        /** @brief Extend this bounds so that it includes the given point
         * @param _that Point to include
         * @attention assertion when this is not valid
         */
        void extend(const point_type& _that)
        {
          BOOST_ASSERT(valid());
          TOMO_FOREACH_DIM(i)
          {
            min_[i] = std::min(_that[i],min_[i]);
            max_[i] = std::max(_that[i],max_[i]);
          }
        }
        /** @brief Test if the given point is inside bounds
         * @param _that point to check
         * @return true if point was inside and false otherwise
         * @attention assertion when this is not valid
         */
        bool inside(const point_type& _that) const
        {
          BOOST_ASSERT(valid());
          TOMO_FOREACH_DIM(i)
          {
            if (_that[i] < min_[i] || _that[i] > max_[i]) 
              return false;
          }
          return true;
        }
        /** @brief Test if two bounds overlap
         * @param _first first bounds
         * @param _second second bounds
         * @return true if _first and _second share some points
         * @attention assertion when _first or _second are not valid
         */
        friend bool overlap(const Bounds& _first, const Bounds& _second)
        {
          BOOST_ASSERT(_first.valid() && _second.valid());
          TOMO_FOREACH_DIM(i)
          {
            if (_first.min_[i] > _second.max_[i] || _first.max_[i] < _second.min_[i]) 
              return false;
          }
          return true;
        }

        /** @brief Return axis with largest extent
         * @return axis with largest extent
         */
        base::Axis dominantAxis() const
        {
          return size().dominantAxis() ;
        }

        /** @brief Set to the smallest bounds that include two points
         * @param _first first point
         * @param _second second point
         */
        void operator()(const point_type& _first, const point_type& _second)
        {
          min_ = _first;
          max_ = _second;
          validate();
        }

        /** @brief Return the size of this bounds
         * @return size of this bounds as vector
         */
        vector_type size() const
        {
          BOOST_ASSERT(valid());
          return max_ - min_;
        }

        /** @brief Return radius of this bounds
         * @return radius as scalar
         */
        scalar_type radius() const
        {
          return size().length()/2;
        }
        /** @brief Return center of this bounds
         * @return center as point
         */
        const point_type center() const
        {
          return 0.5*(max_.vec() + min_.vec());
        }

        /** @brief Split bounds in two halves
         * @param _splitPos scalar position on the given axis where to split this bounds
         * @param _axis axis to split along
         * @param _first [OUT] first half result
         * @param _second [OUT] second half result
         * @return true if _first and _second become the same, otherwise false
         */
        bool split(scalar_type _splitPos, base::Axis _axis, Bounds& _first, Bounds& _second) const
        {
          BOOST_ASSERT(valid());
          // split position must be within bounds!
          BOOST_ASSERT(_splitPos >= min_[_axis] && _splitPos <= max_[_axis]);
          // calculate bounds
          point_type _min = min_, _max = max_;
          _min[_axis] = _splitPos;
          _max[_axis] = _splitPos;
          _second(_min,max_);
          _first(min_,_max);
          // check if bounds has no extension on the given axis
          return min_[_axis] == max_[_axis];
        }
        /// normalize the coordinates in that bounds
        void validate()
        {
          TOMO_FOREACH_DIM(i)
          {
            if (!valid_dim(i))
              std::swap(min_[i],max_[i]);
          }
        }
        /** @brief check if the coordinates in this bounds are normalized
         * @return true if coordinates are valid
         */
        bool valid() const
        {
          TOMO_FOREACH_DIM(i)
          {
            if (!valid_dim(i))
             return false; 
          }
          return true;
        }
        /** @brief check if some coordinates in this bounds are normalized
         * @param i dimension of the coordinate to check
         * @return true if coordinates are valid
         */
        bool valid_dim(int i) const
        {
          /// @todo I don't understand why you have to check min_ for maxmax and max_ for minmin. Please explain!
          if( min_[i] != maxmax() &&  max_[i] != minmin() )
            if (min_[i] > max_[i] )
              return false;
          return true;
        }
        
        template<class ARCHIVE>
        void serialize( ARCHIVE& _ar, const unsigned int _fileVersion ) 
        {
          _ar & min_;
          _ar & max_;
        }

        TBD_PROPERTY_MON(point_type,min,validate);
        TBD_PROPERTY_MON(point_type,max,validate);
      };

      // Bounds operators
      namespace 
      {
        template<class T, class MODEL> 
          inline const Bounds<MODEL>& operator|=(Bounds<MODEL>& _lvalue, const T& _rvalue) 
        { 
          _lvalue.extend(_rvalue); 
          return _lvalue; 
        }
        template<class T, class MODEL> 
          inline Bounds<MODEL>&& operator|(const Bounds<MODEL>& _first, const T& _second) 
        {
          Bounds<MODEL> _result = _first;
          _result.extend(_second); 
          return _result; 
        }
        template<class T,class MODEL> 
          inline bool operator&&(const Bounds<MODEL>& _first,  const T& _second)  
        { 
          return _first.inside(_second); 
        }
        template<class MODEL> 
          inline bool operator&&(const Bounds<MODEL>& _first,  const Bounds<MODEL>& _second)
        { 
          return _first.overlap(_second); 
        }
      }

      typedef Bounds<Model2f> Bounds2f;
      typedef Bounds<Model3f> Bounds3f;
    }
  }
}
