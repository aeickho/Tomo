#include "tomo/BoundingBox.hpp"

namespace tomo
{
  /* A SceneObject is an object which is transformable and displayable within a scene.
     Moreover, it has a bounding box to determine its extents.
   */
  struct SceneObject : public Primitive3f
  {
    bool visible;
    TBD_PROPERTY_REF(Matrix4f,matrix);   
  protected:
    virtual void calcBoundingBox() = 0;
  };
}
