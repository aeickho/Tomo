#pragma once

#include "tomo/SceneObject.hpp"
#include "tomo/Light.hpp"
#include "tomo/Camera.hpp"
#include "tomo/FrameBuffer.hpp"

#include <vector>

namespace tomo
{
  using std::vector;

  struct Scene
  {
    Scene() { maxBounce = 15; }

    void addObject(SceneObject* obj) { objects.push_back(obj); }

    void castRays(Rays& rays, Image* outImage = NULL);
    
    Vec3f traceRay(Ray& ray, SceneObject* curObj = NULL);
    bool traceShadowRay(Ray& shadowRay, SceneObject* curObj = NULL);

    unsigned maxBounce;

    SceneObjectList objects;
    LightList  lights;
    Camera* camera;
    FrameBuffer frameBuffer;
  };
}

