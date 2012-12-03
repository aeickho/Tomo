#pragma once

#include "create/circleWith3Holes.hpp"

namespace tomo
{
  namespace test
  {
    namespace generators
    {
      struct MultiPolygonGenerator : 
        Generator<geometry::prim::MultiPolygon>
      {
        typedef geometry::prim::MultiPolygon MultiPolygon;
        TOMO_TEST_GENERATOR_DECL(MultiPolygon)

        void generate()
        {
          using geometry::base::Point2f;

          Object _multiPolygon;
          _multiPolygon.push_back(create::circleWith3Holes(Point2f(0.25,0.3),0.2));
          _multiPolygon.push_back(create::circleWith3Holes(Point2f(0.75,0.3),0.2));
          _multiPolygon.push_back(create::circleWith3Holes(Point2f(0.5,0.7),0.2));
          objs().push_back(_multiPolygon);
        }

        void draw(const Object& _obj)
        {
          wrapper().draw<>(_obj,Magick::Color("green"));
        }
      };
    }
  }
}
