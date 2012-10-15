#pragma once

#include "BoundingBox.hpp"
#include "tomo/slicing/SlicableObject.hpp"

namespace tomo
{
  namespace geometry
  {
    namespace prim
    {
      /** @brief A bounding is primitive which defines bounds of a compound object
       */
      struct Box : 
        public slicing::SlicableObject,
        public BoundingBox
      {
        Box() : BoundingBox() {}
        Box(point_type _min, point_type _max) : BoundingBox(_min,_max) {}

        void slice();
        
      private:
        void generateSlice(slicing::Slice* _slice) const;
      };
    }
  }
}
