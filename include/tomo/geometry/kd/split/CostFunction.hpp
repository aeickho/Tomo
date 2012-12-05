#pragma once

#include <array>

namespace tomo
{
  namespace geometry
  {
    namespace kd
    {
      namespace split
      {
        template
        <
          typename BUILD_STATE,
          typename PRIM_SPLITPOS,
          typename PRIM_INTERSECT_COST,
          size_t N_BUCKETS = 8
        >
        struct CostFunction
        {
          typedef BUILD_STATE state_type;
          typedef typename state_type::primitive_type primitive_type;
          typedef typename state_type::primitive_cntr_type primitive_cntr_type;
          typedef typename state_type::node_geometry_type node_geometry_type;
          typedef typename state_type::bounds_type bounds_type;
          typedef typename state_type::scalar_type scalar_type;

          CostFunction() : primitive_(nullptr) {}

          bool operator()(state_type& _state)
          {
            // With this bucket structure, we can achieve building a good kdtree within O(n*log n)
            struct Buckets
            {
            private:
              // A bucket contains two split candidates: 1 left and 1 right
              struct Bucket
              {
                Bucket() : cost_(0) {}
                void put(scalar_type _pos, const primitive_type* _primitive)
                {
                  if (_pos >= leftExt_ && _pos <= rightExt_)
                  {
                    if (!left_.primitive() || _pos <= left_.pos() )  left_.put(_pos,_primitive);
                    if (!right_.primitive() || _pos >= right_.pos()) right_.put(_pos,_primitive);
                  }
                }

                TBD_PROPERTY_REF(SplitCandidate,left);
                TBD_PROPERTY_REF(SplitCandidate,right);
                TBD_PROPERTY(scalar_type,leftExt);
                TBD_PROPERTY(scalar_type,rightExt);
                TBD_PROPERTY_REF(scalar_type,cost);
              };

            public:
              Buckets(const bounds_type& _bounds, const base::Axis _axis)
              {
                axis_ = _axis;
                min_ = _bounds.min()[axis_];
                max_ = _bounds.max()[axis_];
                invSize_ = N_BUCKETS / (max_ - min_);
                scalar_type _bucketSize = (max_ - min_) / N_BUCKETS;
                scalar_type _bucketPos = min_;

                for (Bucket& _bucket : buckets_)
                {
                  _bucket.leftExt(_bucketPos);
                  _bucketPos += _bucketSize;
                  _bucket.rightExt(_bucketPos);
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

              const SplitCandidate* splitCandidate()
              {
                const SplitCandidate* _bestCandidate = NULL;
                scalar_type _minSplitCost = INF;
                scalar_type _overallCost = 0;

                for (Bucket& _bucket : buckets_)
                  _overallCost += _bucket.cost();

                scalar_type _rightCost = _overallCost, _leftCost = 0;

                // The minimum cost is the extent of bounds
                scalar_type _minCost = max_ - min_;
                scalar_type _cost = INF;

                for (const Bucket& _bucket : buckets_)
                {
                  if (_bucket.left().primitive())
                  {
                    _cost = splitCost(_bucket.left().pos(),_leftCost,_rightCost);
                    if (_cost < _minSplitCost)
                    {
                      _minSplitCost = _cost;
                      _bestCandidate = &_bucket.left();
                    }
                  }

                  _leftCost += _bucket.cost();
                  _rightCost -= _bucket.cost();

                  if (_bucket.right().primitive())
                  {
                    _cost = splitCost(_bucket.right().pos(),_leftCost,_rightCost);
                    if (_cost < _minSplitCost)
                    {
                      _minSplitCost = _cost;
                      _bestCandidate = &_bucket.right();
                    }
                  }
                  if (_minSplitCost < _minCost) return NULL;
                }

                return _bestCandidate;
              }

              TBD_PROPERTY(base::Axis,axis);
            private:
              std::array<Bucket,N_BUCKETS> buckets_;

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

            } _buckets(_state.nodeGeometry().bounds(),
                       _state.nodeGeometry().axis());

            /// Insert split candidates into buckets
            for ( const primitive_type* _primitive : _state.primitives() )
            {
              _buckets.insert(_primitive);
            }

            const SplitCandidate* _bucketSplitCandidate = _buckets.splitCandidate();
            if (!_bucketSplitCandidate) return false;

            _state.nodeGeometry().splitPos(_bucketSplitCandidate->pos());
            primitive_ = _bucketSplitCandidate->primitive();

            return true;
          }

          TBD_PROPERTY(const primitive_type*,primitive)

        private:
          struct SplitCandidate
          {
            SplitCandidate(scalar_type _pos = INF,
                           const primitive_type* _primitive = nullptr) :
              pos_(_pos),
              primitive_(_primitive)
            {}

            void put(scalar_type _pos, const primitive_type* _primitive)
            {
              primitive_=_primitive;
              pos_=_pos;
            }

            TBD_PROPERTY(scalar_type,pos);
            TBD_PROPERTY_RO(const primitive_type*,primitive);
          };
        };
      }
    }
  }
}
