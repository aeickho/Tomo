namespace tomo
{
  namespace geometry
  {
    namespace kd 
    {
      namespace build
      {
        template <
          typename KDTREE, 
          typename PRIM_NODE_INTERSECTION,
          size_t N_PRIMITIVES = 3> 
        struct Half
        {
          TOMO_INHERIT_MODEL_TYPES(KDTREE);
          typedef typename KDTREE::node_type node_type;
          typedef typename KDTREE::geometry_type geometry_type;
          typedef typename KDTREE::primitive_type primitive_type;
          typedef typename KDTREE::prim_cntr_type prim_cntr_type;

          bool operator()( const bounds_type& _bounds, 
                           const prim_cntr_type& _primitives,
                           Candidate& _splitCandidate)
          {
            if (_primitives.size() < N_PRIMITIVES) return false;

            base::Axis _axis = _bounds.dominantAxis();
            _nodeGeometry.axis(_axis);
            _nodeGeometry.splitPos(0.5*(_bounds.min()[_axis] + _bounds.max()[_axis]));
            _nodeGeometry.bounds(_bounds);
            
            return true;
          }
        };
      }
    }
  }
}
