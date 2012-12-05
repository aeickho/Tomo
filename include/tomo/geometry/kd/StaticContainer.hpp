#pragma once

namespace tomo
{
  namespace geometry
  {
    namespace kd
    {
      template<typename BUILD_STATE>
      struct StaticContainer
      {
        typedef BUILD_STATE state_type;
        TOMO_INHERIT_STATE_TYPES(state_type)

        typedef std::vector<node_type> node_cntr_type;

        bool empty() const
        {
          return nodes_.empty();
        }

        void clear()
        {
          nodes_.clear();
        }

        void init(const state_type& _state)
        {
          nodes_.resize(1);
        }

        inner_node_type& insertInner(const state_type& _state)
        {
            nodes_.resize(nodes_.size()+2);
        }

        leaf_node_type& insertLeaf(const state_type& _state)
        {
            nodes_.resize(nodes_.size()+1);
        }

        const node_type& getRoot() const
        {
          return nodes_[0];
        }

        node_type& getRoot()
        {
          return nodes_[0];
        }

        const node_type& getNode(uint32_t _nodeIndex) const
        {
          return nodes_[_nodeIndex];
        }

        node_type& getNode(uint32_t _nodeIndex)
        {
          return nodes_[_nodeIndex];
        }

      protected:
        node_cntr_type nodes_;
      };
    }
  }
}
