#pragma once

#include "tomo/SlicableObject.hpp"
#include "tomo/BoundingBox.hpp"
#include "tomo/KDTree.hpp"

#include <boost/foreach.hpp>

namespace tomo
{

  /** @brief A Compound is object is a slicable object which consists of several primtives
   * @detail Moreover, a compound holds a KDTree structure for fast intersection + traversal
   * @tparam PRIMITIVE  Primitive type
   */
  template <class PRIMITIVE>
  class Compound : public SlicableObject, public KDTree<PRIMITIVE>
  {
    public:
      /* @brief Return bounds of bounding box
       */
      Bounds bounds() const
      {
        return Bounds(boundingBox_.min(),boundingBox_.max());
      }

    protected:
      /* @brief Determine extents of bounding box
       */
      void calcBoundingBox()
      {
        boundingBox_ = BoundingBox();
        BOOST_FOREACH ( PRIMITIVE& _obj , objs_ ) 
          boundingBox_.extend(_obj.bounds());
      }

      /// Bounds are caches by a bounding box
      BoundingBox boundingBox_;
      
      /// Container for primitives
      std::vector<PRIMITIVE> objs_;
  };
}


