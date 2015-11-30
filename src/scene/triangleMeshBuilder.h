#ifndef RT_SCENE_TRIANGLE_MESH_BUILDER_H_
#define RT_SCENE_TRIANGLE_MESH_BUILDER_H_

#include <map>
#include <string>
#include <vector>

#include "triangleMesh.h"

namespace rt { namespace scene {
  class TriangleMeshBuilder {
    public:
      union Number {
        long int i;
        double d;
      };

      enum PropertyType {
        X, Y, Z,
        VERTEX_INDICES,
        UNKNOWN
      };

    private:
      std::vector<glm::dvec4> m_vertices;
      std::vector<std::vector<size_t>> m_faces;
      std::map<size_t, std::vector<size_t>> m_vertexFaces;
      std::vector<TriangleMesh::Triangle> m_triangles;
      std::vector<PropertyType> m_vertexProperties, m_faceProperties;
      std::vector<PropertyType> *m_currentProperties;
      int m_vertexCount, m_faceCount;
      int m_tupleCount;
    public:
      TriangleMeshBuilder();
      ~TriangleMeshBuilder();

      void elementVertex(int count);
      void elementFace(int count);
      void property(int isFloat, std::string identifier);
      void xProperty(int isFloat);
      void yProperty(int isFloat);
      void zProperty(int isFloat);
      void propertyList(int isFloat, std::string identifier);
      void vertexIndicesPropertyList(int isFloat);
      void tuple(const std::vector<Number> &value);

      std::unique_ptr<TriangleMesh> build(const std::string &file,
          MaterialPropertiesPtr material,
          const glm::dvec3 &scale = glm::dvec3(1.0, 1.0, 1.0),
          const glm::dvec4 &position = glm::dvec4(0.0, 0.0, 0.0, 1.0),
          const glm::dquat &orientation = glm::dquat());

    private:
      glm::dvec4 m_computeNormal(size_t v);
      glm::dvec4 m_faceNormal(size_t f);
  };
} }

#endif
