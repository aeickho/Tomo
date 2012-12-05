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
          typedef typename state_type::node_geometry_type node_geometry_type;

          bool operator()(state_type& _state)
          {
            if (_state.primitives().size() < N_PRIMITIVES) return false;
            
            node_geometry_type& _nG = _state.nodeGeometry();
            _nG.splitPos(0.5*(_nG.bounds().min()[_nG.axis()]+_nG.bounds().max()[_nG.axis()]));
            return true;
          }
        };
      }
    }
  }
}
