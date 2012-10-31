#pragma once

namespace tomo
{
  namespace geometry
  {
    namespace kd 
    {
      namespace build
      {
        template <typename NODE, typename PRIM_NODE_INTERSECTION> 
        struct CostEstimated
        {
          TOMO_NODE_TYPES(NODE); 

          base::Axis axis()
          {

          }

          geometry_type split( const bounds_type& _bounds, const ctnr_type& _primitives )
          {
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
      }
    }
  }
}
