#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../math.h"

#include "lens.h"

namespace rt { namespace scene {
  Lens::Lens(double radius, double outsideCurvatureRadius,
      double insideCurvatureRadius, double thickness,
      MaterialPropertiesPtr material,
      glm::dvec4 position, glm::dquat orientation)
    : DrawableObject(material, position, orientation),
    m_radius(radius), m_outsideCurvatureRadius(outsideCurvatureRadius),
    m_insideCurvatureRadius(insideCurvatureRadius), m_thickness(thickness)
  {
    // TODO: Move these matricies into a base class
    m_worldModelMatrix = 
      glm::mat4_cast(glm::inverse(this->orientation())) *
      glm::translate(glm::dmat4(1.0), -glm::dvec3(this->position()));
    m_normalMatrix =
      glm::inverseTranspose(glm::mat4_cast(this->orientation()));
  }

  Lens::~Lens() {
  }

  double Lens::intersect(const Ray &ray, glm::dvec4 &normal) const {
    // Transform the ray into model space
    Ray modelRay = m_worldModelMatrix * ray;

    // The origin of the lens is the point on the  surface of our outside
    // curvature on the z-axis
    // TODO: Handle the case where m_outsideCurvatureRadius is negative
    glm::dvec4 s1_center(0.0, 0.0, m_outsideCurvatureRadius, 1.0);
    glm::dvec4 s2_center(0.0, 0.0, m_insideCurvatureRadius, 1.0);

    double t_s1[2];
    if (m_intersectSphere(
          modelRay, s1_center, m_outsideCurvatureRadius,
          t_s1[0], t_s1[1]))
    {
      for (int i = 0; i < 2; ++i) {
        if (t_s1[i] <= 0.0)
          continue;  // Skip roots behind the ray
        // Determine which roots are inside the cylinder
        if (glm::length(glm::dvec2(ray.point(t_s1[i]))) < m_radius) {
          // Determine which roots are on the correct hemisphere of the sphere
          // that defines the curvature of the lens
          if (std::abs(ray.point(t_s1[i]).z()) < std::abs(m_outsideCurvatureRadius)) {
          } else {
            t_s1[i] = DBL_MAX;
          }
        } else {
          t_s1[i] = DBL_MAX;
        }
      }
    }
    // Determine which of our roots is the closest intersection
    double t_outer =
      t_s1[0] < t_s1[1] ?
        t_s1[0] > 0.0 ?
          t_s1[0] :
          t_s1[1] > 0.0 ?
            t_s1[1] :
            DBL_MAX
        t_s1[1] > 0.0 ?
          t_s1[1] :
          t_s1[0] > 0.0 ?
            t_s1[0] :
            DBL_MAX;

    // TODO: Determine t_inner
    /*
    double t_s2 = math::intersectHemisphere(
        const Ray &ray, s2_center, m_outsideCurvatureRadius);
        */

    // Test for intersection with the cylinder defining the radius of the lens
    double t_cylinder = math::intersectAxisAlignedCylinder(modelRay, m_radius);
    if (t_cylinder != DBL_MAX) {
      // TODO: Determine if this intersection point is on the lens
      bool insideOuter =
        glm::length(ray.point(t_cylinder) - s1_center) < m_outsideCurvatureRadius;
      if (m_outsideCurvatureRadius > 0.0) {
        if (!insideOuter) {
          t_cylinder = DBL_MAX;
        }
      } else {
        if (insideOuter) {
          t_cylinder = DBL_MAX;
        }
      }
    }

    // Compute the normal on the cylinder
    glm::dvec4 cylinderNormal = glm::dvec4(
        glm::dvec2(modelRay.point(t)) - glm::dvec2(0.0, 0.0),
        0.0, 0.0);


    // Transform the normal back into world space
    normal = glm::normalize(m_normalMatrix * cylinderNormal);

    return t;
  }
} }
