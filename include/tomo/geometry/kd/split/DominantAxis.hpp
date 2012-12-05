#pragma once

namespace tomo
{
  namespace geometry
  {
    namespace kd 
    {
      namespace split 
      {
        template <typename BUILD_STATE>
        struct DominantAxis
        {
          typedef BUILD_STATE state_type;          

          void operator()(state_type& _state)
          {
            _state.nodeGeometry().axis(_state.nodeGeometry().bounds().dominantAxis());
          }
        };
      }
    }
  }
}
