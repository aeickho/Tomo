#pragma once
#include <GL/glu.h>
#include "tomo/Camera.hpp"

namespace render
{
  namespace gl
  {
    template<class COORD> void project( tomo::Camera<COORD>& _camera )
    {
      // update coordinates
      _camera.update();
      // realize coordinates
      gluLookAt(
          _camera.eye().x(),
          _camera.eye().y(),
          _camera.eye().z(),
          _camera.center().x(),
          _camera.center().y(),
          _camera.center().z(),
          _camera.up().x(),
          _camera.up().y(),
          _camera.up().z() 
          );
    }
  }
}
