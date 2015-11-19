#include <glm/gtc/matrix_transform.hpp>

#include "perspectiveCamera.h"

namespace rt { namespace scene {
  PerspectiveCamera::PerspectiveCamera(
      double fovy, double aspect, double near, double far,
      glm::dvec4 position, glm::dquat orientation) :
    Camera(position, orientation)
  {
    m_fovy = fovy;
    m_aspect = aspect;
    m_near = near;
    m_far = far;
  }

  PerspectiveCamera::~PerspectiveCamera() {
  }

  glm::dmat4 PerspectiveCamera::projection() const {
    return glm::perspective(
        m_fovy,
        m_aspect,
        m_near,
        m_far);
  }
} }
