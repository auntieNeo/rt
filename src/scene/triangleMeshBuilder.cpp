#include <cstdio>
#include <fstream>
#include <sstream>

#include "plyParser.hpp"
#include "plyLexer.h"

#include "triangleMeshBuilder.h"

namespace rt { namespace scene {
  TriangleMeshBuilder::TriangleMeshBuilder()
    : m_currentProperties(nullptr), m_vertexCount(0), m_faceCount(0), m_tupleCount(0)
  {
  }

  TriangleMeshBuilder::~TriangleMeshBuilder() {
  }

  void TriangleMeshBuilder::elementVertex(int count) {
    assert(m_vertexCount == 0); // TODO: handle this error properly
    m_vertexCount = count;
    m_currentProperties = &m_vertexProperties;
  }

  void TriangleMeshBuilder::elementFace(int count) {
    assert(m_faceCount == 0); // TODO: handle this error properly
    m_faceCount = count;
    m_currentProperties = &m_faceProperties;
  }

  void TriangleMeshBuilder::property(int isFloat, std::string identifier) {
    // TODO: Check for "x", "y", "z"

    // Ignore unknown identifiers
    m_currentProperties->push_back(PropertyType::UNKNOWN);
    fprintf(stderr, "Added unknown property '%s'. m_currentProperties->size(): %d\n",
        identifier.c_str(), m_currentProperties->size());
  }

  void TriangleMeshBuilder::xProperty(int isFloat) {
    assert(isFloat);  // Only support floating point coordinates
    assert(m_currentProperties == &m_vertexProperties);
    m_currentProperties->push_back(PropertyType::X);
    fprintf(stderr, "Added X property. m_currentProperties->size(): %d\n",
        m_currentProperties->size());
  }

  void TriangleMeshBuilder::yProperty(int isFloat) {
    assert(isFloat);  // Only support floating point coordinates
    assert(m_currentProperties == &m_vertexProperties);
    m_currentProperties->push_back(PropertyType::Y);
    fprintf(stderr, "Added Y property. m_currentProperties->size(): %d\n",
        m_currentProperties->size());
  }

  void TriangleMeshBuilder::zProperty(int isFloat) {
    assert(isFloat);  // Only support floating point coordinates
    assert(m_currentProperties == &m_vertexProperties);
    m_currentProperties->push_back(PropertyType::Z);
    fprintf(stderr, "Added Z property. m_currentProperties->size(): %d\n",
        m_currentProperties->size());
  }

  void TriangleMeshBuilder::propertyList(int isFloat, std::string identifier) {
    // Ignore unknown identifiers
    m_currentProperties->push_back(PropertyType::UNKNOWN);
    fprintf(stderr, "Added unknown list property. m_currentProperties->size(): %d\n",
        m_currentProperties->size());
  }

  void TriangleMeshBuilder::vertexIndicesPropertyList(int isFloat) {
    assert(!isFloat);  // Only support integers for vertex indices
    m_currentProperties->push_back(PropertyType::VERTEX_INDICES);
    fprintf(stderr, "Added vertex indices list property. m_currentProperties->size(): %d\n",
        m_currentProperties->size());
  }

  void TriangleMeshBuilder::tuple(const std::vector<Number> &value) {
    // NOTE: It is assumed that vertices come before faces
    // NOTE: No support for edges or any other element type besides faces and vertices
    if (m_tupleCount < m_vertexCount) {
      // Read the vertex data into our vertices
      assert(value.size() == m_vertexProperties.size());  // FIXME: do proper error checking
      glm::dvec4 vertex(0.0, 0.0, 0.0, 1.0);
      for (size_t i = 0; i < value.size(); ++i) {
        switch (m_vertexProperties[i]) {
          case PropertyType::X:
            vertex[0] = value[i].d;
            break;
          case PropertyType::Y:
            vertex[1] = value[i].d;
            break;
          case PropertyType::Z:
            vertex[2] = value[i].d;
            break;
          default:
            ;  // Ignore unknown properties
        }
      }
      fprintf(stderr, "Adding mesh vertex: %g, %g, %g\n",
          vertex[0], vertex[1], vertex[2]);
      m_vertices.push_back(vertex);
    } else if (m_tupleCount < m_vertexCount + m_faceCount) {
      // Read the vertex index data into triangles
      assert(m_faceProperties.size() == 1);  // Only vertex_indices supported
      glm::dvec4 p0(0.0, 0.0, 0.0, 1.0),
        p1(0.0, 0.0, 0.0, 1.0), p2(0.0, 0.0, 0.0, 1.0);
      for (size_t i = 0; i < value.size(); ++i) {
        switch (m_faceProperties[i]) {
          case PropertyType::VERTEX_INDICES:
            for (size_t j = i+1; j-(i+1) < value.at(i).i; ++j) {
              switch (j-(i+1)) {
                case 0:
                  p0 = m_vertices.at(value.at(j).i);
                  fprintf(stderr, "p0: %g, %g, %g\n",
                      p0[0], p0[1], p0[2]);
                  break;
                case 1:
                  p1 = m_vertices.at(value.at(j).i);
                  fprintf(stderr, "p1: %g, %g, %g\n",
                      p1[0], p1[1], p1[2]);
                  break;
                case 2:
                  p2 = m_vertices.at(value.at(j).i);
                  fprintf(stderr, "p2: %g, %g, %g\n",
                      p2[0], p2[1], p2[2]);
                  break;
              }
            }
            i += value.at(i).i;
            break;
          default:
            // Don't support unknown properties
            assert(false);
        }
      }
      m_triangles.push_back(TriangleMesh::Triangle(p0, p1, p2));
    }
    m_tupleCount += 1;
  }

  std::unique_ptr<TriangleMesh> TriangleMeshBuilder::build(const std::string &file,
      MaterialPropertiesPtr material,
      const glm::dvec3 &scale,
      const glm::dvec4 &position, const glm::dquat &orientation)
  {
    // Prepare the PLY scanner
    yyscan_t yyscanner;
    if (yylex_init(&yyscanner)) {
      assert(false);
    }

    // Set the scanner's file using the yyin macro
    std::stringstream ss;
    std::ifstream ifs;
    ifs.open(file, std::ifstream::in);
    ss << ifs.rdbuf();
    yy_scan_string(ss.str().c_str(), yyscanner);
    ifs.close();

    // Parse the PLY file
    yyparse(this, yyscanner);

    // Clean up
    yylex_destroy(yyscanner);

    // TODO: Compute each vertex normal as the average of adjacent face normals

    // TODO: Look for all triangles that share this vertex

    fprintf(stderr, "m_triangles.size(): %d\n", m_triangles.size());

    // Construct the triangle mesh
    std::unique_ptr<TriangleMesh> result(
        new TriangleMesh(m_triangles, material,
          scale, position, orientation));

    return std::move(result);
  }
} }
