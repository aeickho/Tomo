#pragma once

namespace tomo
{
  namespace geometry
  {
    namespace kd
    {
      template<typename KDTREE, typename STATE>
      struct Visitor
      {
        typedef KDTREE kdtree_type;
        typedef STATE state_type;

        Visitor(kdtree_type& _kdTree) : kdTree_(_kdTree) {}

        /// Set given state's node to left subnode
        void traverseLeft(state_type& _state)
        {
          state_.node(&kdTree_.nodes().getNode(state_.node()->inner().left()));
        }

        /// Set given state's node to right subnode
        void traverseRight(state_type& _state)
        {
          state_.node(&kdTree_.nodes().getNode(state_.node()->inner().right()));
        }

        kdtree_type& kdTree()
        {
          return kdTree_;
        }
        const kdtree_type& kdTree() const
        {
          return kdTree_;
        }

        TBD_PROPERTY_REF(state_type,state);
      private:
        kdtree_type& kdTree_;
      };

    }
  }
}
