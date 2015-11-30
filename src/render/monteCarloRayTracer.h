#ifndef RT_RENDER_MONTE_CARLO_RAY_TRACER_H_
#define RT_RENDER_MONTE_CARLO_RAY_TRACER_H_

#include <cmath>
#include <random>

#include <glm/glm.hpp>

#include "../math.h"

namespace rt { namespace render {
  template <class PseudoRandomNumberEngine>
  class MonteCarloRayTracer {
    public:
      static glm::dvec3 computeRadiance(
          const Ray &ray, const scene::Scene &scene,
          PseudoRandomNumberEngine &randomEngine);

      static const int NUM_SAMPLES = 1;
      static const int MAX_DEPTH = 15;

      static glm::dvec3 m_trace(
          const Ray &ray, const scene::Scene &scene, 
          PseudoRandomNumberEngine &randomEngine,
          int depth);
  };

  template <class PseudoRandomNumberEngine>
  glm::dvec3 MonteCarloRayTracer<PseudoRandomNumberEngine>::computeRadiance(
      const Ray &ray, const scene::Scene &scene,
      PseudoRandomNumberEngine &randomEngine)
  {
    glm::dvec3 result(0.0, 0.0, 0.0);
    for (size_t i = 0; i < NUM_SAMPLES; ++i) {
      // Monte Carlo numeric integration
      result += m_trace(ray, scene, randomEngine, 0) / double(NUM_SAMPLES);
    }
    return result;
  }

  template <class PseudoRandomNumberEngine>
  glm::dvec3 MonteCarloRayTracer<PseudoRandomNumberEngine>::m_trace(
      const Ray &ray, const scene::Scene &scene, 
      PseudoRandomNumberEngine &randomEngine,
      int depth)
  {
    // The method used in this function can be found in Realistic Ray Tracing,
    // Second Edition, Shirley 2003

    if (++depth > MAX_DEPTH)
      return glm::dvec3(0.0, 0.0, 0.0);

    glm::dvec4 normal;
    scene::DrawableObjectPtr object;
    double t = scene.intersect(ray, normal, object);
    if (t == DBL_MAX)
      return glm::dvec3(0.0, 0.0, 0.0);

    glm::dvec3 result(0.0, 0.0, 0.0);

    // Get the point just inside/outside the volume to avoid self-shadows
    glm::dvec4 insidePoint = ray.point(t) - math::EPSILON * normal;
    glm::dvec4 outsidePoint = ray.point(t) + math::EPSILON * normal;

    // Make a tangent basis
    glm::dvec4 w = glm::dot(normal, ray.direction()) < 0.0 ? normal : -normal;
    glm::dvec4 u = 
      glm::dvec4(
        glm::normalize(
            glm::cross(
              std::abs(w[0]) > 0.1 ?
                glm::dvec3(0.0, 1.0, 0.0) :
                glm::dvec3(1.0, 0.0, 0.0),
              glm::dvec3(w))),
        0.0);
    glm::dvec4 v = glm::dvec4(glm::cross(glm::dvec3(w), glm::dvec3(u)), 0.0);

    // TODO: Use Monte Carlo method for materials that are both diffuse and
    // specular at the same time
    if (object->material()->isDiffuse()) {
      // Select a reflected ray randomly (but uniformly) from the unit
      // hemisphere to approximate Lambertian shading.
      double theta = 2 * M_PI * std::generate_canonical<double, 10>(randomEngine);
      double r = sqrt(std::generate_canonical<double, 10>(randomEngine));
      // Convert these circular coordinates to Cartesian coordinates on the
      // unit hemisphere
      glm::dvec4 reflectedDirection =
        glm::dvec4(
          glm::normalize(
            glm::dvec3(
              u*cos(theta)*r + v*sin(theta)*r + w*sqrt(1-r))),
          0.0);
      Ray reflectedRay(outsidePoint, reflectedDirection);
      return object->material()->emission() +
        object->material()->diffuse() * m_trace(reflectedRay, scene, randomEngine, depth);
    } else if (object->material()->isMirror()) {
      // Reflect the ray as in a perfect mirror
      Ray reflectedRay = Ray(
          outsidePoint,
          glm::normalize(glm::reflect(ray.direction(), normal)));
      return object->material()->emission() +
        object->material()->mirror() * m_trace(reflectedRay, scene, randomEngine, depth);
    } else if (object->material()->isRefraction()) {
      bool into = glm::dot(normal, ray.direction()) < 0.0;  // Is the ray entering or leaving the volume?
      // Index of refraction (assuming that we are rendering in a vacuum with
      // an index of refraction of 1.0)
      double n = into ? 1.0 / object->material()->refractiveIndex()
                       : object->material()->refractiveIndex();
      double cosTheta = glm::dot(ray.direction(), normal);
      double discriminant = 1.0 - (n*n * (1.0 - cosTheta*cosTheta));
      if (discriminant < 0.0) {
        // Total internal reflection
        Ray reflectedRay(
            insidePoint,
            glm::normalize(glm::reflect(ray.direction(), -normal)));
        return object->material()->emission() +
          object->material()->refraction() * m_trace(reflectedRay, scene, randomEngine, depth);
      }
      // Compute the refracted ray
      glm::dvec4 refractionDirection = glm::normalize(n * (ray.direction() + cosTheta * normal) - normal * sqrt(discriminant));
      Ray refractedRay(into ? insidePoint : outsidePoint, refractionDirection);
      // TODO: Implement Beer's law
      return object->material()->emission() +
        object->material()->refraction() * m_trace(refractedRay, scene, randomEngine, depth);
    }

    return object->material()->emission();
  }
} }

#endif
