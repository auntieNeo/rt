#include "../math.h"

#include "simpleRayCaster.h"

namespace rt { namespace render {
  glm::dvec3 SimpleRayCaster::computeRadiance(const Ray &ray, const scene::Scene &scene) {
    glm::dvec4 normal;
    scene::DrawableObjectPtr object;
    double t = scene.intersect(ray, normal, object);
    if (t == DBL_MAX)
      return glm::dvec3(0.0, 0.0, 1.0);

    // Compute light contribution from point light sources
    glm::dvec3 localLight(0.0, 0.0, 0.0);
    for (scene::PointLightPtr pointLight : scene.pointLights()) {
      glm::dvec4 lightDirection = glm::normalize(
          pointLight->position() - ray.point(t));
      Ray shadowRay(
          ray.point(t) + math::EPSILON * lightDirection, lightDirection);
      double t_shadowRay = scene.intersect(shadowRay);
      if (t_shadowRay < DBL_MAX) {
        // The shadow ray hit something; determine if the object it hit is
        // between us and the light source
        double lightDistance = glm::length(
            pointLight->position() - shadowRay.origin());
        double occlusionDistance = glm::length(
            shadowRay.point(t_shadowRay) - shadowRay.origin());
        if (occlusionDistance < lightDistance)
          continue;  // We are in shadow for this light source
      }
      // TODO: Add the contribution from this light source
      localLight += shading(
          ray,
          t,
          normal,
          *pointLight);
    }

    return localLight;
  }

  glm::dvec3 SimpleRayCaster::shading(const Ray &ray, double t,
      const glm::dvec4 &normal, scene::PointLight &pointLight) {
    return glm::dvec3(1.0, 1.0, 1.0);  // TODO
  }
} }
