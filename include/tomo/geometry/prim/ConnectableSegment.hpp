#include "LineSegment.hpp"

namespace tomo
{
  namespace geometry
  {
    namespace prim
    {
      struct ConnectableSegment : LineSegment
      {
        ConnectableSegment(const point_type& _p0, const point_type& _p1) :
          LineSegment(_p0,_p1) {}

        TBD_PROPERTY(int32_t,next);
        TBD_PROPERTY(int32_t,prev);
      
        template<class ARCHIVE>
        void serialize( ARCHIVE& _ar, const unsigned int _fileVersion )
        {
          _ar & LineSegment::operator[](0);
          _ar & LineSegment::operator[](1);
          _ar & prev_;
          _ar & next_;
        }
      };
    }
  }
}

