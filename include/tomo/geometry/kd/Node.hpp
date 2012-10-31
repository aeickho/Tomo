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

#define TOMO_NODE_TYPES(NODE) \
          typedef NODE Node; \
          typedef typename Node::geometry_type geometry_type; \
          typedef typename Node::primitive_type primitive_type; \
          typedef typename Node::vec_type vec_type; \
          typedef typename Node::point_type point_type; \
          typedef typename Node::scalar_type scalar_type; \
          typedef typename Node::bounds_type bounds_type; \
          typedef typename Node::intersection_type intersection_type;\
          typedef typename Node::cntr_type cntr_type;\
          typedef typename Node::Inner NodeInner;\
          typedef typename Node::Leaf NodeLeaf; 

      template <typename PRIMITIVE>
      struct Node
      {
        typedef PRIMITIVE primitive_type;
        typedef typename primitive_type::bounds_type bounds_type;
        typedef typename primitive_type::scalar_type scalar_type;
        typedef typename primitive_type::point_type point_type;
        typedef typename primitive_type::vec_type vec_type;
        typedef NodeGeometry< typename primitive_type::model_type > geometry_type; 
        typedef NodeIntersectResult intersection_type;
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
          inline void setup(uint32_t _index, base::Axis _axis, scalar_type _splitPos)
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

          TBD_PROPERTY_RO(scalar_type,splitPos);
          TBD_PROPERTY_RO(uint32_t,data);
        };

        /// Leaf node
        struct Leaf
        {
          typedef typename cntr_type::const_iterator const_iterator;
          typedef typename cntr_type::iterator iterator;

          const_iterator begin(const cntr_type& _src) const
          {
            return _src.begin() + begin_;
          }
          const_iterator end(const cntr_type& _src) const
          {
            return _src.begin() + end_;
          }

          iterator begin(cntr_type& _src) const
          {
            return _src.begin() + begin_;
          }
          iterator end(cntr_type& _src) const
          {
            return _src.begin() + end_;
          }

          TBD_PROPERTY(uint32_t,begin);
          TBD_PROPERTY(uint32_t,end);
        };

        union
        {
          Inner inner_;
          Leaf leaf_;
        };
      };
    }
  }
}
