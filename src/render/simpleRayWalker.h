#ifndef RT_RENDER_SIMPLE_RAY_WALKER_H_
#define RT_RENDER_SIMPLE_RAY_WALKER_H_

namespace rt { namespace render {
  class SimpleRayWalker {
    public:
      static glm::dvec3 computeRadiance(const Ray &ray, const scene::Scene &scene) {
        glm::dvec4 normal;
        scene::DrawableObjectPtr object;
        double t = scene.intersect(ray, normal, object);
        if (t != DBL_MAX)
          return glm::dvec3(normal);
        return glm::dvec3(0.0, 0.0, 0.0);
      }
  };
} }

#endif
