#pragma once

#include "create/circle.hpp"
#include "create/irregular.hpp"

namespace tomo
{
  namespace test
  {
    namespace generators
    {
      struct RingGenerator : 
        Generator<geometry::prim::Ring>
      {
        typedef geometry::prim::Ring Ring;
        TOMO_TEST_GENERATOR_DECL(Ring)

        void generate()
        {
          using geometry::base::Point2f;
          _objs().push_back(create::box(Point2f(0.5,0.5),Point2f(0.5,0.5)));
          _objs().push_back(create::irregularRing(Point2f(0.5,0.5),
          _objs().push_back(create::circle(Point2f(0.5,0.5),
        }

        void draw(const Object& _obj)
        {
          wrapper().draw<>(_obj,Magick::Color("green"));
        }
      };
    }
  }
}
