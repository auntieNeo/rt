#include "scene.h"

namespace rt { namespace scene {
  Scene::Scene() {
  }

  Scene::~Scene() {
  }

  void Scene::addObject(std::unique_ptr<SceneObject> object) {
    SceneObjectPtr o = SceneObjectPtr(std::move(object));
    m_objects.push_back(o);
    DrawableObjectPtr drawable = std::dynamic_pointer_cast<DrawableObject>(o);
    if (drawable) {
      m_drawables.push_back(drawable);
    }
    PointLightPtr pointLight = std::dynamic_pointer_cast<PointLight>(o);
    if (pointLight) {
      m_pointLights.push_back(pointLight);
    }
  }

  double Scene::intersect(const Ray &ray, glm::dvec4 &normal, DrawableObjectPtr &object) const {
    // FIXME: I would rather loop over m_drawableObjects or something this
    // tight in the main loop
    double least = DBL_MAX;
    glm::dvec4 leastNormal;
    for (DrawableObjectPtr o : m_drawables) {
      double t = o->intersect(ray, leastNormal);
      if (t <= 0.0)
        continue;
      if (t < least) {
        least = t;
        normal = leastNormal;
        object = o;
      }
    }
    return least;
  }

  double Scene::intersect(const Ray &ray) const {
    glm::dvec4 normal;
    DrawableObjectPtr object;
    return this->intersect(ray, normal, object);
  }
} }
