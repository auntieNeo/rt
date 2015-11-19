#ifndef RT_RENDER_SAMPLE_H_
#define RT_RENDER_SAMPLE_H_

namespace rt { namespace render {
  class Sample {
    private:
      double m_x, m_y;

    public:
      Sample(double x, double y);
      ~Sample();

      double x() const { return m_x; }
      double y() const { return m_y; }
  };
} }

#endif
