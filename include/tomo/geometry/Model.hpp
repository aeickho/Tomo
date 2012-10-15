#pragma once

namespace tomo
{
  namespace geometry
  {
    /// DEFAULT_TYPE of coordinates is float
    typedef float DEFAULT_TYPE;

    template<int DIMENSIONS, typename SCALAR=DEFAULT_TYPE>
    struct Model
    {
      typedef SCALAR scalar_type;
      enum { dimensions_ = DIMENSIONS };
      /// Maximum corner
      static const scalar_type scalarMax()
      {
        return std::numeric_limits<scalar_type>::max();
      }
      /// Minimum corner
      static const scalar_type scalarMin()
      {
        return std::numeric_limits<scalar_type>::min();
      }
    };

    typedef Model<2,int> Model2i;
    typedef Model<2,double> Model2d;
    typedef Model<2,float> Model2f;
    typedef Model<2,unsigned short> Model2us;
    typedef Model<2,short> Model2s;
    typedef Model<3,float> Model3f;
  }
}

#define TOMO_MODEL_TYPES(MODEL) \
      typedef MODEL model_type; \
      using typename MODEL::scalar_type; \
      using MODEL::dimensions_;


