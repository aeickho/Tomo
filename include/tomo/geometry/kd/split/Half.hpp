#pragma once

namespace tomo
{
  namespace geometry
  {
    namespace kd 
    {
      namespace split 
      {
        template <
          typename BUILD_STATE, 
          size_t N_PRIMITIVES = 3> 
        struct Half
        {
          typedef BUILD_STATE state_type;
          typedef typename state_type::primitive_type primitive_type;
          typedef typename state_type::prim_cntr_type prim_cntr_type;
          typedef typename state_type::node_geometry_type node_geometry_type;
          typedef typename state_type::bounds_type bounds_type;
          typedef typename state_type::scalar_type scalar_type;

          bool operator()( const bounds_type& _bounds, 
                           const prim_cntr_type& _primitives,
                           const base::Axis _axis,
                           scalar_type& _splitPos)
          {
            if (_primitives.size() < N_PRIMITIVES) return false;

            _splitPos = 0.5*(_bounds.min()[_axis] + _bounds.max()[_axis]);
            return true;
          }
        };
      }
    }
  }
}
