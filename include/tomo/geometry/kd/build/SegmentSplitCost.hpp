namespace tomo
{
  namespace geometry
  {
    namespace kd 
    {
      namespace build
      {
        struct SegmentSplitPos
        {
          typedef tomo::geometry::prim::Segment Segment;
          typedef Segment::scalar_type scalar_type;

          scalar_type operator()(const Segment* _segment, base::Axis _axis)
          {
            return _segment->p0()[_axis];
          }
        };

        struct SegmentIntersectionCost
        {
          typedef tomo::geometry::prim::Vertex2f Vertex;
          typedef Vertex::scalar_type scalar_type;
          scalar_type operator()(const Vertex* _v)
          {
            return 0.25;
          }
        };

        struct SegmentSplitCost : 
          SplitCost<
            Tree<prim::Segment>,
            intersect::SegmentNode,
            SegmentSplitPos,
            SegmentSplitCost
          >
        {
          typedef Tree<prim::Segment> KDTree;
          SegmentSplitCost(const KDTree& _kdTree)
          {
            TOMO_NOT_IMPLEMENTED();
          }
        };
      }
    }
  }
}
