#pragma once

#include "tomo/geometry/comp/Mesh.hpp"
#include "Generator.hpp"

namespace tomo
{
  namespace test
  {
    namespace generators
    {
      struct TriangleSlicer :
        Generator<geometry::comp::Mesh>
      {
        typedef geometry::comp::Mesh Mesh;
        TOMO_TEST_GENERATOR_DECL(TriangleSlicer,Mesh)

        ObjectList generate() const
        {
          ObjectList _objs;
          Object _mesh("yoda.stl");
          _objs.push_back(_mesh);
          return _objs;
        }

        void draw(Wrapper& _wrapper, const Object& _obj)
        {
        }
      };
    }
  }
}

