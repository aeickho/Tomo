#pragma once

#include "Generator.hpp"

namespace tomo
{
  namespace test
  {
    namespace generators
    {
      struct VertexNearestVisitors :
        Generator< std::vector<geometry::prim::Vertex2f> >
      {
        typedef std::vector<geometry::prim::Vertex2f> Vertices;
        TOMO_TEST_GENERATOR_DECL(VertexNearestVisitors,Vertices)

        ObjectList generate() const
        {
          using tomo::geometry::base::Point2f;
          using tomo::geometry::prim::Vertex2f;

          ObjectList _objs;

          Vertices _vertices;
          {
            int n = 1000;
            for (int i = 0; i < n; i++)
            {
            _vertices.push_back(Vertex2f(Point2f(0.5+0.5*std::sin(RND*M_PI*2),RND)));
            }
          }
          _objs.push_back(_vertices);

          return _objs;
        }

        void draw(Wrapper& _wrapper, const Object& _obj)
        {
          _wrapper.draw<>(_obj,"green");
        }
      };
    }
  }
}
