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
        /// A StaticContainer holds nodes, primitive lists and defines how setup node attributes
        template<typename BUILD_STATE>
        struct StaticContainer : 
          kd::StaticContainer<BUILD_STATE>
        {
          typedef BUILD_STATE state_type;
          TOMO_INHERIT_STATE_TYPES(state_type)

          typedef kd::StaticContainer<state_type> base_cntr_type;

          /// An object::StaticContainer stores primitive pointers, hence the typedefs
          typedef typename state_type::primitive_type primitive_type;
          typedef typename state_type::primitive_cntr_type primitive_cntr_type;

          void init(const state_type& _state)
          {
            // Reserve appropriate number of nodes
            base_cntr_type::nodes_.reserve(_state.primitives().size());
            base_cntr_type::init(_state);
          }

          /// Insert, setup and return an inner node
          inner_node_type& insertInner(const state_type& _state)
          {
            inner_node_type& _inner =  base_cntr_type::insertInner(_state);
            uint32_t _index = base_cntr_type::nodes_.size();
            _inner.setup(_index,
                         _state.nodeGeometry().axis(),
                         _state.nodeGeometry().splitPos()); 
            return _inner;
          }

          /// Insert, setup and return a leaf node
          leaf_node_type& insertLeaf(const state_type& _state)
          {
            leaf_node_type& _leaf = base_cntr_type::insertLeaf(_state);

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

        private:
          /// Holds a list of primitives for each leaf
          primitive_cntr_type primitives_;
        };
      }
    }
  }
}
