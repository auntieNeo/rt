#include <glm/gtc/matrix_transform.hpp>

#include "perspectiveCamera.h"

namespace rt { namespace scene {
  PerspectiveCamera::PerspectiveCamera(
      double fovy, double near, double far,
      glm::dvec4 position, glm::dquat orientation) :
    Camera(position, orientation), m_fovy(fovy), m_near(near), m_far(far)
  {
  }

  PerspectiveCamera::~PerspectiveCamera() {
  }

  glm::dmat4 PerspectiveCamera::projection(double aspect) const {
    return glm::perspective(
        m_fovy,
        aspect,
        m_near,
        m_far);
  }
} }
