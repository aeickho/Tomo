#pragma once

namespace tomo
{
  namespace geometry
  {
    namespace kd 
    {
      namespace build
      {
        template <typename KDTREE, 
                  typename PRIM_NODE_INTERSECTION,
                  typename PRIM_SPLITPOS,
                  typename PRIM_INTERSECT_COST> 
        struct SplitCost
        {
          typedef typename KDTREE::Node NODE; 
          TOMO_NODE_TYPES(NODE); 

          bool split( const bounds_type& _bounds,
                      const cntr_type& _primitives,
                      geometry_type& _nodeGeometry)
          {
            // A split candidate holds a possible split position and a pointer to a primitive
            struct SplitCandidate
            {
              SplitCandidate(scalar_type _pos = INF, 
                            const primitive_type* _prim = nullptr) :
                pos_(_pos),
                prim_(_prim)
              {}

              void put(scalar_type _pos, const primitive_type* _prim)
              { 
                prim_=_prim;
                pos_=_pos;
              }

              TBD_PROPERTY(scalar_type,pos);
              TBD_PROPERTY_RO(const primitive_type*,prim);
            };

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

              TBD_PROPERTY_REF(SplitCandidate,left);
              TBD_PROPERTY_REF(SplitCandidate,right);
              TBD_PROPERTY(scalar_type,leftExt);
              TBD_PROPERTY(scalar_type,rightExt);
              TBD_PROPERTY_REF(scalar_type,cost);
            };
             
#define N_BUCKETS 8
            // With this bucket structure, we can achieve building a good kdtree within O(n*log n)
            struct Buckets
            {
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

              scalar_type splitCost(scalar_type _splitPos, 
                                    scalar_type _leftCost, 
                                    scalar_type _rightCost)
              {
                return ((_splitPos - min_)  * _leftCost + (max_ - _splitPos) * _rightCost);
              }

              SplitCandidate* splitCandidate()
              {
                SplitCandidate* _bestSplitCandidate = NULL;

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
                  SplitCandidate* _left = &_bucket.left();
                  SplitCandidate* _right = &_bucket.right();

                  if (_left->prim())
                  {
                    _cost = splitCost(_left->pos(),_leftCost,_rightCost);

                    if (_cost < _minSplitCost) 
                    {
                      _minSplitCost = _cost;
                      _bestSplitCandidate = _left;
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
                      _bestSplitCandidate = _right;
                    }
                  }

                  if (_minSplitCost < _minCost) return NULL;
                }

                return _bestSplitCandidate;
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

              scalar_type min_;
              scalar_type max_;
              scalar_type invSize_;

            } _buckets(_bounds);

            /// Insert split candidates into buckets
            BOOST_FOREACH( const primitive_type* _primitive, _primitives )
            {
              _buckets.insert(_primitive);
            }

            SplitCandidate* _splitCandidate = _buckets.splitCandidate();

            if (!_splitCandidate) return false;

            _nodeGeometry.axis(_buckets.axis());
            _nodeGeometry.splitPos(_splitCandidate->pos());
            _nodeGeometry.bounds(_bounds);

            return true;
          }

          intersection_type intersect( const primitive_type* _prim, const geometry_type& _nodeGeometry)
          {
            return PRIM_NODE_INTERSECTION()(*_prim,_nodeGeometry);
          }
        };
      }
    }
  }
}
