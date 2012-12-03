#include "Generator.hpp"
#pragma once

namespace tomo
{
  namespace test
  {
    namespace generators
    {
      struct Segments : 
        Generator< std::vector<geometry::prim::Segment> >
      {
        TOMO_TEST_GENERATOR_DECL(Segments,std::vector<geometry::prim::Segment>);

        void generate()
        {
        }

        void draw(const Object& _obj)
        {
          wrapper().draw<>(_obj,Magick::Color("green"));
        }
      };
    }
  }
}
