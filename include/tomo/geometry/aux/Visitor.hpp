#pragma once

namespace tomo
{
  namespace geometry
  {
    namespace aux
    {
      template<typename KDTREE, typename STATE> 
      struct KDTreeVisitor
      {
        typedef STATE State;
        typedef KDTREE KDTree;
        
        KDTreeVisitor(const KDTree& _kdTree) 
        {
        }

        /// Root intersection
        bool root() { return true; }

        /// Inner node intersection
        bool inner(State& _nextState) { return true; }

        /// Leaf node intersection
        bool leaf() { return false; }

      };

      template<typename KDTREE> 
      struct KDTreeState
      {
        typedef typename KDTREE::Node Node;
        typedef typename KDTREE::bounds_type bounds_type;

        TBD_PROPERTY(const Node*,node);
      };
    }
  }
}
