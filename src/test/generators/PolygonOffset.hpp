#pragma once

#include "Generator.hpp"
#include "create/circleWith3Holes.hpp"

namespace tomo
{
  namespace test
  {
    namespace generators
    {
      struct PolygonOffset : 
        Generator<geometry::prim::Polygon>
      {
        typedef geometry::prim::Polygon Polygon;
        TOMO_TEST_GENERATOR_DECL(PolygonOffset,Polygon)

        ObjectList generate() const
        {
          using geometry::base::Point2f;
          ObjectList _objs;
          _objs.push_back(create::circleWith3Holes(Point2f(0.25,0.3),0.2));
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
