#ifndef RT_SCENE_SCENE_H_
#define RT_SCENE_SCENE_H_

#include <glm/glm.hpp>

#include "../ray.h"
#include "drawableObject.h"
#include "pointLight.h"
#include "sceneObject.h"

namespace rt { namespace scene {
  class Scene {
    private:
      SceneObjectList m_objects;
      DrawableObjectList m_drawables;
      PointLightList m_pointLights;

    public:
      Scene();
      ~Scene();

      void addObject(std::unique_ptr<SceneObject> object);

      // TODO: Add to the intersect interface. Some things that we need are
      // material properties, normal, object, etc.
      double intersect(const Ray &ray, glm::dvec4 &normal, DrawableObjectPtr &object) const;
      double intersect(const Ray &ray) const;

      const PointLightList &pointLights() const { return m_pointLights; }
  };
} }

#endif
