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
        template<
        typename KDTREE,
        typename SQR_DISTANCE
          >
        struct KNearest
        {
          typedef KDTREE KDTree;
          typedef typename KDTREE::Node Node;
          typedef typename KDTREE::bounds_type bounds_type;
          typedef typename KDTREE::bounds_type bounds_type;
          typedef typename KDTREE::primitive_type primitive_type;
          typedef typename KDTREE::scalar_type scalar_type;
          typedef typename KDTREE::point_type point_type;
          typedef std::multimap<scalar_type,primitive_type*> map_type;
          typedef std::pair<scalar_type,PRIMITIVE*> pair_type;
          typedef std::vector<primitive_type*> ptr_vector_type;

          struct State
          {
            TBD_PROPERTY(const Node*,node);
            TBD_PROPERTY_REF(bounds_type,bounds);
          };

          KNearest(const primitive_type* _primitive, unsigned _k = 1) :
            primitive_(_primitive),
            k_(_k)
          {
            updateRadius();
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
            nearestPrimitives_.clear();
            return ;
          }

          /// Inner node intersection
          bool inner(State& _pushedState)
          {
            bounds_type _left, _right;
            state_.bounds().split(state_.node()->inner_.splitPos(),
                                  state_.node()->inner_.axis(),
                                  _left,_right);
            scalar_type _sqrRadius = 
              (nearestPrimitives_.size() < _k) 
              ? INF 
              : (--nearestPrimitives_.end())->first; // Largest value

            scalar_type _leftDist = BOUNDS_DISTANCE(primitive_,_left),
                        _rightDist = BOUNDS_DISTANCE(primitive_,_right);
            bool _leftFirst = _leftDist <= _rightDist;
            bool _traverseLeft = _leftDist <= radius_;
            bool _traverseRight = _rightDist <= radius_;
            
            if (_leftFirst)
            {
              traverseLeft(state_,_left);
              if (_traverseRight) traverseRight(_pushedState,_right);
            } else
            {
              traverseRight(state_,_right);
              if (_traverseLeft) traverseLeft(_pushedState,_left);
            }
            return _traverseLeft && _traverseRight;
          }

          /// Leaf node intersection
          bool leaf()
          {
            ptr_vector_type _primitives = state_.node()->leaf_.primitives(kdTree_->primLists_);
            BOOST_FOREACH( PRIMITIVE* _nodePrim, _primitives )
            {
              if (_nodePrim == primitive_) continue;
              scalar_type _distance = SQR_DISTANCE(primitive_,_nodePrim);
              if (_distance <= sqrRadius_ )
                nearestPrimitives_.insert(pair_type(_distance,_nodePrim));

               /// Only hold at most k nearest primitives
              if (nearestPrimitives_.size() > _k)
                nearestPrimitives_.erase(--nearestPrimitives_.end());            
            }
            return false;
          }

          void updateRadius() 
          {
            sqrRadius_ = _radius * _radius;
          }

          TBD_PROPERTY_REF(State,state);
          TBD_PROPERTY(map_type,nearestPrimitives);
          TBD_PROPERTY(KDTree,kdTree);
          TBD_PROPERTY(const primitive_type*,primitive);
          TBD_PROPERTY(unsigned,k);

        private:
          scalar_type sqrRadius_;
        };
      }
    }
  }
}
