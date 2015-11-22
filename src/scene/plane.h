#ifndef RT_SCENE_PLANE_H_
#define RT_SCENE_PLANE_H_

#include "drawableObject.h"

namespace rt { namespace scene {
  class Plane : public DrawableObject {
    // NOTE: A plane is defined by a point and a normal. All scene objects
    // already have a position and an orientation, so no additional members are
    // needed for Plane objects.
    public:
      Plane(MaterialPropertiesPtr material, glm::dvec4 position, glm::dquat orientation);
      virtual ~Plane();

      virtual double intersect(const Ray &ray, glm::dvec4 &normal) const;
  };
} }

#endif
