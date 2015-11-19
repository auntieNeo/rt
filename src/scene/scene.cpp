#include "scene.h"

namespace rt { namespace scene {
  Scene::Scene() {
  }

  Scene::~Scene() {
  }

  void Scene::addObject(std::unique_ptr<SceneObject> object) {
    m_objects.push_back(SceneObjectPtr(std::move(object)));
  }
} }
