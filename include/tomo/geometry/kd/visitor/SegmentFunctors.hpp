#pragma once

#include <set>
#include "tomo/geometry/prim/Segment.hpp"

namespace tomo
{
  namespace geometry
  {
    namespace kd 
    {
      namespace visitor
      {
        struct SegmentNodeIntersectPointOnly
        {
          typedef prim::Segment Segment;
          typedef Segment::scalar_type scalar_type;
          typedef Segment::bounds_type bounds_type;
          typedef NodeGeometry<Segment::model_type> NodeGeometry;

          NodeIntersectResult operator()(const Segment& _segment,
                                         const NodeGeometry& _node)
          {
            return NodeIntersectResult(_segment[0][_node.axis()] <= _node.splitPos(),
                                       _segment[0][_node.axis()] > _node.splitPos());
          }

          template <typename NODE>
          void nodeSetup(NodeGeometry& _nodeGeometry,
                         NODE* _node, 
                         Segment*& _primitive)
          {

            base::Axis _axis = _nodeGeometry.bounds().dominantAxis();
            scalar_type _splitPos = 0.5*(_nodeGeometry.bounds().min()[_axis] + 
                                         _nodeGeometry.bounds().max()[_axis]);
 /*           
            struct SplitCandidate
            {
              SplitCandidate(const Segment* _segment, scalar_type _splitPos) :
                segment_(_segment),
                splitPos_(_splitPos),
                cost_(INF)
              {
              }

              const Segment* segment_;
              scalar_type splitPos_;
              scalar_type cost_;
            };
            
            typedef std::set<SplitCandidate> SplitCandidates;
            std::set<SplitCandidate> _splitCandidates;
            BOOST_FOREACH( const Segment* _segment, _state.primList() )
            {
              _splitCandidates.insert(SplitCandidate(_segment,_segment->p0()[_axis]));
            }
            SplitCandidates::iterator _it0 = _splitCandidates.begin(),
                                      _it1 = _it0++,
                                      _it2 = _it1++;
            int i = 0;
            for (; _it2 != _splitCandidates.end(); ++_it2)
            {
              bounds_type _left, _right;
              _state.bounds().split(_it1->splitPos_,_axis,_left,_right);
              _it1->cost_ = i*boundsLength(_left) + 
                           (_splitCandidates.size()-2-i)*boundsLength(_right);
              _it1->cost_ /= std::max(_it1->splitPos_ - _it0->splitPos_,
                                      _it2->splitPos_ - _it1->splitPos_);
              ++_it0;
              ++_it1;
              i++;
            }
*/
            _nodeGeometry.splitPos(_splitPos);
            _nodeGeometry.axis(_axis);
          }

          scalar_type boundsLength(const bounds_type& _bounds)
          {
            return 2*(_bounds.size().x() + _bounds.size().y());
          }
        };

        struct SegmentSegmentSqrDistance
        {
          typedef geometry::prim::Segment Segment;
          typedef Segment::scalar_type scalar_type;

          scalar_type operator()(const Segment& _a, const Segment& _b)
          {
            return (_a[1] - _b[0]).sqrLength(); 
          }
        };

        struct SegmentBoundsSqrDistance
        {
          typedef geometry::prim::Segment Segment;
          typedef Segment::scalar_type scalar_type;
          typedef Segment::point_type point_type;
          typedef Segment::bounds_type bounds_type;

          scalar_type operator()(const Segment& _s, const bounds_type& _b)
          {
            const point_type& _p = _s[1];
            if (_b.inside(_p)) return 0;

            scalar_type _minDist = INF;
            TOMO_FOREACH_DIM_(_p,i)
            {
              scalar_type _d = (_p[i] < _b.min()[i]) ? _b.min()[i] : _b.max()[i];
              _d -= _p[i];
              _d *= _d;
              _minDist = std::min(_d,_minDist);
            }
            return _minDist*_minDist;
          }
        };
      }
    }
  }
}
