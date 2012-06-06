#include "tomo/Shader.hpp"
#include "tomo/BoundingBox.hpp"

namespace tomo
{
  struct SceneObject : public Primitive
  {
    bool visible;
    BoundingBox boundingBox;
    TBD_PROPERTY_REF(Matrix4f,matrix);   

  protected:
    virtual void calcBoundingBox() = 0;
  };
}
