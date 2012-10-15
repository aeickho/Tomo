#pragma once

#include "tomo/geometry/prim/BoundingBox.hpp"
#include "KDTree.hpp"
#include "tomo/misc.hpp"
#include "visitor/InRadius.hpp"
#include "visitor/Nearest.hpp"
#include "visitor/KNearest.hpp"
#include "visitor/RayTraversal.hpp"

#include <boost/foreach.hpp>

namespace tomo
{
  namespace geometry
  {
    namespace aux
    {
      /** @brief A Compound is object is an object which consists of several primtives
       * @detail Moreover, a compound holds a KDTree structure for fast search
       * @tparam PRIMITIVE  Primitive type
       */
      template <class PRIMITIVE, unsigned DIMENSIONS, class SCALAR = base::DEFAULT_TYPE>
      class Compound : public prim::Primitive<DIMENSIONS,SCALAR>
      {
        /// TODO: Large code blocks of inRadius and kNearest are similar, refactor it
      public:
#define TOMO_COMPOUND_PRIMITIVE_NAME(name) \
        ctnr_type& name() { return objs_; } \
        const ctnr_type& name() const { return objs_; } 

        typedef SCALAR scalar_type;
        typedef prim::Primitive<DIMENSIONS,scalar_type> primitive_type;
        typedef typename primitive_type::vec_type vec_type;
        typedef typename primitive_type::point_type point_type;
        typedef typename primitive_type::ray_type ray_type;
        typedef typename primitive_type::bounds_type bounds_type;

        typedef KDTree<PRIMITIVE> kdtree_type;
        typedef std::vector<PRIMITIVE> ctnr_type;
        typedef std::vector<PRIMITIVE*> ptr_ctnr_type;

        TOMO_COMPOUND_PRIMITIVE_NAME(objs)

        void add(const PRIMITIVE& _primitive)
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

        bounds_type bounds() const { return kdTree_.bounds_; }

        TBD_PROPERTY_REF(kdtree_type,kdTree);

      protected:
        ctnr_type objs_;

        /// Calculates the distance of a primitive to a kdtree node
        scalar_type nodeDistance(const PRIMITIVE* _p, const bounds_type _bounds) const
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
