#pragma once

#include "RayTraversal.hpp"
#include "tomo/geometry/intersect/RaySegment.hpp"

namespace tomo
{
  namespace geometry
  {
    namespace kd
    {
      namespace visitor
      {
        struct SegmentMarkers 
        {
          typedef prim::Segment Segment;
          typedef Tree<Segment> KDTree;
          typedef typename KDTree::Node NODE;
          TOMO_NODE_TYPES(NODE);

          typedef Segment::scalar_type SegmentMarker;
          typedef std::set<SegmentMarker> SegmentMarkerContainer;
          typedef RayTraversal<KDTree,intersect::RaySegment> RayTraversal;
          typedef RayTraversal::State State;

          SegmentMarkers(const KDTree& _kdTree) :
            rayTraversal_(_kdTree)
          {
          }

          bool root()
          {
            segmentMarkers_.clear();
            return rayTraversal_.root();
          }

          bool inner(State& _stateToPush)
          {
            return rayTraversal_.inner(_stateToPush);
          }

          /// Leaf node intersection
          bool leaf()
          {
            const Node::Leaf& _leaf = rayTraversal_.state().node()->leaf_;
            for (auto it = _leaf.begin(rayTraversal_.kdTree().primLists_); 
                it != _leaf.end(rayTraversal_.kdTree().primLists_); ++it)
            {
              if (intersect::RaySegment()(
                    rayTraversal_.ray(),**it,
                    rayTraversal_.state().tNear(),
                    rayTraversal_.state().tFar())) 
                segmentMarkers_.insert(rayTraversal_.ray().tNear());
            }
            return false;
          }

          void segment(const Segment& _segment)
          {
            rayTraversal_.ray().org(_segment[0]);
            rayTraversal_.ray().dir(_segment[1] - _segment[0]);
            rayTraversal_.ray().tNear(0.0);
            rayTraversal_.ray().tFar(1.0);
          }

          TBD_PROPERTY_REF(SegmentMarkerContainer,segmentMarkers);
        private:
          RayTraversal rayTraversal_;
        };

      }
    }
  }
}
