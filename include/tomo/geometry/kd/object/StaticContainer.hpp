#pragma once

#include "tomo/geometry/kd/StaticContainer.hpp"

namespace tomo
{
  namespace geometry
  {
    namespace kd
    {
      namespace object
      {
        template<typename BUILD_STATE>
        struct StaticContainer : kd::StaticContainer<BUILD_STATE>
        {
          typedef BUILD_STATE state_type;
          typedef kd::StaticContainer<state_type> base_cntr_type;
          TOMO_INHERIT_STATE_TYPES(state_type)

          typedef typename BUILD_STATE::primitive_type primitive_type;
          typedef typename BUILD_STATE::primitive_cntr_type primitive_cntr_type;

          void init(const state_type& _state)
          {
            base_cntr_type::nodes_.reserve(_state.primitives().size());
            base_cntr_type::init(_state);
          }

          inner_node_type& insertInner(const state_type& _state)
          {
            base_cntr_type::insertInner(_state);
            node_type& _node = base_cntr_type::nodes_[_state.nodeIndex()];
            uint32_t _index = base_cntr_type::nodes_.size();
            inner_node_type& _inner =
              _node.inner(_index,
                          _state.nodeGeometry().axis(),
                          _state.nodeGeometry().splitPos(),
                          inner_attr_type() ); 
            return _inner;

          }

          leaf_node_type& insertLeaf(const state_type& _state)
          {
            base_cntr_type::insertLeaf(_state);
            leaf_node_type& _leaf = base_cntr_type::nodes_.back().leaf();
            _leaf.attributes().begin(primitives_.size());
            _leaf.attributes().end(primitives_.size()+_state.primitives().size());
            primitives_.insert(primitives_.end(),
                               _state.primitives().begin(),
                               _state.primitives().end());
            return _leaf;
          }

        private:
          primitive_cntr_type primitives_;
        };
      }
    }
  }
}
