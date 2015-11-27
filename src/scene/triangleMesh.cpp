#include <cstdio>

#include "triangleMesh.h"

namespace rt { namespace scene {
  TriangleMesh::TriangleMesh(const std::vector<Triangle> &triangles,
      MaterialPropertiesPtr material,
      const glm::dvec4 &position, const glm::dquat &orientation)
    : DrawableObject(material, position, orientation), m_triangles(triangles)
  {
  }

  TriangleMesh::~TriangleMesh() {
  }

  double TriangleMesh::intersect(const Ray &ray, glm::dvec4 &normal) const {
    // TODO: Use a BVH tree to speed up intersection
    glm::dvec4 triangleNormal;
    double t;
    for (Triangle triangle : m_triangles) {
      // Check for intersections on each triangle
      t = triangle.intersect(ray, triangleNormal);
      if (t != 0.0) {
        fprintf(stderr, "Triangle intersection!\n");
        normal = triangleNormal;
        return t;
      }
    }
    return 0.0;
  }

  TriangleMesh::Triangle::Triangle(
      const glm::dvec4 &p0, const glm::dvec4 &p1, const glm::dvec4 &p2)
    : m_p0(p0), m_p1(p1), m_p2(p2)
  {
  }

  TriangleMesh::Triangle::~Triangle() {
  }

  double TriangleMesh::Triangle::intersect(
      const Ray &ray, glm::dvec4 &normal) const
  {
    // Calculate a plane vector for the points in this triangle
    glm::dvec3 n = glm::cross(glm::dvec3(m_p1 - m_p0), glm::dvec3(m_p2 - m_p0));
    glm::dvec4 l(n, -glm::dot(n, glm::dvec3(m_p0)));

    double denominator = glm::dot(l, ray.direction());
    if (denominator == 0.0) {
      // The ray is parallel to the plane
      return 0.0;
    }

    double t = -glm::dot(l, ray.origin()) / denominator;
    glm::dvec3 R(ray.point(t) - m_p0);
    glm::dvec3 Q1(m_p1 - m_p0);
    glm::dvec3 Q2(m_p2 - m_p0);

    // Solve the system of linear equations for the barycentric coordinates
    glm::dvec2 w =
      glm::inverse(
        glm::mat2(glm::dot(Q1, Q1), glm::dot(Q1, Q2),
                  glm::dot(Q1, Q2), glm::dot(Q2, Q2))) *
      glm::dvec2(glm::dot(R, Q1), glm::dot(R, Q2));

    if ((w[0] < 0.0) ||
        (w[1] < 0.0) ||
        (w[0] + w[1] > 1.0)) {
      // The ray intersects outside of the triangle
      return 0.0;
    }

    // TODO: Compute the normal from the vertex normals weighted by the
    // barycentric coordinates

    normal = glm::dvec4(n, 0.0);  // NOTE: this is a normal for flat shading

    return t;
  }
} }
