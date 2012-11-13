#pragma once
#include <vector>
#include <tbd/bit.h>
#include <stdint.h>
#include "NodeGeometry.hpp"
#include "NodeIntersectResult.hpp"

namespace tomo
{
  namespace geometry
  {
    namespace kd
    {
      // get bit operations from TBD
      namespace bit=tbd::bit;

      namespace detail
      {
        struct NoAttributes
        {
        };
      }

      template<typename ATTRIBUTES>
      struct NodeConcept : ATTRIBUTES
      {
        bool isLeaf() const
        {
          return !inner_.is();
        }

        struct Inner
        {
          inline bool is() const
          {
            return bit::get<bool>(data_,31,1);
          }

          inline base::Axis axis() const
          {
            return bit::get<base::Axis>(data_,0,2);
          }

          /// Set values
          inline void setup(uint32_t _index, base::Axis _axis, float _splitPos)
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

          TBD_PROPERTY_RO(float,splitPos);
          TBD_PROPERTY_RO(uint32_t,data);
        };

        struct Leaf
        {
          TBD_PROPERTY(uint32_t,begin);
          TBD_PROPERTY(uint32_t,end);
        };

        union
        {
          Inner inner_;
          Leaf leaf_;
        };
      };

      /// Standard kd node without any additional attributes
      typedef NodeConcept<detail::NoAttributes> Node;
    }
  }
}
