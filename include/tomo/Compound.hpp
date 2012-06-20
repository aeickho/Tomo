#pragma once

#include "tomo/SlicableObject.hpp"
#include "tomo/BoundingBox.hpp"
#include "tomo/KDTree.hpp"

#include <boost/foreach.hpp>

namespace tomo
{
  template <class SUBOBJECT>
  class Compound : public SlicableObject, public KDTree<SUBOBJECT>
  {
    public:
      virtual void read(const std::string& _filename) = 0;

      Bounds bounds() const
      {
        return Bounds(boundingBox_.min(),boundingBox_.max());
      }


    protected:
      void calcBoundingBox()
      {
        boundingBox_ = BoundingBox();
        BOOST_FOREACH ( SUBOBJECT& _obj , objs_ ) 
          boundingBox_.extend(_obj.bounds());
      }
      BoundingBox boundingBox_;
      std::vector<SUBOBJECT> objs_;
  };
}


