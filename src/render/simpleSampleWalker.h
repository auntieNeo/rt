#ifndef RT_RENDER_SIMPLE_SAMPLE_WALKER_H_
#define RT_RENDER_SIMPLE_SAMPLE_WALKER_H_

namespace rt { namespace render {
  template <class SampleDistribution>
  class SimpleSampleWalker {
    private:
      SampleDistribution m_samples;
      Pixel m_pixel;
    public:
      SimpleSampleWalker(Pixel pixel, SampleDistribution samples);
      ~SimpleSampleWalker();

      auto begin() { return m_samples.begin(); }
      auto end() { return m_samples.end(); }
  };

  template <class SampleDistribution>
  SimpleSampleWalker<SampleDistribution>::SimpleSampleWalker(
      Pixel pixel, SampleDistribution samples)
    : m_pixel(pixel),
      m_samples(samples)  // FIXME: Don't copy the sample distribution
  {
  }

  template <class SampleDistribution>
  SimpleSampleWalker<SampleDistribution>::~SimpleSampleWalker() {
  }
} }

#endif
