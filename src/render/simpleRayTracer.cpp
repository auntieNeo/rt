#include "../math.h"

#include "simpleRayTracer.h"

namespace rt { namespace render {
  glm::dvec3 SimpleRayTracer::computeRadiance(
      const Ray &ray, const scene::Scene &scene)
  {
    return m_trace(ray, scene, 0);
  }

  glm::dvec3 SimpleRayTracer::m_trace(
      const Ray &ray, const scene::Scene &scene, int depth)
  {
    if (++depth > MAX_DEPTH)
      return glm::dvec3(1.0, 0.0, 1.0);

    glm::dvec4 normal;
    scene::DrawableObjectPtr object;
    double t = scene.intersect(ray, normal, object);
    if (t == DBL_MAX)
      return glm::dvec3(1.0, 0.0, 1.0);

    glm::dvec3 result;

    // Get the point just outside the surface to avoid self-shadows
    glm::dvec4 outsidePoint = ray.point(t) + math::EPSILON * normal;

    if (object->material()->isDiffuse()) {
      // Compute light contribution from point light sources
      glm::dvec3 localLight(0.0, 0.0, 0.0);
      for (scene::PointLightPtr pointLight : scene.pointLights()) {
        glm::dvec4 lightDirection = glm::normalize(
            pointLight->position() - ray.point(t));
        Ray shadowRay(outsidePoint, lightDirection);
        double t_shadowRay = scene.intersect(shadowRay);
        double lightDistance = glm::length(
            pointLight->position() - shadowRay.origin());
        if (t_shadowRay < DBL_MAX) {
          // The shadow ray hit something; determine if the object it hit is
          // between us and the light source
          double occlusionDistance = glm::length(
              shadowRay.point(t_shadowRay) - shadowRay.origin());
          if (occlusionDistance < lightDistance)
            continue;  // We are in shadow for this light source
        }
        // Add the contribution from this light source to the sum
        localLight += shading(
            ray,
            t,
            lightDistance,
            normal,
            *pointLight,
            *(object->material()));
      }
      result += localLight;
    }

    if (object->material()->isMirror()) {
      // Compute the reflected light using a recursive call
      Ray reflectedRay = Ray(
          outsidePoint,
          glm::normalize(glm::reflect(ray.direction(), normal)));
      glm::dvec3 reflectedLight = m_trace(reflectedRay, scene, depth);
      result += reflectedLight;
    }

    result += object->material()->ambient();

    return result;
  }

  glm::dvec3 SimpleRayTracer::shading(const Ray &ray, double t, double r,
      const glm::dvec4 &normal, scene::PointLight &pointLight,
      const scene::MaterialProperties &material)
  {
    glm::dvec4 lightVector = glm::normalize(pointLight.position() - ray.point(t));
    glm::dvec4 viewVector = glm::normalize(-ray.direction());

    glm::dvec4 halfVector = glm::normalize(lightVector + viewVector);

    glm::dvec3 reflectivity =
      material.normalizedDiffuse() +
      material.normalizedSpecular() *
      pow(dot(halfVector, normal), material.smoothness());

    glm::dvec3 lightIrradiance = pointLight.intensity() / (r * r);
    glm::dvec3 irradiance = lightIrradiance * dot(lightVector, normal);

    return reflectivity * irradiance;
  }
} }
