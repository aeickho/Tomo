#pragma once
#include "Visitor.hpp"

#include <tbd/property.h>
#include <map>
#include <vector>

namespace tomo
{
  namespace geometry
  {
    namespace kd
    {
      namespace visitor
      {
        /// Collects k nearest primitives relative to a primitive _p
        template
        <
        typename KDTREE,
                 typename SQR_PRIM_DISTANCE,
                 typename SQR_NODE_DISTANCE
                 >
        struct KNearest : Visitor<KDTREE>
        {
          typedef KDTREE KDTree;
          typedef typename KDTree::Node Node;
          typedef typename KDTree::bounds_type bounds_type;
          typedef typename KDTree::primitive_type primitive_type;
          typedef typename KDTree::scalar_type scalar_type;
          typedef typename KDTree::point_type point_type;
          typedef std::multimap<scalar_type,const primitive_type*> map_type;
          typedef std::pair<scalar_type,const primitive_type*> pair_type;

          struct State
          {
            TBD_PROPERTY_REF(bounds_type,bounds);
            TBD_PROPERTY(const Node*,node);
          };

          KNearest(const KDTree& _kdTree, const primitive_type& _primitive, unsigned _k = 1) :
            primitive_(_primitive),
            k_(_k),
            kdTree_(_kdTree)
          {
            state_.node(_kdTree.root());
            state_.bounds(_kdTree.bounds_);
            sqrRadius_ = INF;
          }

          void traverseLeft(State& _state)
          {
            _state.node(kdTree_.node(state_.node()->inner_.left()));
          }

          void traverseRight(State& _state)
          {
            _state.node(kdTree_.node(state_.node()->inner_.right()));
          }

          /// Root intersection
          bool root()
          {
            nearest_.clear();
            return true;
          }

          /// Inner node intersection
          bool inner(State& _pushedState)
          {
            bounds_type _left, _right;
            state_.bounds().split(state_.node()->inner_.splitPos(),
                                  state_.node()->inner_.axis(),
                                  _left,_right);
            sqrRadius_ =
              (nearest_.size() < k_)
              ? INF
              : (--nearest_.end())->first; // Largest value

            SQR_NODE_DISTANCE _sqrNodeDistance;
            scalar_type _leftDist = _sqrNodeDistance(primitive_,_left),
                        _rightDist = _sqrNodeDistance(primitive_,_right);

            bool _leftFirst = _leftDist <= _rightDist;
            bool _traverseLeft = _leftDist <= sqrRadius_;
            bool _traverseRight = _rightDist <= sqrRadius_;

            if (_leftFirst)
            {
              if (_traverseRight)
              {
                traverseRight(_pushedState);
                _pushedState.bounds(_right);
              }
              traverseLeft(state_);
              state_.bounds(_left);
              return _traverseRight;
            }
            else
            {
              if (_traverseLeft)
              {
                traverseLeft(_pushedState);
                _pushedState.bounds(_left);
              }
              traverseRight(state_);
              state_.bounds(_right);
              return _traverseLeft;
            }
            return false;
          }

          /// Leaf node intersection
          bool leaf()
          {
            for (auto it = state_.node()->leaf_.begin(kdTree_.primLists_);
                 it != state_.node()->leaf_.end(kdTree_.primLists_); ++it)
            {
              const primitive_type& _nodePrim = *(*it);
              if (&_nodePrim == &primitive_) continue;
              scalar_type _distance = SQR_PRIM_DISTANCE()(primitive_,_nodePrim);
              if (_distance <= sqrRadius_ )
                nearest_.insert(pair_type(_distance,&_nodePrim));

              /// Only hold at most k nearest primitives
              if (nearest_.size() > k_)
                nearest_.erase(--nearest_.end());
            }
            return false;
          }

          TBD_PROPERTY_REF(State,state);
          TBD_PROPERTY_REF(map_type,nearest);
          TBD_PROPERTY_RO(const primitive_type&,primitive);
          TBD_PROPERTY(unsigned,k);

        private:
          const KDTree& kdTree_;
          scalar_type sqrRadius_;
        };
      }
    }
  }
}
