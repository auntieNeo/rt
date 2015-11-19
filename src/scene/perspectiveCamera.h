#ifndef RT_SCENE_PERSPECTIVE_CAMERA_H_
#define RT_SCENE_PERSPECTIVE_CAMERA_H_

#include "camera.h"

namespace rt { namespace scene {
  class PerspectiveCamera : public Camera {
    private:
      double m_fovy, m_near, m_far;
    public:
      PerspectiveCamera(
          double fovy, double near, double far,
          glm::dvec4 position, glm::dquat orientation);
      ~PerspectiveCamera();

      glm::dmat4 projection(double aspect) const;
  };
} }

#endif
