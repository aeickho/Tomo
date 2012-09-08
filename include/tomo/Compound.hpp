#pragma once

#include "tomo/SlicableObject.hpp"
#include "tomo/BoundingBox.hpp"
#include "tomo/KDTree.hpp"
#include "tomo/misc.hpp"

#include <boost/foreach.hpp>

namespace tomo
{
  /** @brief A Compound is object is an object which consists of several primtives
   * @detail Moreover, a compound holds a KDTree structure for fast search
   * @tparam PRIMITIVE  Primitive type
   */
  template <class PRIMITIVE, unsigned DIMENSIONS, class SCALAR = DEFAULT_TYPE>
  class Compound
  {
    /// TODO: Large code blocks of inRadius and kNearest are similar, refactor it
  public:
    typedef SCALAR scalar_type;
    typedef Bounds<DIMENSIONS,SCALAR> bounds_type;
    typedef KDTree<PRIMITIVE,DIMENSIONS,SCALAR> kdtree_type;
    typedef typename kdtree_type::Node node_type;
    typedef std::multimap<SCALAR,PRIMITIVE*> map_type;
    typedef std::pair<SCALAR,PRIMITIVE*> pair_type;
    typedef std::vector<PRIMITIVE*> vector_type;

    /// Aggregate another compound to this one 
    void aggregate(const Compound& _compound, bool _update = true)
    {
      objs().reserve(_compound.objs_.size() + objs_.size());
      BOOST_FOREACH ( const PRIMITIVE& _obj , _compound.objs() )
        objs().push_back(_obj);
      if (_update) update();
    }

    /// Collects k nearest primitives relative to a primitive _p
    vector_type collectKNearest(const PRIMITIVE* _p, int _k) const
    {
      map_type _nearestPrimitives;
      vector_type _kNearestPrimitives;

      kNearest(_p,&kdTree_.root(),bounds_,_nearestPrimitives,_k);
      BOOST_FOREACH( pair_type _prim, _nearestPrimitives )
        _kNearestPrimitives.push_back(_prim.second);

      return _kNearestPrimitives;
    }
    
    /// Uses the kNearest algorithm to return the nearest object
    PRIMITIVE* nearest(const PRIMITIVE* _p) const
    {
      vector_type _nearest = collectKNearest(_p,1);
      return (_nearest.empty()) ? NULL : _nearest[0];
    }


    /// Collects all objects inside a radius r
    vector_type collectInRadius(const PRIMITIVE* _p, scalar_type _radius) const
    {
      map_type _nearestPrimitives;
      vector_type _primitivesInRadius;

      inRadius(_p,&kdTree_.root(),bounds_,_nearestPrimitives,_radius);
      BOOST_FOREACH( pair_type _prim, _nearestPrimitives )
      _primitivesInRadius.push_back(_prim.second);

      return _primitivesInRadius;
    }

    /// Calculates bounds and constructs kdtree
    void update()
    {
      calcBounds();
      kdTree_.build(objs_,bounds(),1);
    }

    TBD_PROPERTY_REF(std::vector<PRIMITIVE>,objs);
    TBD_PROPERTY_RO(bounds_type,bounds);
    TBD_PROPERTY_REF(kdtree_type,kdTree);

  protected:

    /// Calculates the distance of a primitive to a kdtree node
    scalar_type nodeDistance(const PRIMITIVE* _p, const bounds_type _bounds) const
    {
      if (overlap(_bounds,_p->bounds())) return 0.0;
      float _minDist = INF;

      TOMO_FOREACH_DIM
      {
        _minDist = std::min(std::min(std::abs(_p->center()[i] - _bounds.min()[i]),
                                     std::abs(_bounds.max()[i]- _p->center()[i])),
                            _minDist);
      }
      return _minDist;
    }

    /// Finds the k nearest primitives starting from a certain node
    void kNearest(const PRIMITIVE* _p, const node_type* _node,
                  const bounds_type& _bounds,
                  map_type& _nearestPrimitives, unsigned _k ) const
    {
      if (!_node) return;

      scalar_type _sqrRadius = (_nearestPrimitives.size() < _k) ? INF : largestValue(_nearestPrimitives);

      if (_node->isLeaf())
      {
        vector_type _primitives = _node->leaf_.primitives(kdTree_.primLists_);
        BOOST_FOREACH( PRIMITIVE* _nodePrim, _primitives )
        {
          if (_nodePrim == _p) continue;
          scalar_type _distance = _p->sqrDistance(*_nodePrim);
          if (_distance < _sqrRadius )
            _nearestPrimitives.insert(pair_type(_distance,_nodePrim));

          /// Only hold at most k nearest primitives 
          if (_nearestPrimitives.size() > _k)
            _nearestPrimitives.erase(--_nearestPrimitives.end());
        }
        return;
      }

      bounds_type _left, _right;
      _bounds.split(_node->inner_.splitPos(),_node->inner_.axis(),_left,_right);

      scalar_type _leftDist = nodeDistance(_p,_left);
      scalar_type _rightDist = nodeDistance(_p,_right);

      if (_leftDist < _rightDist)
      {
        if (_leftDist < _sqrRadius)
          kNearest(_p,kdTree_.node(_node->inner_.left()),_left,_nearestPrimitives,_k);
        if (_rightDist < _sqrRadius)
          kNearest(_p,kdTree_.node(_node->inner_.right()),_right,_nearestPrimitives,_k);
      }
      else
      {
        if (_rightDist < _sqrRadius)
          kNearest(_p,kdTree_.node(_node->inner_.right()),_right,_nearestPrimitives,_k);
        if (_leftDist < _sqrRadius)
          kNearest(_p,kdTree_.node(_node->inner_.left()),_left,_nearestPrimitives,_k);
      }

    }

    /// Finds all primitives which lie inside a given radius, starting from a certain node
    void inRadius(const PRIMITIVE* _p, const node_type* _node,
                  const bounds_type& _bounds,
                  map_type& _nearestPrimitives, scalar_type _radius ) const
    {
      if (!_node) return;

      scalar_type _sqrRadius = _radius * _radius;
      if (_node->isLeaf())
      {
        vector_type _primitives = _node->leaf_.primitives(kdTree_.primLists_);
        BOOST_FOREACH( PRIMITIVE* _nodePrim, _primitives )
        {
          if (_nodePrim == _p) continue;
          scalar_type _distance = _p->sqrDistance(*_nodePrim);
          if (_distance < _sqrRadius )
            _nearestPrimitives.insert(pair_type(_distance,_nodePrim));
        }
        return;
      }

      bounds_type _left, _right;
      _bounds.split(_node->inner_.splitPos(),_node->inner_.axis(),_left,_right);

      scalar_type _leftDist = nodeDistance(_p,_left);
      scalar_type _rightDist = nodeDistance(_p,_right);

      if (_leftDist < _rightDist)
      {
        if (_leftDist < _sqrRadius)
          inRadius(_p,kdTree_.node(_node->inner_.left()),_left,_nearestPrimitives,_radius);
        if (_rightDist < _sqrRadius)
          inRadius(_p,kdTree_.node(_node->inner_.right()),_right,_nearestPrimitives,_radius);
      }
      else
      {
        if (_rightDist < _sqrRadius)
          inRadius(_p,kdTree_.node(_node->inner_.right()),_right,_nearestPrimitives,_radius);
        if (_leftDist < _sqrRadius)
          inRadius(_p,kdTree_.node(_node->inner_.left()),_left,_nearestPrimitives,_radius);
      }
    }

    /* @brief Determine compound's bounds, O(n) complexity */
    void calcBounds()
    {
      bounds_type _bounds;
      BOOST_FOREACH ( PRIMITIVE& _obj , objs() )
        _bounds.extend(_obj.bounds());
      _bounds.validate();
      bounds_ = _bounds;
    }

    inline scalar_type largestValue(const map_type& _nearestPrimitives) const
    {
      return (--_nearestPrimitives.end())->first;
    }
  };
}


