#pragma once

#include "ForEachRing.hpp"

namespace tomo
{
  namespace geometry
  {
    namespace algorithm
    {
      /// Chop takes a primitive with rings as input and splits it into a MultiLineString      
      template<typename RING>
      struct Chop
      {};

      template<>
      struct Chop<prim::Ring>
      {
        Chop<prim::Ring>(int _number = 8) : number_(_number) {}

        typedef prim::Ring Ring;
        typedef prim::MultiLineString MultiLineString;
        typedef prim::LineString LineString;
        typedef typename Ring::scalar_type scalar_type;
        typedef typename Ring::point_type point_type;
        typedef typename Ring::vec_type vec_type;
        MultiLineString operator()(const Ring& _ring)
        {
          TOMO_ASSERT(_ring.correct());
          MultiLineString _multiLineString;
          scalar_type _ringLength = _ring.length();
          scalar_type _lineStringLength = _ringLength / (number());

          LineString _lineString;
          scalar_type _length = 0;
          point_type _p = _ring.front(), _p0, _p1;
          Ring::const_iterator it = _ring.begin();

          for(;;)
          {
            if (it+1 == _ring.end())
            {
              _p0 = *it; _p1 = _ring.front();
              break;
            } else
            {
              _p0 = *it; _p1 = *(it+1);
            }
            vec_type _d = (_p1 - _p0).normalized();
/*
            while ((_p1 - _p0).length() + _length < _lineStringLength)
            {
              _p0 += _d * _lineStringLength;
              _lineString.push_back(_p0);
              _multiLineString.push_back(_lineString);
              _lineString.clear();
              _lineString.push_back(_p0);
              std::cout << (_p1-_p0).length() << " " << _length << " " << _lineStringLength << std::endl;
            }
*/
            _lineString.push_back(_p0);
            _length += (_p1 - _p0).length();
            ++it;

            if (_length > _lineStringLength)
            {
              _length -= _lineStringLength;
              _p0 += _d * _length;
              _lineString.push_back(_p0);
              _lineString.push_back(_p1);
              _multiLineString.push_back(_lineString);
              _lineString.clear();
              _lineString.push_back(_p0);
              _length = (_p1 - _p0).length();
            } else
            {
              //_lineString.push_back(_p0);
            }
          }

          std::cout << _multiLineString.size() << " " << number() << std::endl;
       //   TOMO_ASSERT(_multiLineString.size() == number());

          return _multiLineString;
        }

        TBD_PROPERTY(int,number)
      };
    }
  }
}
