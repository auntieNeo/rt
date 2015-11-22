#include "plane.h"

namespace rt { namespace scene {
  Plane::Plane(MaterialPropertiesPtr material,
      glm::dvec4 position, glm::dquat orientation)
    : DrawableObject(material, position, orientation)
  {
  }

  Plane::~Plane() {
  }

  double Plane::intersect(const Ray &ray, glm::dvec4 &normal) const {
    normal = this->orientation() * glm::dvec4(1.0, 0.0, 0.0, 0.0);

    double denominator = glm::dot(ray.direction(), normal);

    if (denominator == 0.0) {
      return 0.0;  // Ray is parallel to the plane
    }

    double t = glm::dot(this->position() - ray.origin(), normal)  / denominator;
    return t;
  }
} }
