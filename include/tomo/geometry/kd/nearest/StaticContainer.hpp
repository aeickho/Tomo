#pragma once

#include "BuildState.hpp"
#include "tomo/geometry/kd/StaticContainer.hpp"

namespace tomo
{
  namespace geometry
  {
    namespace kd
    {
      namespace nearest 
      {
        /// A StaticContainer holds nodes, primitive lists and defines how setup node attributes
        template<typename PRIMITIVE>
        struct StaticContainer : 
          kd::StaticContainer<BuildState<PRIMITIVE>>
        {
          typedef BuildState<PRIMITIVE> state_type;
          TOMO_INHERIT_STATE_TYPES(state_type)

          typedef kd::StaticContainer<state_type> base_cntr_type;
          typedef typename state_type::primitive_type primitive_type;
          typedef typename state_type::primitive_cntr_type primitive_cntr_type;

          typedef typename primitive_cntr_type::const_iterator prim_const_iterator;
          typedef typename primitive_cntr_type::iterator prim_iterator;

          void init(const state_type& _state)
          {
            // Reserve appropriate number of nodes
            base_cntr_type::nodes_.reserve(_state.primitives().size());
            base_cntr_type::init(_state);
          }

          /// Insert, setup and return an inner node
          inner_node_type& insertInner(const state_type& _state)
          {
            base_cntr_type::insertInner(_state);
            
            // Each node has a primitive as attribute, get it from state
            node_type& _node = base_cntr_type::nodes_[_state.nodeIndex()];
            _node.attributes().primitive(_state.primitive());
 
            uint32_t _index = base_cntr_type::nodes_.size();
            inner_node_type& _inner =
              _node.inner(_index,
                          _state.nodeGeometry().axis(),
                          _state.nodeGeometry().splitPos(),
                          inner_attr_type() ); 
            return _inner;
          }

          /// Insert, setup and return a leaf node
          leaf_node_type& insertLeaf(const state_type& _state)
          {
            leaf_node_type& _leaf = base_cntr_type::insertLeaf(_state);

            // Each node has a primitive as attribute, get it from state
            node_type& _node = base_cntr_type::nodes_.back();
            _node.attributes().primitive(_state.primitive());

            // Each leaf has a range of primitives as attributes, 
            // get it from primitive container
            _leaf.attributes().begin(primitives_.size());
            _leaf.attributes().end(primitives_.size()+_state.primitives().size());
            
            // Insert state's primitives into primitive container
            primitives_.insert(primitives_.end(),
                               _state.primitives().begin(),
                               _state.primitives().end());
            return _leaf;
          }

          void leafRange(const leaf_node_type& _leaf, 
                         prim_const_iterator& _begin,
                         prim_const_iterator& _end) const
          {
            _begin = primitives_.begin() + _leaf.attributes().begin();
            _end = primitives_.begin() + _leaf.attributes().end();
          }
          
        private:
          /// Holds a list of primitives for each leaf
          primitive_cntr_type primitives_;
        };
      }
    }
  }
}

