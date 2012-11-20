#pragma once
#include "Candidate.hpp"

namespace tomo
{
  namespace geometry
  {
    namespace kd
    {
      namespace split
      {
        template <typename KDTREE,
                 typename PRIM_SPLITPOS,
                 typename PRIM_INTERSECT_COST,
                 size_t N_BUCKETS = 8>
        struct CostFunction
        {
          TOMO_INHERIT_MODEL_TYPES(KDTREE)
          typedef typename KDTREE::primitive_type primitive_type;
          typedef typename KDTREE::prim_cntr_type prim_cntr_type;
          typedef typename KDTREE::geometry_type geometry_type;
          typedef Candidate<KDTREE> candidate_type;

          bool operator()( const bounds_type& _bounds,
                           const prim_cntr_type& _primitives,
                           candidate_type& _splitCandidate)
          {
            // With this bucket structure, we can achieve building a good kdtree within O(n*log n)
            struct Buckets
            {
            private:

              // A bucket contains two split candidates: 1 left and 1 right
              struct Bucket
              {
                Bucket() : cost_(0) {}

                void put(scalar_type _pos, const primitive_type* _prim)
                {
                  if (_pos >= leftExt_ && _pos <= rightExt_)
                  {
                    if (!left_.prim() || _pos <= left_.pos() )  left_.put(_pos,_prim);
                    if (!right_.prim() || _pos >= right_.pos()) right_.put(_pos,_prim);
                  }
                }

                TBD_PROPERTY_REF(candidate_type,left);
                TBD_PROPERTY_REF(candidate_type,right);
                TBD_PROPERTY(scalar_type,leftExt);
                TBD_PROPERTY(scalar_type,rightExt);
                TBD_PROPERTY_REF(scalar_type,cost);
              };

            public:
              Buckets(const bounds_type& _bounds)
              {
                axis_ = _bounds.dominantAxis();
                min_ = _bounds.min()[axis_];
                max_ = _bounds.max()[axis_];

                invSize_ = N_BUCKETS / (max_ - min_);

                scalar_type _bucketSize = (max_ - min_) / N_BUCKETS;
                scalar_type _bucketPos = min_;
                for (int i = 0; i < N_BUCKETS; i++)
                {
                  buckets_[i].leftExt(_bucketPos);
                  _bucketPos += _bucketSize;
                  buckets_[i].rightExt(_bucketPos);
                }

                buckets_[0].leftExt(_bucketSize*0.5 + min_);
                buckets_[N_BUCKETS-1].rightExt(_bucketPos-_bucketSize*0.5);
              }

              void insert(const primitive_type* _primitive)
              {
                scalar_type _splitPos = PRIM_SPLITPOS()(_primitive,axis_);

                Bucket* _bucket = bucketBySplitPos(_splitPos);
                if (!_bucket) return;

                _bucket->cost() += PRIM_INTERSECT_COST()(_primitive);
                _bucket->put(_splitPos,_primitive);
              }

              candidate_type* splitCandidate()
              {
                candidate_type* _bestCandidate = NULL;

                scalar_type _minSplitCost = INF;
                scalar_type _overallCost = 0;
                for (int i = 0; i < N_BUCKETS; i++)
                  _overallCost += buckets_[i].cost();

                scalar_type _rightCost = _overallCost, _leftCost = 0;

                // The minimum cost is the extent of bounds
                scalar_type _minCost = max_ - min_;
                scalar_type _cost = INF;

                for (int i = 0; i < N_BUCKETS; i++)
                {
                  Bucket& _bucket = buckets_[i];
                  Candidate* _left = &_bucket.left();
                  Candidate* _right = &_bucket.right();

                  if (_left->prim())
                  {
                    _cost = splitCost(_left->pos(),_leftCost,_rightCost);

                    if (_cost < _minSplitCost)
                    {
                      _minSplitCost = _cost;
                      _bestCandidate = _left;
                    }
                  }

                  _leftCost += _bucket.cost();
                  _rightCost -= _bucket.cost();

                  if (_right->prim())
                  {
                    _cost = splitCost(_right->pos(),_leftCost,_rightCost);
                    if (_cost < _minSplitCost)
                    {
                      _minSplitCost = _cost;
                      _bestCandidate = _right;
                    }
                  }

                  if (_minSplitCost < _minCost) return NULL;
                }

                return _bestCandidate;
              }

              TBD_PROPERTY(base::Axis,axis);
            private:
              Bucket buckets_[N_BUCKETS];

              Bucket* bucketBySplitPos(scalar_type _splitPos)
              {
                int _bucketIndex = invSize_ * (_splitPos - min_);
                if (_bucketIndex < 0 || _bucketIndex >= N_BUCKETS) return nullptr;

                return &buckets_[_bucketIndex];
              }

              scalar_type splitCost(scalar_type _splitPos,
                                    scalar_type _leftCost,
                                    scalar_type _rightCost)
              {
                return ((_splitPos - min_)  * _leftCost + (max_ - _splitPos) * _rightCost);
              }

              scalar_type min_;
              scalar_type max_;
              scalar_type invSize_;

            } _buckets(_bounds);

            /// Insert split candidates into buckets
            BOOST_FOREACH( const primitive_type* _primitive, _primitives )
            {
              _buckets.insert(_primitive);
            }

            candidate_type* _bucketSplitCandidate = _buckets.splitCandidate();
            if (!_bucketSplitCandidate) return false;

            _splitCandidate = *_bucketSplitCandidate;
            _nodeGeometry.axis(_buckets.axis());
            _nodeGeometry.splitPos(_bucketSplitCandidate->pos());
            _nodeGeometry.bounds(_bounds);

            return true;
          }
        };
      }
    }
  }
}
