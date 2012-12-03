#pragma once

namespace tomo
{
  namespace geometry
  {
    namespace kd
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
          typedef typename KDTREE::Node NODE;
          TOMO_NODE_TYPES(NODE);
          typedef base::Ray< model_type > ray_type;

          RayTraversal(const KDTree& _kdTree) :
            kdTree_(_kdTree)
          {
          }

          struct State
          {
            TBD_PROPERTY(scalar_type,tFar);
            TBD_PROPERTY(scalar_type,tNear);
            TBD_PROPERTY(const Node*,node);
          };

          /// Root intersection
          bool root()
          {
            if (kdTree_.bounds_.intersect(ray_))
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

            const Node* _front = kdTree_.node(state_.node()->inner_.left());
            const Node* _back = kdTree_.node(state_.node()->inner_.right());
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
            for (auto it = state_.node()->leaf_.begin(kdTree_.primLists_); 
                it != state_.node()->leaf_.end(kdTree_.primLists_); ++it)
            {
              _found |= INTERSECT_FUNCTOR(ray_,state_.tNear(),state_.tFar(),*it);
            }
            return _found;
          }

          TBD_PROPERTY_REF(State,state);
          TBD_PROPERTY_RO(const KDTree&,kdTree);
          TBD_PROPERTY_REF(ray_type,ray);
        };
      }
    }
  }
}
