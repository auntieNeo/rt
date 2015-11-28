#include "aabb.h"

namespace rt { namespace scene {
  AABB::AABB(const glm::dvec3 &min, const glm::dvec3 &max)
    : m_min(min), m_max(max)
  {
  }

  AABB::~AABB() {
  }

  bool AABB::intersect(const Ray &ray) const {
    // Ray-box intersection algorithm from
    // "An Efficient and Robust Ray-Box Intersection Algorithm", Williams.
    // Also "Efficiency Issues for Ray Tracing", Brian Smits, 1999
    double t_min, t_max, t_ymin, t_ymax, t_zmin, t_zmax;
    if (ray.direction()[0] >= 0.0) {
      // Ray pointed in the positive x direction must hit the min x plane first
      t_min = (m_min[0] - ray.origin()[0]) / ray.direction()[0];
      t_max = (m_max[0] - ray.origin()[0]) / ray.direction()[0];
    } else {
      // Ray pointed in the negative x direction must hit the max x plane first
      t_min = (m_max[0] - ray.origin()[0]) / ray.direction()[0];
      t_max = (m_min[0] - ray.origin()[0]) / ray.direction()[0];
    }
    if (ray.direction()[1] >= 0.0) {
      // Ray pointed in the positive y direction must hit the min y plane first
      t_ymin = (m_min[1] - ray.origin()[1]) / ray.direction()[1];
      t_ymax = (m_max[1] - ray.origin()[1]) / ray.direction()[1];
    } else {
      // Ray pointed in the negative y direction must hit the max y plane first
      t_ymin = (m_max[1] - ray.origin()[1]) / ray.direction()[1];
      t_ymax = (m_min[1] - ray.origin()[1]) / ray.direction()[1];
    }
    if ((t_min > t_ymax) || (t_ymin > t_max))
      return false;
    if (t_ymin > t_min)
      t_min = t_ymin;
    if (t_ymax < t_max)
      t_max = t_ymax;
    if (ray.direction()[2] >= 0.0) {
      // Ray pointed in the positive z direction must hit the min z plane first
      t_zmin = (m_min[2] - ray.origin()[2]) / ray.direction()[2];
      t_zmax = (m_max[2] - ray.origin()[2]) / ray.direction()[2];
    } else {
      // Ray pointed in the negative z direction must hit the max z plane first
      t_zmin = (m_max[2] - ray.origin()[2]) / ray.direction()[2];
      t_zmax = (m_min[2] - ray.origin()[2]) / ray.direction()[2];
    }
    if ((t_min > t_zmax) || (t_zmin > t_max))
      return false;
    if (t_zmin > t_min)
      t_min = t_zmin;
    if (t_zmax < t_max)
      t_max = t_zmax;

    // NOTE: We allow intersections from within the bounding box (e.g. from
    // rays reflecting inside of bounding boxes in the scene)
    return t_max > 0.0;
  }
} }
