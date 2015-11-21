#ifndef RT_SCENE_POINT_LIGHT_H_
#define RT_SCENE_POINT_LIGHT_H_

#include <memory>
#include <vector>

#include "sceneObject.h"

namespace rt { namespace scene {
  class PointLight : public SceneObject {
      glm::dvec3 m_intensity;

    public:
      PointLight(glm::dvec3 intensity, glm::dvec4 position,
          // Point lights are not really orientable
          glm::dquat orientation = glm::dquat());
      ~PointLight();

      glm::dvec3 intensity() const { return m_intensity; }
  };

  typedef std::shared_ptr<PointLight> PointLightPtr;
  typedef std::vector<PointLightPtr> PointLightList;
} }

#endif
