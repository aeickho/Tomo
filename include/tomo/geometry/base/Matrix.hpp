#include "Coord.hpp"

namespace tomo
{
  namespace geometry
  {
    namespace base
    {
      template<class MODEL>
      class Matrix : MODEL
      {
      public:
        TOMO_MODEL_TYPES(MODEL);
        typedef Coords<MODEL> coords_type;
        
        Matrix()
        {
          loadIdentity();
        };

        void loadIdentity()
        {
          TOMO_FOREACH_DIM(i)
          {
            TOMO_FOREACH_DIM(j)
            {
              Matrix::operator()(i,j) = (i == j) ? 1 : 0;
            }
          }
        }

        const scalar_type& operator()(int _x, int _y) const
        {
          return a_[_x][_y];
        }

        scalar_type& operator()(int _x, int _y)
        {
          return a_[_x][_y];
        }

      private:
        coords_type a_[dimensions_];
      };

      typedef Matrix<Model2f> Matrix2f;
      typedef Matrix<Model3f> Matrix3f;
      typedef Matrix<Model4f> Matrix4f;
    }
  }
}
