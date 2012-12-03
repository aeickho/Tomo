#pragma once

#include <boost/geometry.hpp>
//#include "tomo/geometry/algorithm/Chop.hpp"
#include "../generators/Generator.hpp"
#include "../generators/create/circle.hpp"


namespace tomo
{
  namespace test
  {
    namespace generators
    {
      struct Chop : Generator<geometry::prim::Ring>
      {
        typedef geometry::prim::Ring Ring;
        TOMO_TEST_GENERATOR_DECL(Chop,Ring)

        ObjectList generate() const
        {
          using geometry::base::Point2f;
          ObjectList _objs;
        //  _objs.push_back(create::circle(Point2f(0.5,0.5),0.4));
        //  _objs.push_back(create::circle(Point2f(0.5,0.5),0.4,false,16));
          _objs.push_back(create::circle(Point2f(0.5,0.5),0.4,false,12));
          return _objs;
        }

        void draw(Wrapper& _wrapper, const Object& _obj)
        {
          _wrapper.draw(_obj,Magick::Color("green"));
        }
      };
    }
  }
}

TOMO_TEST_CASE( Chop, 512 )
{
  //using tomo::geometry::algorithm::Chop;

  _w.drawLineStringEndings(true);

  using tomo::geometry::prim::Ring;
  using tomo::geometry::prim::LineString;
  using tomo::geometry::prim::MultiLineString;

  const Ring& _ring = _obj;
  typedef Ring::scalar_type scalar_type;
  typedef Ring::vec_type vec_type;
  typedef Ring::point_type point_type;

  int number_ = 5;

  TOMO_ASSERT(_ring.correct());
  MultiLineString _multiLineString;
  scalar_type _ringLength = _ring.length();
  scalar_type _lineStringLength = _ringLength / (number_);

  LineString _lineString;
  scalar_type _length = 0;
  point_type _p0 = _ring.front(), _p1;
  Ring::const_iterator it = _ring.begin();

  //_lineString.push_back(_p0);
  
  for(;;)
  {
    if (it+1 == _ring.end())
    {
      _p0 = *it;
      _p1 = _ring.front();
      break;
    }
    else
    {
      _p0 = *it;
      _p1 = *(it+1);
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

    //_lineString.push_back(_p0);
    scalar_type _segmentLength = (_p1 - _p0).length();
    
    _lineString.push_back(_p0);
    _length += _segmentLength;

    if (_length >= _lineStringLength)
    {
      _length -= _lineStringLength;
      _p0 += _d * _length;
      _lineString.push_back(_p0);
      _multiLineString.push_back(_lineString);
      _lineString.clear();
      _lineString.push_back(_p0);
   //   _lineString.push_back(_p1);
      _length += (_p1 - _p0).length();
    } else
    {
      ++it;
    }

    if (_lineString.size() > 1)
    {
      _w.clear();
      _w.draw(_obj,Magick::Color("green"));
      _w.draw(_lineString,Magick::Color("orange"));
      _w.draw(_p0,Magick::Color("white"));
      writeImage();
    }
    
  }


      _w.clear();

  _w.draw(_obj,Magick::Color("green"));
  _w.draw(_multiLineString,Magick::Color("red"));

//  MultiLineString&& _choppedRing = Chop<tomo::geometry::prim::Ring>(8)(_obj);
//  _w.draw(_choppedRing,Magick::Color("red"));

  writeImage();
}
TOMO_TEST_CASE_END
