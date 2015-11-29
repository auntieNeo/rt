#include <cstdio>

#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "triangleMesh.h"

namespace rt { namespace scene {
  TriangleMesh::TriangleMesh(const std::vector<Triangle> &triangles,
      MaterialPropertiesPtr material,
      const glm::dvec3 &scale,
      const glm::dvec4 &position, const glm::dquat &orientation)
    : DrawableObject(material, position, orientation),
    m_triangles(triangles), m_scale(scale), m_bvh(nullptr)
  {
    m_bvh = std::unique_ptr<BVH<Triangle>>(new BVH<Triangle>(triangles));

    m_worldModelMatrix = 
      glm::scale(glm::dmat4(1.0), 1.0 / m_scale) *
      glm::mat4_cast(glm::inverse(this->orientation())) *
      glm::translate(glm::dmat4(1.0), -glm::dvec3(this->position()));
    m_normalMatrix =
      glm::inverseTranspose(
          glm::mat4_cast(this->orientation()) *
          glm::scale(glm::dmat4(1.0), m_scale));
  }

  TriangleMesh::~TriangleMesh() {
  }

  double TriangleMesh::intersect(const Ray &ray, glm::dvec4 &normal) const {
    // Transform the ray into model space
    Ray objectRay = m_worldModelMatrix * ray;
    double t = m_bvh->intersect(objectRay, normal);
    if (t != DBL_MAX) {
      // TODO: Transform the normal back into world space
      normal = glm::normalize(m_normalMatrix * normal);
    }
    return t;
    /*
    // TODO: Use a BVH tree to speed up intersection
    glm::dvec4 triangleNormal;
    double t;
    int i = 0;
    for (Triangle triangle : m_triangles) {
      // Check for intersections on each triangle
      t = triangle.intersect(ray, triangleNormal);
      if (t != DBL_MAX) {
        normal = triangleNormal;
        return t;
      }
    }
    return DBL_MAX;
    */
  }

  TriangleMesh::Triangle::Triangle(
      const glm::dvec4 &p0, const glm::dvec4 &p1, const glm::dvec4 &p2)
  {
    m_p[0] = p0;
    m_p[1] = p1;
    m_p[2] = p2;
  }

  TriangleMesh::Triangle::~Triangle() {
  }

  double TriangleMesh::Triangle::intersect(
      const Ray &ray, glm::dvec4 &normal) const
  {
    // Calculate a plane vector for the points in this triangle
    glm::dvec3 n = glm::cross(glm::dvec3(m_p[1] - m_p[0]), glm::dvec3(m_p[2] - m_p[0]));
    glm::dvec4 l(n, -glm::dot(n, glm::dvec3(m_p[0])));

    double denominator = glm::dot(l, ray.direction());
    if (denominator == 0.0) {
      // The ray is parallel to the plane
      return DBL_MAX;
    }

    double t = -glm::dot(l, ray.origin()) / denominator;
    glm::dvec3 R(ray.point(t) - m_p[0]);
    glm::dvec3 Q1(m_p[1] - m_p[0]);
    glm::dvec3 Q2(m_p[2] - m_p[0]);

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
      return DBL_MAX;
    }

    // TODO: Compute the normal from the vertex normals weighted by the
    // barycentric coordinates

    normal = glm::dvec4(n, 0.0);  // NOTE: this is a normal for flat shading

    return t;
  }

  AABB TriangleMesh::Triangle::bbox() const {
    AABB result;
    int i = 0;
    for (glm::dvec4 p : m_p) {
      ++i;
      result.min()[0] = std::min(result.min()[0], p[0]);
      result.min()[1] = std::min(result.min()[1], p[1]);
      result.min()[2] = std::min(result.min()[2], p[2]);
      result.max()[0] = std::max(result.max()[0], p[0]);
      result.max()[1] = std::max(result.max()[1], p[1]);
      result.max()[2] = std::max(result.max()[2], p[2]);
    }
    assert(i == 3);
    return result;
  }

  glm::dvec4 TriangleMesh::Triangle::centroid() const {
    return (1.0 / 3.0) * (m_p[0] + m_p[1] + m_p[2]);
  }
} }
