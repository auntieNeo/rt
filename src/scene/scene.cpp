#include "scene.h"

namespace rt { namespace scene {
  Scene::Scene() {
  }

  Scene::~Scene() {
  }

  void Scene::addObject(std::unique_ptr<SceneObject> object) {
    m_objects.push_back(SceneObjectPtr(std::move(object)));
  }

  double Scene::intersect(const Ray &ray, glm::dvec4 &normal) const {
    // FIXME: I would rather loop over m_drawableObjects or something this
    // tight in the main loop
    double least = DBL_MAX;
    glm::dvec4 leastNormal;
    for (SceneObjectPtr object : m_objects) {
      if (!object->drawable())
        continue;
      double t = object->intersect(ray, leastNormal);
      if (t <= 0.0)
        continue;
      if (t < least) {
        least = t;
        normal = leastNormal;
      }
    }
    return least;
  }
} }
