#pragma once

namespace tomo
{
  namespace geometry
  {
    namespace kd
    {
      template 
      <
        typename STATE,
        typename SPLITAXIS,
        typename SPLITPOS
      >
      struct BuildPolicy
      {
        typedef STATE state_type;
        typedef typename STATE::node_type node_type;
        typedef typename node_type::geometry_type node_geometry_type;
        typedef typename node_type::attr_type attr_type;
        typedef typename node_type::Inner::attr_type inner_attr_type;
        typedef typename node_type::Leaf::attr_type leaf_attr_type;

        /// Split function
        bool operator()( const state_type& _state,
                         node_geometry_type& _nodeGeometry)
        {
        }
      };
    }
  }
}

