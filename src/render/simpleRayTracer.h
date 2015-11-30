#ifndef RT_RENDER_SIMPLE_RAY_TRACER_H_
#define RT_RENDER_SIMPLE_RAY_TRACER_H_

#include <glm/glm.hpp>

#include "../ray.h"
#include "../scene/materialProperties.h"
#include "../scene/pointLight.h"
#include "../scene/scene.h"

namespace rt { namespace render {
  class SimpleRayTracerBase {
    public:
      static glm::dvec3 computeRadiance(
          const Ray &ray, const scene::Scene &scene);

      // TODO: Make this value configurable in the template parameters
      static const int MAX_DEPTH = 10;

    private:
      static glm::dvec3 m_trace(const Ray &ray, const scene::Scene &scene, int depth);
      static glm::dvec3 shading(const Ray &ray, double t, double r,
          const glm::dvec4 &normal, scene::PointLight &pointLight,
          const scene::MaterialProperties &material);
  };

  /**
   * Shim class to templatize the simple ray tracer, which does not actually
   * use a pseudo random number engine.
   */
  template <class PseudoRandomNumberEngine>
  class SimpleRayTracer : public SimpleRayTracerBase {
    public:
      static glm::dvec3 computeRadiance(
          const Ray &ray, const scene::Scene &scene,
          PseudoRandomNumberEngine &randomEngine)
      {
        return SimpleRayTracerBase::computeRadiance(ray, scene);
      }
  };
} }

#endif
