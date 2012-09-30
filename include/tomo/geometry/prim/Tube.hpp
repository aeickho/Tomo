#pragma once

#include "tomo/slicing/SlicableObject.hpp"

namespace tomo
{
  namespace geometry
  {
    namespace prim
    {
      /** @brief A tube is a cylinder with a circular hole
       */
      struct Tube : public slicing::SlicableObject 
      {
        typedef float scalar_type;
        typedef base::Point<3,scalar_type> point_type;
        typedef base::Vec<3,scalar_type> vector_type;

        Tube(scalar_type _outerRadius = 1.0, 
             scalar_type _innerRadius = 0.5,
             scalar_type _height = 2.0,
             int _sides = 24);
        
        void slice();
        
        TBD_PROPERTY(scalar_type,outerRadius);
        TBD_PROPERTY(scalar_type,innerRadius);
        TBD_PROPERTY(scalar_type,height);
        TBD_PROPERTY(int,sides);

      private:
        void generateSlice(slicing::Slice* _slice) const;
      };
    }
  }
}
