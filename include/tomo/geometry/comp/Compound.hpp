#pragma once

#include "tomo/geometry/prim/BoundingBox.hpp"
#include "tomo/misc.hpp"
#include "tomo/geometry/kd/Tree.hpp"


#include <boost/foreach.hpp>

/// inline implement compound primitive container accessor for tomo::geometry::aux::Compound
#define TOMO_COMPOUND_PRIMITIVE_NAME(name) \
        ctnr_type& name() { return objs_; } \
        const ctnr_type& name() const { return objs_; }

namespace tomo
{
  namespace geometry
  {
    namespace comp
    {
      /** @brief A Compound is object is an object which consists of several primtives
       * @details Moreover, a compound holds a KDTree structure for fast search
       * @tparam PRIMITIVE  Primitive type
       * @todo Large code blocks of inRadius and kNearest are similar, refactor it
       */
      template <class PRIMITIVE>
      struct Compound : public prim::Primitive<typename PRIMITIVE::model_type>
      {
        TOMO_PRIMITIVE_TYPES(prim::Primitive<typename PRIMITIVE::model_type>);
        typedef PRIMITIVE value_type;
        typedef kd::Tree<value_type> kdtree_type;
        typedef std::vector<value_type> ctnr_type;
        typedef std::vector<value_type*> ptr_ctnr_type;

        TOMO_COMPOUND_PRIMITIVE_NAME(objs)

        /** @brief add primitve to this Compound
         * @param _primitive Primitive to append
         */
        void add(const value_type& _primitive)
        {
          kdTree_.clear();
          objs_.push_back(_primitive);
        }

        /// Aggregate another compound to this one
        void aggregate(const Compound& _compound, bool _update = true)
        {
          objs_.insert(objs_.end(),_compound.objs_.begin(),_compound.objs_.end());
//          if (_update) update();
        }

        bounds_type bounds() const
        {
          return kdTree_.bounds_;
        }

        template<class NODE_INTERSECTOR>
        void validate()
        {
          if( !valid() )
            kdTree_.template build<NODE_INTERSECTOR>(objs_,3);
        }

        bool valid() const 
        {

          return !kdTree_.empty();
        }

        TBD_PROPERTY_REF(kdtree_type,kdTree);

      protected:
        ctnr_type objs_;
      };
    }
  }
}
