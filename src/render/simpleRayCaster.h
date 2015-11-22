#ifndef RT_RENDER_SIMPLE_RAY_CASTER_H_
#define RT_RENDER_SIMPLE_RAY_CASTER_H_

#include <glm/glm.hpp>

#include "../ray.h"
#include "../scene/materialProperties.h"
#include "../scene/pointLight.h"
#include "../scene/scene.h"

namespace rt { namespace render {
  class SimpleRayCaster {
    public:
      static glm::dvec3 computeRadiance(const Ray &ray, const scene::Scene &scene);

    private:
      static glm::dvec3 shading(const Ray &ray, double t, double r,
          const glm::dvec4 &normal, scene::PointLight &pointLight,
          const scene::MaterialProperties &material);
  };
} }

#endif
