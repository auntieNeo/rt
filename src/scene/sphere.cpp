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
    double b = 2.0 * glm::dot(ray.direction(), ray.origin() - this->position());
    float c = glm::dot(ray.origin() - this->position(),
                       ray.origin() - this->position())
              - this->radius() * this->radius();
    double d = b*b - 4.0*a*c;

    if (d <= 0.0)
      return 0.0;  // Non-intersecting or grazing

    double t_0 = (-b + sqrt(d)) / (2.0 * a);
    double t_1 = (-b - sqrt(d)) / (2.0 * a);

    return t_0 < t_1 ? std::max(t_0, 0.0) : std::max(t_1, 0.0);
  }
} }
