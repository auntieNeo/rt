#include "camera.h"

namespace rt { namespace scene {
  Camera::Camera(glm::dvec4 position, glm::dquat orientation) :
    SceneObject(position, orientation)
  {
  }

  Camera::~Camera() {
  }
} }
