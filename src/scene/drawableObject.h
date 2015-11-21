#ifndef RT_SCENE_DRAWABLE_OBJECT_H_
#define RT_SCENE_DRAWABLE_OBJECT_H_

#include <memory>

#include "sceneObject.h"

namespace rt {
  class Ray;

  namespace scene {
    class DrawableObject : public SceneObject {
      public:
        DrawableObject(glm::dvec4 position, glm::dquat orientation);
        virtual ~DrawableObject();

        virtual double intersect(const Ray &ray, glm::dvec4 &normal) const = 0;

        virtual bool drawable() const { return true; }
    };

    typedef std::shared_ptr<DrawableObject> DrawableObjectPtr;
    typedef std::vector<DrawableObjectPtr> DrawableObjectList;
  }
}

#endif
