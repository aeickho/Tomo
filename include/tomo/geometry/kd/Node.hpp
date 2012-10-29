#pragma once
#include <vector>
#include <tbd/bit.h>
#include <stdint.h>

namespace tomo
{
  namespace geometry
  {
    namespace kd
    {
      // get bit operations from TBD
      namespace bit=tbd::bit;

      template <typename PRIMITIVE>
      union Node
      {
        typedef typename PRIMITIVE::scalar_type scalar_type;
        typedef std::vector<const PRIMITIVE*> cntr_type;
        
        bool isLeaf() const
        {
          return !inner_.is();
        }

        /// Inner node
        struct Inner
        {
          typedef std::vector<Node> NodeCont;

          inline bool is() const
          {
            return bit::get<bool>(data_,31,1);
          }
          inline base::Axis axis() const
          {
            return bit::get<base::Axis>(data_,0,2);
          }
          /// Insert children
          inline void setup(NodeCont& _nodes, base::Axis _axis, scalar_type _splitPos)
          {
            data_ = 0;
            bit::set(data_,true,31,1);
            bit::set(data_,_nodes.size(),2,29);
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
          TBD_PROPERTY_RO(scalar_type,splitPos);
        private:
          uint32_t      data_;
        };

        /// Leaf node
        struct Leaf
        {
          typedef typename cntr_type::const_iterator const_iterator;

          const_iterator begin(const cntr_type& _src) const
          {
            return _src.begin() + begin_;
          }
          const_iterator end(const cntr_type& _src) const
          {
            return _src.begin() + end_;
          }

          TBD_PROPERTY(uint32_t,begin);
          TBD_PROPERTY(uint32_t,end);
        };

        Inner inner_;
        Leaf leaf_;
      };
    }
  }
}
