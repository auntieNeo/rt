#ifndef RT_SCENE_DRAWABLE_OBJECT_H_
#define RT_SCENE_DRAWABLE_OBJECT_H_

#include <memory>

#include "materialProperties.h"
#include "sceneObject.h"

namespace rt {
  class Ray;

  namespace scene {
    class DrawableObject : public SceneObject {
      private:
        MaterialPropertiesPtr m_materialProperties;
      public:
        DrawableObject(MaterialPropertiesPtr materialProperties,
            const glm::dvec4 &position = glm::dvec4(0.0, 0.0, 0.0, 1.0),
            const glm::dquat &orientation = glm::dquat());
        virtual ~DrawableObject();

        MaterialPropertiesPtr material() { return m_materialProperties; }

        virtual double intersect(const Ray &ray, glm::dvec4 &normal) const = 0;

        virtual bool drawable() const { return true; }
    };

    typedef std::shared_ptr<DrawableObject> DrawableObjectPtr;
    typedef std::vector<DrawableObjectPtr> DrawableObjectList;
  }
}

#endif
