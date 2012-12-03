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
          typedef typename state_type::bounds_type bounds_type;

          base::Axis operator()( const bounds_type& _bounds)
          {
            return _bounds.dominantAxis();
          }
        };

      }
    }
  }
}
