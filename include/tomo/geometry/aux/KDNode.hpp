#pragma once

#include <vector>

namespace tomo
{
  namespace geometry
  {
    namespace aux
    {
      template <typename PRIMITIVE>
      union KDNode
      {
        typedef typename PRIMITIVE::scalar_type scalar_type;
        typedef typename PRIMITIVE::vec_type vec_type;
        typedef std::vector<PRIMITIVE*> PrimCont;

        bool isLeaf() const
        {
          return !inner_.leafFlag();
        }

        /// Inner node
        struct Inner
        {
          typedef std::vector<KDNode> NodeCont;

          inline bool leafFlag() const
          {
            return data_ & (1 << 31);
          }
          inline base::Axis axis() const
          {
            return base::Axis(data_ & 3);
          }

          /// Insert children
          inline void setup(NodeCont& _nodes, base::Axis _axis, scalar_type _splitPos)
          {
            data_ = (0x7FFFFFFC & (_nodes.size() << 2)) | (1 << 31) | _axis;
            splitPos_ = _splitPos;
          }

          inline unsigned left() const
          {
            return (data_ & 0x7FFFFFFC) >> 2;
          }
          inline unsigned right() const
          {
            return ((data_ & 0x7FFFFFFC) >> 2) + 1;
          }

          inline scalar_type splitPos() const
          {
            return splitPos_;
          }

        private:
          unsigned int data_;
          scalar_type splitPos_;
        };

        /// Leaf node
        struct Leaf
        {
          typedef std::vector<PRIMITIVE*> PrimCont;

          /// Insert Primitive pointers from _src into _dest
          inline void insert(const PrimCont& _src, PrimCont& _dest)
          {
            // Make leaf node
            offset_ = _dest.size();
            size_ = _src.size();
            _dest.resize(offset_ + size_);
            for (unsigned i = 0; i < size_; i++)
              _dest[offset_+i] = _src[i];
          }

          inline PrimCont primitives(const PrimCont& _primLists) const
          {
            PrimCont _primList;
            _primList.resize(size_);
            for (unsigned i = offset_, j = 0; i < offset_ + size_; i++, j++)
              _primList[j] = _primLists[i];
            return _primList;
          }

        private:
          unsigned int size_;
          unsigned int offset_;
        };

        Inner inner_;
        Leaf leaf_;
      };
    }
  }
}
