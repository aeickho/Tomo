#include "tomo/Shader.hpp"
#include "tomo/BoundingBox.hpp"

namespace tomo
{
  struct SceneObject : public Primitive
  {
    bool visible;
    TBD_PROPERTY_REF(Matrix4f,matrix);   
    TBD_PROPERTY_REF(BoundingBox,boundingBox);

  protected:
    virtual void calcBoundingBox() = 0;
  };

  DEFINE_CONTAINERS(SceneObject)
}
