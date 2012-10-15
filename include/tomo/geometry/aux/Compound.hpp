#pragma once

#include "tomo/geometry/prim/BoundingBox.hpp"
#include "KDTree.hpp"
#include "tomo/misc.hpp"
#include "visitor/InRadius.hpp"
#include "visitor/Nearest.hpp"
#include "visitor/KNearest.hpp"
#include "visitor/RayTraversal.hpp"

#include <boost/foreach.hpp>

/// inline implement compound primitive container accessor for tomo::geometry::aux::Compound
#define TOMO_COMPOUND_PRIMITIVE_NAME(name) \
        ctnr_type& name() { return objs_; } \
        const ctnr_type& name() const { return objs_; }

namespace tomo
{
  namespace geometry
  {
    namespace aux
    {
      /** @brief A Compound is object is an object which consists of several primtives
       * @detail Moreover, a compound holds a KDTree structure for fast search
       * @tparam PRIMITIVE  Primitive type
       * @todo Large code blocks of inRadius and kNearest are similar, refactor it
       */
      template <class PRIMITIVE>
      struct Compound : public prim::Primitive<typename PRIMITIVE::model_type>
      {
        TOMO_PRIMITIVE_TYPES(prim::Primitive<typename PRIMITIVE::model_type>);
        typedef PRIMITIVE value_type;
        typedef KDTree<value_type> kdtree_type;
        typedef std::vector<value_type> ctnr_type;
        typedef std::vector<value_type*> ptr_ctnr_type;

        TOMO_COMPOUND_PRIMITIVE_NAME(objs)

        /** @brief add primitve to this Compound
         * @param _primitive Primitive to append
         */
        void add(const value_type& _primitive)
        {
          objs_.push_back(_primitive);
        }

        /// Aggregate another compound to this one
        void aggregate(const Compound& _compound, bool _update = true)
        {
          objs_.insert(objs_.end(),_compound.objs_.begin(),_compound.objs_.end());
          if (_update) update();
        }

        /// Calculates bounds and constructs kdtree
        void update()
        {
          kdTree_.build(objs_,3);
        }

        bounds_type bounds() const
        {
          return kdTree_.bounds_;
        }

        TBD_PROPERTY_REF(kdtree_type,kdTree);

      protected:
        ctnr_type objs_;

        /// Calculates the distance of a primitive to a kdtree node
        scalar_type nodeDistance(const value_type* _p, const bounds_type _bounds) const
        {
          if (overlap(_bounds,_p->bounds())) return 0.0;
          float _minDist = INF;

          TOMO_FOREACH_DIM(i)
          {
            _minDist = std::min(std::min(std::abs(_p->center()[i] - _bounds.min()[i]),
                                         std::abs(_bounds.max()[i]- _p->center()[i])),
                                _minDist);
          }
          return _minDist;
        }
      };
    }
  }
}
