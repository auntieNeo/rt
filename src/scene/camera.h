#ifndef RT_SCENE_CAMERA_H_
#define RT_SCENE_CAMERA_H_

#include "sceneObject.h"

namespace rt { namespace scene {
  class Camera : public SceneObject {
    public:
      Camera(glm::dvec4 position, glm::dquat orientation);
      virtual ~Camera();

      virtual glm::dmat4 projection(double aspect) const = 0;
  };
} }

#endif
