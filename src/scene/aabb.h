#ifndef RT_SCENE_AABB_H_
#define RT_SCENE_AABB_H_

#include <glm/glm.hpp>

#include <utility>

#include "../ray.h"

namespace rt { namespace scene {
  class AABB {
    private:
      glm::dvec3 m_min, m_max;
    public:
      AABB() :
        AABB(glm::dvec3(DBL_MAX, DBL_MAX, DBL_MAX),
             glm::dvec3(-DBL_MAX, -DBL_MAX, -DBL_MAX)) {}
      AABB(const glm::dvec3 &min, const glm::dvec3 &max);
      ~AABB();

      const glm::dvec3 &min() const { return m_min; }
      const glm::dvec3 &max() const { return m_max; }
      glm::dvec3 &min() { return m_min; }
      glm::dvec3 &max() { return m_max; }

      bool intersect(const Ray &ray) const;
  };
} }

#endif
