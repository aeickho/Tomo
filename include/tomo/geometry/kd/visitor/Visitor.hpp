#pragma once

namespace tomo
{
  namespace geometry
  {
    namespace kd
    {
      namespace visitor
      {
        template<typename KDTREE> 
        struct Visitor
        {
          typedef KDTREE KDTree;
          typedef typename KDTree::Node Node;
          typedef typename KDTree::bounds_type bounds_type;
          typedef typename KDTree::primitive_type primitive_type;
          typedef typename KDTree::scalar_type scalar_type;
          typedef typename KDTree::point_type point_type;
        };
      }
    }
  }
}
