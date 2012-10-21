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
          typename KDTREE,
          typename INTERSECT_FUNCTOR
          >
        struct RayTraversal 
        {
          typedef KDTREE KDTree;
          typedef typename KDTREE::Node Node;
          typedef typename KDTREE::bounds_type bounds_type;
          typedef typename KDTREE::primitive_type primitive_type;
          typedef typename KDTREE::scalar_type scalar_type;
          typedef typename KDTREE::point_type point_type;
          typedef typename KDTREE::ray_type ray_type;
          typedef std::vector<primitive_type*> ptr_vector_type;
          
          struct State
          {
            TBD_PROPERTY(scalar_type,tFar);
            TBD_PROPERTY(scalar_type,tNear);
            TBD_PROPERTY(const Node*,node);
          };

          RayTraversal(const KDTree& _kdTree)
          {
          }

          /// Root intersection
          bool root()
          {
            if (kdTree_->bounds().intersect(ray_))
            {
              state_.tNear(ray_.tNear());
              state_.tFar(ray_.tFar());
              return true;
            } 
            return false;
          }

          /// Inner node intersection
          bool inner(State& _stateToPush)
          {
            base::Axis k = state_.node()->inner_.axis();
            scalar_type d = (state_.node()->inner_.splitPos() - ray_.org()[k]) / ray_.dir()[k];

            const Node* _front = kdTree_->node(state_.node()->inner_.left());
            const Node* _back = kdTree_->node(state_.node()->inner_.right());
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
            state_.node(_front);
            state_.tFar(d);
            return true;
          }

          /// Leaf node intersection
          bool leaf()
          {
            bool _found = false;
            for (typename ptr_vector_type::const_iterator it = state_.node()->leaf_.begin(); 
                 it != state_.node()->leaf_.end(); ++it)
            {
              _found |= INTERSECT_FUNCTOR(ray_,state_.tNear(),state_.tFar(),*it);
            }
            return _found;
          }

          TBD_PROPERTY_REF(State,state);
          TBD_PROPERTY_RO(KDTree*,kdTree);
          TBD_PROPERTY_REF(ray_type,ray);
        };
      }
    }
  }
}
