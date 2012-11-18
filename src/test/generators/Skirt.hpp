#pragma once

#include "Generator.hpp"
#include "create/circleWith3Holes.hpp"

namespace tomo
{
  namespace test
  {
    namespace generators
    {
      struct Skirt : 
        Generator<geometry::prim::MultiPolygon>
      {
        typedef geometry::prim::MultiPolygon MultiPolygon;
        TOMO_TEST_GENERATOR_DECL(Skirt,MultiPolygon)

        ObjectList generate() const
        {
          using geometry::base::Point2f;
          ObjectList _objs;

          Object _multiPolygon;
          _multiPolygon.push_back(create::circleWith3Holes(Point2f(0.25,0.3),0.2));
          _multiPolygon.push_back(create::circleWith3Holes(Point2f(0.75,0.3),0.2));
          _multiPolygon.push_back(create::circleWith3Holes(Point2f(0.5,0.7),0.2));
          _objs.push_back(_multiPolygon);
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
