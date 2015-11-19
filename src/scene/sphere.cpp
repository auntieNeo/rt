#include <algorithm>

#include "../ray.h"

#include "sphere.h"

namespace rt { namespace scene {
  Sphere::Sphere(double radius, glm::dvec4 position, glm::dquat orientation) :
    DrawableObject(position, orientation)
  {
    m_radius = radius;
  }

  Sphere::~Sphere() {
  }

  /**
   * Calculates the ray-sphere intersection using the quadratic equation.
   */
  double Sphere::intersect(const Ray &ray) const {
    double a = glm::dot(ray.direction(), ray.direction());
    double b = 2 * (
        glm::dot(ray.origin(), ray.direction()) -
        glm::dot(this->position(), ray.direction())
        );
    double c = glm::dot(ray.origin(), ray.origin()) -
      2 * glm::dot(this->position(), ray.origin()) -
      this->radius() * this->radius();

    double d = b*b - 4*a*c;

    if (d <= 0.0)
      return 0.0;  // Non-intersecting or grazing

    double t_0 = (-b + sqrt(d)) / (2.0 * a);
    double t_1 = (-b - sqrt(d)) / (2.0 * a);

    return t_0 < t_1 ? std::max(t_0, 0.0) : std::max(t_1, 0.0);
  }
} }
