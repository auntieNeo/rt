#ifndef RT_SCENE_LENS_H_
#define RT_SCENE_LENS_H_

#include "drawableObject.h"

namespace rt { namespace scene {
  class Lens : public DrawableObject {
    private:
      glm::dmat4 m_worldModelMatrix, m_normalMatrix;
      double m_radius, m_outsideCurvatureRadius, m_insideCurvatureRadius,
             m_thickness;
    public:
      Lens(double radius, double outsideCurvatureRadius,
          double insideCurvatureRadius, double thickness,
          MaterialPropertiesPtr material,
          glm::dvec4 position = glm::dvec4(0.0, 0.0, 0.0, 1.0),
          glm::dquat orientation = glm::dquat());
      ~Lens();

      double intersect(const Ray &ray, glm::dvec4 &normal) const;
  };
} }

#endif
