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
        typename KDTREE,
        typename SQR_DISTANCE,
        typename BOUNDS_DISTANCE
          >
        struct Nearest
        {
          typedef KDTREE KDTree;
          typedef typename KDTREE::Node Node;
          typedef typename KDTREE::bounds_type bounds_type;
          typedef typename KDTREE::bounds_type bounds_type;
          typedef typename KDTREE::primitive_type primitive_type;
          typedef typename KDTREE::scalar_type scalar_type;
          typedef typename KDTREE::point_type point_type;
          typedef std::pair<scalar_type,PRIMITIVE*> pair_type;
          typedef std::vector<primitive_type*> ptr_vector_type;

          struct State
          {
            TBD_PROPERTY(const Node*,node);
            TBD_PROPERTY_REF(bounds_type,bounds);
          };

          Nearest(const primitive_type* _primitive) :
            primitive_(_primitive)
          {
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

            scalar_type _leftDist = BOUNDS_DISTANCE(primitive_,_left),
                        _rightDist = BOUNDS_DISTANCE(primitive_,_right);
            _leftDist *= _leftDist;
            _rightDist *= _rightDist;
            bool _leftFirst = _leftDist <= _rightDist;
            bool _traverseLeft = _leftDist <= nearestPrimitive_.first;
            bool _traverseRight = _rightDist <= nearestPrimitive_.first;
            
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
            for (ptr_vector_type::const_iterator it = state_.node()->leaf_.begin(); 
                 it != state_.node()->leaf_.end(); ++it)
            {
              const Primitive* _nodePrim = (*it);
              if (_nodePrim == primitive_) continue;
              scalar_type _distance = SQR_DISTANCE(primitive_,_nodePrim);
              if (_distance <= nearestPrimitive_.first )
                nearestPrimitive_(pair_type(_distance,_nodePrim));
            }
            return false;
          }


          TBD_PROPERTY_REF(State,state);
          TBD_PROPERTY_REF(pair_type,nearestPrimitive);
          TBD_PROPERTY(const KDTree*,kdTree);
          TBD_PROPERTY(const primitive_type*,primitive);
        };
      }
    }
  }
}

