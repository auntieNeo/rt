#ifndef RT_SCENE_SCENE_H_
#define RT_SCENE_SCENE_H_

#include "sceneObject.h"

namespace rt { namespace scene {
  class Scene {
    private:
      SceneObjectList m_objects;

    public:
      Scene();
      ~Scene();

      void addObject(std::unique_ptr<SceneObject> object);
  };
} }

#endif
