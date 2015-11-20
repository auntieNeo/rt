#ifndef RT_SCENE_SCENE_OBJECT_H_
#define RT_SCENE_SCENE_OBJECT_H_

#include <memory>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "../ray.h"

namespace rt { namespace scene {
  class SceneObject {
    private:
      glm::dvec4 m_position;
      glm::dquat m_orientation;

    public:
      SceneObject(glm::dvec4 position, glm::dquat orientation);
      virtual ~SceneObject();

      glm::dvec4 position() const { return m_position; }
      glm::dquat orientation() const { return m_orientation; }

      // FIXME: I would rather have only drawable objects define intersect, but
      // I'm not sure how to handle the polymorphism here.
      virtual double intersect(const Ray &ray) const { return 0.0; }

      virtual bool drawable() const { return false; }
  };

  typedef std::shared_ptr<SceneObject> SceneObjectPtr;
  typedef std::vector<SceneObjectPtr> SceneObjectList;
} }

#endif
