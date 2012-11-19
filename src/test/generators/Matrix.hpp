#include "Generator.hpp"

namespace tomo
{
  namespace test
  {
    namespace generators
    {
      struct Matrix : Generator<geometry::base::Matrix4f> 
      {
        typedef geometry::base::Matrix4f Matrix4f;
        TOMO_TEST_GENERATOR_DECL(Matrix,Matrix4f);

        ObjectList generate() const
        {
          ObjectList _objs;
          _objs.push_back(Matrix4f()); // Identity matrix
          return _objs;
        }

        void draw(Wrapper& _wrapper, const Object& _obj)
        {}

      };
    }
  }
}
