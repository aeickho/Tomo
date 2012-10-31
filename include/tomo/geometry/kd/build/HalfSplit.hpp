namespace tomo
{
  namespace geometry
  {
    namespace kd 
    {
      namespace build
      {
        template <typename KDTREE, typename PRIM_NODE_INTERSECTION> 
        struct HalfSplit
        {
          typedef typename KDTREE::Node NODE; 
          TOMO_NODE_TYPES(NODE);

          bool split( const bounds_type& _bounds, 
                      const cntr_type& _primitives,
                      geometry_type& _nodeGeometry)
          {
            if (_primitives.size() < 3) return false;

            base::Axis _axis = _bounds.dominantAxis();
            _nodeGeometry.axis(_axis);
            _nodeGeometry.splitPos(0.5*(_bounds.min()[_axis] + _bounds.max()[_axis]));
            _nodeGeometry.bounds(_bounds);
            
            return true;
          }

          intersection_type intersect( const primitive_type* _prim, const geometry_type& _nodeGeometry)
          {
            return PRIM_NODE_INTERSECTION()(*_prim,_nodeGeometry);
          }
        };
      }
    }
  }
}
