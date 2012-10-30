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
        template<
          /// KDTree type template parameter
        typename KDTREE,
          /// Functor which determines the distance between two primitives   
        typename SQR_PRIM_DISTANCE,
          /// Functor which determines the distance between a primtive and a node
        typename SQR_NODE_DISTANCE
          >
        struct Nearest
        {
          typedef KDTREE KDTree;
          typedef typename KDTREE::Node Node;
          typedef typename KDTREE::bounds_type bounds_type;
          typedef typename KDTREE::primitive_type primitive_type;
          typedef typename KDTREE::scalar_type scalar_type;
          typedef typename KDTREE::point_type point_type;
          typedef std::pair<scalar_type,primitive_type*> pair_type;

          struct State
          {
            TBD_PROPERTY(const Node*,node);
            TBD_PROPERTY_REF(bounds_type,bounds);
          };

          Nearest(KDTree& _kdTree) :
            primitive_(NULL),
            kdTree_(_kdTree)
          {
            state_.node(_kdTree.root());
            state_.bounds(_kdTree.bounds_);
          }

          /// Set given state's node to left subnode  
          void traverseLeft(State& _state)
          {
            _state.node(kdTree_.node(state_.node()->inner_.left()));
          }

          /// Set given state's node to right subnode  
          void traverseRight(State& _state)
          {
            _state.node(kdTree_.node(state_.node()->inner_.right()));
          }

          /// Root intersection
          bool root()
          {
            TOMO_ASSERT(primitive_);
            nearest_ = pair_type(INF,NULL);
            return true;
          }

          /// Inner node intersection
          bool inner(State& _pushedState)
          {
            bounds_type _left, _right;

            /// Split current node's bounds in to a left and right one
            state_.bounds().split(state_.node()->inner_.splitPos(),
                                  state_.node()->inner_.axis(),
                                  _left,_right);

            /// Calculate primitives squared distance to node's bounds using template functor
            SQR_NODE_DISTANCE _sqrNodeDistance;
            scalar_type _leftDist = _sqrNodeDistance(*primitive_,_left),
                        _rightDist = _sqrNodeDistance(*primitive_,_right);
            bool _leftFirst = _leftDist <= _rightDist;
            bool _traverseLeft = _leftDist <= nearest_.first;
            bool _traverseRight = _rightDist <= nearest_.first;
            
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
            /// For each primitive in leaf node, calculate distance
            /// Distance is smaller than current found nearest primitive,
            /// replace it
            for (auto it = state_.node()->leaf_.begin(kdTree_.primLists_); 
                 it != state_.node()->leaf_.end(kdTree_.primLists_); ++it)
            {
              if (*it == primitive_ || *it == NULL) continue;
              
              scalar_type _distance = SQR_PRIM_DISTANCE()(*primitive_,**it);
              if (_distance <= nearest_.first )
                nearest_ = pair_type(_distance,const_cast<primitive_type*>(*it));
            }
            return false;
          }

          primitive_type* find(primitive_type* _primitive) 
          {
            primitive_ = _primitive;
            kdTree_.template traversal<>(*this);
            return nearest_.second;
          }

          TBD_PROPERTY_REF(State,state);
          TBD_PROPERTY_REF(pair_type,nearest);
          TBD_PROPERTY(primitive_type*,primitive);
        private:
          KDTree& kdTree_;
        };
      }
    }
  }
}

