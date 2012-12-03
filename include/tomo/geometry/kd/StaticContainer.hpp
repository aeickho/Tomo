#pragma once

namespace tomo
{
  namespace geometry
  {
    namespace kd
    {
      namespace node
      {
        template<typename BUILD_STATE>
        struct StaticNodeContainer
        {
          typedef BUILD_STATE state_type;
          typedef typename state_type::node_type node_type;
          typedef typename node_type::attr_type attr_type;
          typedef typename node_type::Inner inner_node_type;
          typedef typename inner_node_type::attr_type inner_attr_type;
          typedef typename node_type::Leaf leaf_node_type;
          typedef typename leaf_node_type::attr_type leaf_attr_type;
          
          typedef typename node_type::geometry_type geometry_type;

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
          }

          inner_node_type& insertInner(
              const state_type& _state,
              const geometry_type& _nodeGeometry,
              attr_type _attributes = attr_type(),
              inner_attr_type _innerAttributes = inner_attr_type())
          {
            node_type& _node = nodes_[_state.nodeIndex()];
            _node.attributes(_attributes);
            uint32_t _index = nodes_.size();
            inner_node_type& _inner = 
              _node.inner(_index,_nodeGeometry.axis(),_nodeGeometry.splitPos(),_innerAttributes );            
            nodes_.resize(nodes_.size()+2);
            return _inner;
          }

          leaf_node_type& insertLeaf(const state_type& _state)
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
}