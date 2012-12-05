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

        inner_node_type& insertInner(
          const state_type& _state,
          attr_type _attributes,
          inner_attr_type _innerAttributes)
        {
            node_type& _node = nodes_[_state.nodeIndex()];
            _node.attributes(_attributes);
            uint32_t _index = nodes_.size();
            inner_node_type& _inner =
            _node.inner(_index,
                        _state.nodeGeometry().axis(),
                        _state.nodeGeometry().splitPos(),
                        _innerAttributes );
            nodes_.resize(nodes_.size()+2);
            return _inner;

        }

        leaf_node_type& insertLeaf(
            const state_type& _state,
            attr_type _attributes,
            leaf_attr_type _leafAttributes)
        {
          nodes_.resize(nodes_.size()+1);
          return nodes_.back().leaf();
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

      private:
        std::vector<node_type> nodes_;
      };
    }
  }
}
