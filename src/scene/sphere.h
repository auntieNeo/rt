#ifndef RT_SCENE_SPHERE_H_
#define RT_SCENE_SPHERE_H_

#include "drawableObject.h"

namespace rt { namespace scene {
  class Sphere : public DrawableObject {
    private:
      double m_radius;

    public:
      Sphere(double radius = 1.0,
          glm::dvec4 position = glm::dvec4(0.0, 0.0, 0.0, 1.0),
          glm::dquat orientation = glm::dquat());
      virtual ~Sphere();

      double radius() const { return m_radius; }

      virtual double intersect(const Ray &ray, glm::dvec4 &normal) const;
  };
} }

#endif
