#pragma once

namespace tomo
{
  namespace geometry
  {
    namespace aux
    {
      namespace visitor
      {
        template<
          typename KDTREE
          typename INTERSECT_FUNCTOR
          >
        struct RayTraversal 
        {
          struct State
          {
            TBD_PROPERTY(scalar_type,tFar);
            TBD_PROPERTY(scalar_type,tNear);
            TBD_PROPERTY(const Node*,node);
          };

          typedef KDTREE KDTree;

          RayTraversal(const KDTree& _kdTree)
          {
          }

          /// Root intersection
          bool root()
          {
            state_.tNear(ray_.tNear());
            state_.tFar(ray_.tFar());
            return kdTree_->bounds().intersect(_ray);
          }

          /// Inner node intersection
          bool inner(State& _stateToPush)
          {
            base::Axis k = state_.node()->inner_.axis();
            scalar_type d = (state_node()->inner_.splitPos() - ray_.org()[k]) / ray_.dir()[k];

            const Node* _front = &kdTree->node(_node->inner_.left());
            const Node* _back = &kdTree->node(_node->inner_.right());
            if (ray_.dir()[k] < 0) std::swap(_front,_back);

            if (d < state_.tNear())
            {
              state_.node(_back);
              return false;
            }
            else if (d >= state_.tFar())
            {
              state_.node(_front);
              return false;
            }
            
            _stateToPush.tNear(d);
            _stateToPush.tFar(state_.tFar());
            _stateToPush.node(_back);
            _state.node(_front);
            _state.tFar(d);
            return true;
          }

          /// Leaf node intersection
          bool leaf()
          {
            bool _found = false;
            for (ptr_vector_type::const_iterator it = state_.node()->leaf_.begin(); 
                 it != state_.node()->leaf_.end(); ++it)
            {
              _found |= INTERSECT_FUNCTOR(ray_,state_.tNear(),state_.tFar(),*_it);
            }
            return _found;
          }

          TBD_PROPERTY_REF(State,state);
          TBD_PROPERTY_REF(ray_type,ray);
        };
      }
    }
  }
}
