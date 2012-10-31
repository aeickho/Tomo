namespace tomo
{
  namespace geometry
  {
    namespace kd 
    {
      namespace build
      {
        template <typename NODE, typename PRIM_NODE_INTERSECTION> 
        struct BuildPolicy
        {
          TOMO_NODE_TYPES(NODE);

          constexpr size_t maxSplitCost()
          {
            return 8;
          }

          scalar_type split( const bounds_type& _bounds, 
                             const cntr_type& _primitives,
                             geometry_type& _nodeGeometry)
          {
          }

          intersection_type intersect( const primitive_type* _prim, const geometry_type& _nodeGeometry)
          {
          }
        };
      }
    }
  }
}

