#ifndef RT_RENDER_SAMPLE_H_
#define RT_RENDER_SAMPLE_H_

#include <glm/glm.hpp>

namespace rt { namespace render {
  class Sample {
    private:
      glm::dvec2 m_position;

    public:
      Sample(double x, double y);
      ~Sample();

      double x() const { return m_position[0]; }
      double y() const { return m_position[1]; }

      glm::dvec2 position() const { return m_position; }
  };
} }

#endif
