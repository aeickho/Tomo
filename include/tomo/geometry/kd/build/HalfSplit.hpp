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
          TOMO_INHERIT_MODEL_TYPES(KDTREE);
          typedef typename KDTREE::node_type node_type;
          typedef typename KDTREE::geometry_type geometry_type;
          typedef typename KDTREE::primitive_type primitive_type;
          typedef typename KDTREE::prim_cntr_type prim_cntr_type;

          bool split( const bounds_type& _bounds, 
                      const prim_cntr_type& _primitives,
                      geometry_type& _nodeGeometry)
          {
            if (_primitives.size() < 3) return false;

            base::Axis _axis = _bounds.dominantAxis();
            _nodeGeometry.axis(_axis);
            _nodeGeometry.splitPos(0.5*(_bounds.min()[_axis] + _bounds.max()[_axis]));
            _nodeGeometry.bounds(_bounds);
            
            return true;
          }

          void nodeAttributes(Node& _node)
          {
          }

          NodeIntersectResult intersect( const primitive_type* _prim, const geometry_type& _nodeGeometry)
          {
            return PRIM_NODE_INTERSECTION()(*_prim,_nodeGeometry);
          }
        };
      }
    }
  }
}
