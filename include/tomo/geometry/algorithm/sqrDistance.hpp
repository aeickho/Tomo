#pragma once

namespace tomo
{
  namespace geometry
  {
    namespace kd
    {
      namespace algorithm
      {
        template<typename A, typename B, typename RESULT = typename A::scalar_type>
        RESULT sqrDistance(const A& _a, const B& _b)
        {
          return INF;
        }

        using base::Point;
        using base::Bounds;
        
        template<typename MODEL>
        typename Point<MODEL>::scalar_type sqrDistance(const Point<MODEL>& _a, const Bounds<MODEL>& _b)
        {
          if (_b.inside(_a)) return 0.0;

          typedef typename Point<MODEL>::scalar_type scalar_type;
          
          scalar_type _minDist = INF;  
          TOMO_FOREACH_DIM_(_a,i)
          {
            scalar_type _d = (_a[i] < _b.min()[i]) ? _b.min()[i] : _b.max()[i];
            _d -= _a[i];
            _minDist = std::min(_d*_d,_minDist);
          }
          return _minDist;
        }

        template<typename MODEL>
        typename Point<MODEL>::scalar_type 
          sqrDistance(const Point<MODEL>& _a, 
                      const Point<MODEL>& _b)
        {
          return (_a - _b).sqrLength();
        }

        using prim::Vertex;
        
        template<typename MODEL>
        typename Vertex<MODEL>::scalar_type sqrDistance(const Vertex<MODEL>& _a, const Vertex<MODEL>& _b)
        {
          return sqrDistance<MODEL>(_a.v(),_b.v());
        }

        template<typename MODEL>
        typename Vertex<MODEL>::scalar_type sqrDistance(
                  const Vertex<MODEL>& _a, 
                  const typename Vertex<MODEL>::bounds_type& _b)
        {
          return sqrDistance<MODEL>(_a.v(),_b);
        }

        using prim::Segment;

        template<>
        Segment::scalar_type sqrDistance(const Segment& _a, const Segment& _b)
        {
          return sqrDistance(_a[1],_b[0]);
        }

        template<>
        Segment::scalar_type sqrDistance(const Segment& _a, const Segment::bounds_type& _b)
        {
          return sqrDistance(_a[0],_b);
        }
      }
    }
  }
}

