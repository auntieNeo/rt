#include <algorithm>

#include "../ray.h"

#include "sphere.h"

namespace rt { namespace scene {
  Sphere::Sphere(double radius, MaterialPropertiesPtr materialProperties,
      glm::dvec4 position, glm::dquat orientation)
    : DrawableObject(materialProperties, position, orientation)
  {
    m_radius = radius;
  }

  Sphere::~Sphere() {
  }

  /**
   * Calculates the ray-sphere intersection using the quadratic equation.
   */
  double Sphere::intersect(const Ray &ray, glm::dvec4 &normal) const {
    double a = glm::dot(ray.direction(), ray.direction());
    double b = 2.0 * glm::dot(ray.direction(), ray.origin() - this->position());
    float c = glm::dot(ray.origin() - this->position(),
                       ray.origin() - this->position())
              - this->radius() * this->radius();
    double d = b*b - 4.0*a*c;

    if (d <= 0.0)
      return 0.0;  // Non-intersecting or grazing

    double t_0 = std::max((-b + sqrt(d)) / (2.0 * a), 0.0);
    double t_1 = std::max((-b - sqrt(d)) / (2.0 * a), 0.0);

    double t = t_0 < t_1 ?
      (t_0 != 0.0 ? t_0 : t_1) :
      (t_1 != 0.0 ? t_1 : t_0);

    if (t == 0.0)
      return DBL_MAX;  // Sphere is behind the ray

    // Compute and return the normal
    normal = glm::normalize(ray.point(t) - this->position());

    return t;
  }
} }
