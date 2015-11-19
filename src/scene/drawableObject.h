#ifndef RT_SCENE_DRAWABLE_OBJECT_H_
#define RT_SCENE_DRAWABLE_OBJECT_H_

#include "sceneObject.h"

namespace rt {
  class Ray;

  namespace scene {
    class DrawableObject : public SceneObject {
      public:
        DrawableObject(glm::dvec4 position, glm::dquat orientation);
        virtual ~DrawableObject();

        virtual double intersect(const Ray &ray) const = 0;
    };
  }
}

#endif
