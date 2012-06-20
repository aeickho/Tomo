#include "tomo/BoundingBox.hpp"

namespace tomo
{
  /* A SceneObject is an object which is transformable and displayable within a scene.
     Moreover, it has a bounding box to determine its extents.
   */
  struct SceneObject : public Primitive
  {
    bool visible;
    BoundingBox boundingBox_;
    float radius() const { return boundingBox_.size().length()/2; }
    TBD_PROPERTY_REF(Matrix4f,matrix);   
  protected:
    virtual void calcBoundingBox() = 0;
  };
}
