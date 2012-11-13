#pragma once

namespace tomo
{
  namespace geometry
  {
    namespace algorithms
    {
      template<typename RING, typename FUNCTOR>
      struct RingSection
      {
        typedef typename RING Ring;
        typedef typename Ring::iterator iterator;
        typedef typename Ring::const_iterator const_iterator;

        void operator()(
            const Ring& _ring, 
            const_iterator _it0, 
            const_iterator _it1,
            FUNCTOR _functor)
        {
          scalar_type _length = boost::geometry::model::linestring<point_type>(_ring.begin(),_ring.end());

          struct LengthFunctor
          {
            LengthFunctor() : length_(0) {}
            
            void operator(const_iterator it)
            {
              length_ += length(Segment(*it,*(++it)));
            }

            TBD_PROPERTY(scalar_type,length)
          } _functor;
          
          RingForEach<>()(_ring,_it0,_it1,_functor);

          struct SegmentFunctor
          {
            void operator(const_iterator it)
            {

            }
            TBD_PROPERTY_REF(std::vector<Segment>,segments)
          };
          if (_length - _functor.length() > _functor.length())
          {
            RingForEach<>()(_ring,_it0,_it1,_functor);
          }
        }
      };
    }
  }
}
