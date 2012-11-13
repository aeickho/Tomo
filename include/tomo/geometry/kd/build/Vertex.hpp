#pragma once
#include "SplitCost.hpp"
#include "tomo/geometry/intersect/VertexNode.hpp"

namespace tomo
{
  namespace geometry
  {
    namespace kd
    {
      namespace build
      {
        namespace detail
        {
          template <typename MODEL>
          struct VertexSplitPos
          {
            typedef prim::Vertex<MODEL> Vertex;
            typedef typename Vertex::scalar_type scalar_type;
            scalar_type operator()(const Vertex* _v, base::Axis _axis)
            {
              return _v->v()[_axis];
            }
          };

          template <typename MODEL>
          struct VertexIntersectionCost
          {
            typedef prim::Vertex<MODEL> Vertex;
            typedef typename Vertex::scalar_type scalar_type;
            scalar_type operator()(const Vertex* _v)
            {
              return 0.5;
            }
          };

          template <typename MODEL>
          struct VertexAttribute
          {
            TBD_PROPERTY(const prim::Vertex<MODEL>*,vertex);
          };

          template <typename MODEL>
          struct VertexNode : NodeConcept< VertexAttribute<MODEL> >
          {};
        }

        template <typename MODEL>
        struct VertexTree : Tree<prim::Vertex<MODEL>,detail::VertexNode<MODEL>,32>
        {};

        template <typename MODEL>
        struct VertexTreeBuildPolicy : SplitCost<
            VertexTree<MODEL>,
            detail::VertexSplitPos<MODEL>,
            detail::VertexIntersectionCost<MODEL> >
        {
          typedef VertexTree<MODEL> VertexTree;
          typedef typename VertexTree::node_type node_type;
          typedef typename VertexTree::primitive_type Vertex;
          typedef typename VertexTree::geometry_type geometry_type;

          void nodeAttributes(node_type& _node)
          {
            _node.vertex(vertex_);
          }

          NodeIntersectResult intersect( const Vertex* _vertex, const geometry_type& _nodeGeometry)
          {
            if (vertex_ != NULL && vertex_==_vertex) return NodeIntersectResult(false,false);

            return intersect::VertexNode<MODEL>()(*_vertex,_nodeGeometry);
          }
        private:
          const Vertex* vertex_;
        };
      }
    }
  }
}
