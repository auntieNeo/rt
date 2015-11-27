#ifndef RT_SCENE_TRIANGLE_MESH_H_
#define RT_SCENE_TRIANGLE_MESH_H_

#include <memory>
#include <vector>

#include "drawableObject.h"

namespace rt { namespace scene {
  class TriangleMesh : public DrawableObject {
    public:
      class Triangle;
    private:
      std::vector<Triangle> m_triangles;

    public:
      TriangleMesh(const std::vector<Triangle> &triangles,
          MaterialPropertiesPtr material,
          const glm::dvec4 &position = glm::dvec4(0.0, 0.0, 0.0, 1.0),
          const glm::dquat &orientation = glm::dquat());
      ~TriangleMesh();

      virtual double intersect(const Ray &ray, glm::dvec4 &normal) const;

      class Triangle {
        private:
          glm::dvec4 m_p0, m_p1, m_p2;
        public:
          Triangle(const glm::dvec4 &p0, const glm::dvec4 &p1, const glm::dvec4 &p2);
          ~Triangle();

          double intersect(const Ray &ray, glm::dvec4 &normal) const;
      };
  };

  typedef std::shared_ptr<TriangleMesh> TriangleMeshPtr;
} }

#endif
