#pragma once
#include <vector>
#include <tbd/bit.h>
#include <stdint.h>

namespace tomo
{
  namespace geometry
  {
    namespace aux
    {
      namespace bit=tbd::bit;

      template <typename PRIMITIVE>
      union KDNode
      {
        typedef typename PRIMITIVE::scalar_type scalar_type;
        typedef typename PRIMITIVE::vec_type vec_type;
        typedef std::vector<PRIMITIVE*> PrimCont;
        bool isLeaf() const
        {
          return !inner_.is();
        }

        /// Inner node
        struct Inner
        {
          typedef std::vector<KDNode> NodeCont;

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

          inline unsigned left() const
          {
            return bit::get<uint32_t>(data_,2,29);
          }
          inline unsigned right() const
          {
            return left() + 1;
          }

          inline scalar_type splitPos() const
          {
            return splitPos_;
          }

        private:
          uint32_t      data_;
          scalar_type   splitPos_;
        };

        /// Leaf node
        struct Leaf
        {
          typedef std::vector<PRIMITIVE*> PrimCont;
          typedef typename PrimCont::iterator iterator;
          typedef typename PrimCont::const_iterator const_iterator;

          const_iterator begin(const PrimCont& _src) const
          {
            return _src.begin() + offset_;
          }
          
          const_iterator end(const PrimCont& _src) const
          {
            return _begin(_src) + size_;
          }

          iterator begin(PrimCont& _src) const
          {
            return _src.begin() + offset_;
          }
          
          iterator end(PrimCont& _src) const
          {
            return _begin(_src) + size_;
          }

          TBD_PROPERTY(uint32_t,size);
          TBD_PROPERTY(uint32_t,offset);
        };

        Inner inner_;
        Leaf leaf_;
      };
    }
  }
}
