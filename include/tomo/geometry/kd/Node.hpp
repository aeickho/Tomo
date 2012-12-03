#pragma once
#include <tbd/bit.h>
#include "NodeGeometry.hpp"

namespace tomo
{
  namespace geometry
  {
    namespace kd
    {
      namespace bit=tbd::bit;

      template
      <
        typename MODEL,
        typename ATTRIBUTES,
        typename INNER_ATTRIBUTES,
        typename LEAF_ATTRIBUTES
      >
      struct Node
      {
        typedef MODEL model_type;
        typedef typename MODEL::scalar_type scalar_type;
        typedef scalar_type splitpos_type;
        typedef NodeGeometry<model_type> geometry_type;

        typedef ATTRIBUTES attr_type;

        bool isLeaf() const
        {
          return !inner_.is();
        }

        struct Inner
        {
          typedef INNER_ATTRIBUTES attr_type;

          inline bool is() const
          {
            return bit::get<bool>(data_,31,1);
          }

          inline base::Axis axis() const
          {
            return bit::get<base::Axis>(data_,0,2);
          }

          /// Set values
          inline void setup(
            uint32_t _index,
            base::Axis _axis,
            float _splitPos,
            attr_type _attributes = attr_type())
          {
            bit::set(data_,true,31,1);
            bit::set(data_,_index,2,29);
            bit::set(data_,_axis,0,2);
            splitPos_ = _splitPos;
          }

          inline uint32_t left() const
          {
            return bit::get<uint32_t>(data_,2,29);
          }
          inline uint32_t right() const
          {
            return left() + 1;
          }

          TBD_PROPERTY_RO(uint32_t,data)
          TBD_PROPERTY_RO(float,splitPos)
          TBD_PROPERTY_REF(attr_type,attributes)
        };

        struct Leaf
        {
          typedef LEAF_ATTRIBUTES attr_type;
          TBD_PROPERTY_REF(attr_type,attributes)
        };

        Leaf& leaf()
        {
          TOMO_ASSERT(isLeaf());
          return leaf_;
        }

        const Leaf& leaf() const
        {
          TOMO_ASSERT(isLeaf());
          return leaf_;
        }

        Inner& inner()
        {
          TOMO_ASSERT(inner_.is());
          return inner_;
        }

        Inner& inner(uint32_t _index,
                     base::Axis _axis,
                     float _splitPos,
                     typename Inner::attr_type _attributes = typename Inner::attr_type())
        {
          inner_.setup(_index,_axis,_splitPos,_attributes);
          return inner();
        }

        const Inner& inner() const
        {
          TOMO_ASSERT(inner_.is());
          return inner_;
        }

      private:
        union
        {
          Inner inner_;
          Leaf leaf_;
        };

      public:
        TBD_PROPERTY_REF(attr_type,attributes)
      };

      /// A void attribute...
      struct EmptyAttribute {};
      
      /// An attribute which holds a range of indices
      struct RangeAttribute
      {
        TBD_PROPERTY(uint32_t,begin)
        TBD_PROPERTY(uint32_t,end)
      };
    }
  }
}
