#pragma once
#include "Visitor.hpp"

#include <tbd/property.h>
#include <map>
#include <vector>

namespace tomo
{
  namespace geometry
  {
    namespace aux
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
        struct KNearest
        {
          typedef KDTREE KDTree;
          typedef typename KDTREE::Node Node;
          typedef typename KDTREE::bounds_type bounds_type;
          typedef typename KDTREE::primitive_type primitive_type;
          typedef typename KDTREE::scalar_type scalar_type;
          typedef typename KDTREE::point_type point_type;
          typedef std::multimap<scalar_type,primitive_type*> map_type;
          typedef std::pair<scalar_type,primitive_type*> pair_type;
          typedef std::vector<primitive_type*> ptr_vector_type;

          struct State
          {
            TBD_PROPERTY(const Node*,node);
            TBD_PROPERTY_REF(bounds_type,bounds);
          };

          KNearest(KDTree* _kdTree, primitive_type* _primitive, unsigned _k = 1) :
            kdTree_(_kdTree),
            primitive_(_primitive),
            k_(_k)
          {}

          void traverseLeft(State& _state)
          {
            _state.node(kdTree_->node(state_.node()->inner_.left()));
          }

          void traverseRight(State& _state)
          {
            _state.node(kdTree_->node(state_.node()->inner_.right()));
          }

          /// Root intersection
          bool root()
          {
            nearestPrimitives_.clear();
            return true;
          }

          /// Inner node intersection
          bool inner(State& _pushedState)
          {
            bounds_type _left, _right;
            state_.bounds().split(state_.node()->inner_.splitPos(),
                                  state_.node()->inner_.axis(),
                                  _left,_right);
            scalar_type _sqrRadius = 
              (nearestPrimitives_.size() < k_) 
              ? INF 
              : (--nearestPrimitives_.end())->first; // Largest value

            scalar_type _leftDist = SQR_NODE_DISTANCE(primitive_,_left),
                        _rightDist = SQR_NODE_DISTANCE(primitive_,_right);
            bool _leftFirst = _leftDist <= _rightDist;
            bool _traverseLeft = _leftDist <= _sqrRadius;
            bool _traverseRight = _rightDist <= _sqrRadius;
            
            if (_leftFirst)
            {
              traverseLeft(state_);
              if (_traverseRight) traverseRight(_pushedState);
            } else
            {
              traverseRight(state_);
              if (_traverseLeft) traverseLeft(_pushedState);
            }
            return _traverseLeft && _traverseRight;
          }

          /// Leaf node intersection
          bool leaf()
          {
            for (auto it = state_.node()->leaf_.begin(); it != state_.node()->leaf_.end(); ++it)
            {
              primitive_type* _nodePrim = (*it);
              if (_nodePrim == primitive_) continue;
              scalar_type _distance = SQR_PRIM_DISTANCE(primitive_,_nodePrim);
              if (_distance <= sqrRadius_ )
                nearestPrimitives_.insert(pair_type(_distance,_nodePrim));

               /// Only hold at most k nearest primitives
              if (nearestPrimitives_.size() > k_)
                nearestPrimitives_.erase(--nearestPrimitives_.end());            
            } 
            return false;
          }

          TBD_PROPERTY_REF(State,state);
          TBD_PROPERTY_REF(map_type,nearestPrimitives);
          TBD_PROPERTY_RO(KDTree*,kdTree);
          TBD_PROPERTY(primitive_type*,primitive);
          TBD_PROPERTY(unsigned,k);

        private:
          scalar_type sqrRadius_;
        };
      }
    }
  }
}
