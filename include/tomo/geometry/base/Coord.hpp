#ifndef _COORD_HPP
#define _COORD_HPP

#include "tomo/misc.hpp"
#include "tomo/geometry/Model.hpp"

#include <boost/static_assert.hpp>
#include <sstream>
#include <tbd/log.h>

namespace tomo
{
  namespace geometry
  {
    namespace base
    {
      class Matrix4f
      {
      public:
        Matrix4f()
        {
          for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
              set(i,j,(i == j) ? 1.0f : 0.0f);
        };

        void  set(int _x, int _y, float v)
        {
          c[_x*4+_y] = v;
        }
        float get(int _x, int _y)
        {
          return c[_x*4+_y];
        }

        void  print();

      private:
        float c[4*4];
      };

/// Compiler macro for iterating over each dimension
#define TOMO_FOREACH_DIM(i) for (int i = 0; i < dimensions_; i++)
#define TOMO_FOREACH_DIM_(t,i) for (int i = 0; i < t.dimensions_; i++)

      /** @brief Base class of Point and Vec which basically hold a number of coordinates
       * @tparam dimensions_ Number of dimensions
       * @tparam COORD_TYPE Coordinate type
       */
      template<class MODEL>
      struct Coords : MODEL
      {
        TOMO_MODEL_TYPES(MODEL);
        typedef scalar_type value_type;
        /// Base constructor, all values are initially set to zero
        Coords()
        {
          TOMO_FOREACH_DIM(i) a_[i] = 0;
        }
        Coords( Coords& _coords )
        {
          TOMO_FOREACH_DIM(i) a_[i] = _coords[i];
        }
        Coords( const Coords& _coords )
        {
          TOMO_FOREACH_DIM(i) a_[i] = _coords[i];
        }
        Coords( scalar_type _x, scalar_type _y)
        {
          this->operator()(_x,_y);
        }
        Coords( scalar_type _x, scalar_type _y, scalar_type _z)
        {
          this->operator()(_x,_y,_z);
        }
        Coords( scalar_type _x, scalar_type _y, scalar_type _z, scalar_type _w)
        {
          this->operator()(_x,_y,_z,_w);
        }

        inline void operator()(const Coords _coords)
        {
          TOMO_FOREACH_DIM(i) a_[i] = _coords[i];
        }
        inline void operator()(scalar_type _x, scalar_type _y)
        {
          x(_x);
          y(_y);
        }
        inline void operator()(scalar_type _x, scalar_type _y, scalar_type _z)
        {
          x(_x);
          y(_y);
          z(_z);
        }
        inline void operator()(scalar_type _x, scalar_type _y, scalar_type _z, scalar_type _w)
        {
          x(_x);
          y(_y);
          z(_z);
          w(_w);
        }

        /* @brief Return pointer */
        scalar_type* p()
        {
          return a_;
        }
        /* @brief Return const pointer */
        const scalar_type* p() const
        {
          return a_;
        }

        /// Methods to return coordinate values
        inline scalar_type x() const
        {
          BOOST_STATIC_ASSERT(dimensions_ >= 1);
          return a_[0];
        }
        inline scalar_type y() const
        {
          BOOST_STATIC_ASSERT(dimensions_ >= 2);
          return a_[1];
        }
        inline scalar_type z() const
        {
          BOOST_STATIC_ASSERT(dimensions_ >= 3);
          return a_[2];
        }
        inline scalar_type w() const
        {
          BOOST_STATIC_ASSERT(dimensions_ >= 4);
          return a_[3];
        }

        /// Methods to set coordinate values
        inline void x(const scalar_type _x)
        {
          BOOST_STATIC_ASSERT(dimensions_ >= 1);
          a_[0] = _x;
        }
        inline void y(const scalar_type _y)
        {
          BOOST_STATIC_ASSERT(dimensions_ >= 2);
          a_[1] = _y;
        }
        inline void z(const scalar_type _z)
        {
          BOOST_STATIC_ASSERT(dimensions_ >= 3);
          a_[2] = _z;
        }
        inline void w(const scalar_type _w)
        {
          BOOST_STATIC_ASSERT(dimensions_ >= 4);
          a_[3] = _w;
        }

        /// Methods to access coordinate value in a certain dimension
        scalar_type& operator[] (int i)
        {
          return a_[i];
        }
        const scalar_type& operator[] (int i) const
        {
          return a_[i];
        }

        void operator += ( const Coords _c )
        {
          TOMO_FOREACH_DIM(i) a_[i] += _c[i];
        }
        void operator -= ( const Coords _c )
        {
          TOMO_FOREACH_DIM(i) a_[i] -= _c[i];
        }

        /// size/dimension of the vector
        static const size_t size_ = dimensions_;

        /// size/dimension of the vector
        static size_t size()
        {
          return dimensions_;
        }

        Coords vectorize(const scalar_type& _s)
        {
          TOMO_FOREACH_DIM(i) a_[i] = _s;
          return *this;
        }

        operator std::string() const
        {
          std::stringstream ss;
          ss << "(";
          TOMO_FOREACH_DIM(i)
          ss << a_[i] << ((i < dimensions_) ? "," : ")");
          return ss.str();
        }

        template<class ARCHIVE>
        void serialize( ARCHIVE& _ar, const unsigned int _fileVersion )
        {
          TOMO_FOREACH_DIM(i)
          {
            _ar & a_[i];
          }
        }
      protected:
        /// Array to store coordinate values
        scalar_type a_[dimensions_];
      };

      typedef enum { X,Y,Z,W } Axis;

#define COORDS(C) C.x(),C.y(),C.z()
    }

    template<class MODEL>
    inline fmt operator%( fmt _fmt, const tomo::geometry::base::Coords<MODEL>& _coords)
    {
      return _fmt % (std::string)_coords;
    }
  }
}
#endif /* _COORD_HPP */
