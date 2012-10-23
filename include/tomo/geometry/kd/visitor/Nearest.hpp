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
          typedef std::vector<const primitive_type*> ptr_vector_type;

          struct State
          {
            TBD_PROPERTY(const Node*,node);
            TBD_PROPERTY_REF(bounds_type,bounds);
          };

          Nearest(KDTree* _kdTree, primitive_type* _primitive) :
            kdTree_(_kdTree),
            primitive_(_primitive)
          { 
          }

          /// Set given state's node to left subnode  
          void traverseLeft(State& _state)
          {
            _state.node(kdTree_->node(state_.node()->inner_.left()));
          }

          /// Set given state's node to right subnode  
          void traverseRight(State& _state)
          {
            _state.node(kdTree_->node(state_.node()->inner_.right()));
          }

          /// Root intersection
          bool root()
          {
            nearestPrimitive_(pair_type(INF,NULL));
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
            scalar_type _leftDist = SQR_NODE_DISTANCE(primitive_,_left),
                        _rightDist = SQR_NODE_DISTANCE(primitive_,_right);
            bool _leftFirst = _leftDist <= _rightDist;
            bool _traverseLeft = _leftDist <= nearestPrimitive_.first;
            bool _traverseRight = _rightDist <= nearestPrimitive_.first;
            
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
            /// For each primitive in leaf node, calculate distance
            /// Distance is smaller than current found nearest primitive,
            /// replace it
            for (auto it = state_.node()->leaf_.begin(); 
                 it != state_.node()->leaf_.end(); ++it)
            {
              primitive_type* _nodePrim = (*it);
              if (_nodePrim == primitive_) continue;
              scalar_type _distance = SQR_DISTANCE(primitive_,_nodePrim);
              if (_distance <= nearestPrimitive_.first )
                nearestPrimitive_(pair_type(_distance,_nodePrim));
            }
            return false;
          }


          TBD_PROPERTY_REF(State,state);
          TBD_PROPERTY_REF(pair_type,nearestPrimitive);
          TBD_PROPERTY_RO(KDTree*,kdTree);
          TBD_PROPERTY(primitive_type*,primitive);
        };
      }
    }
  }
}

