#ifndef RT_SCENE_BVH_H_
#define RT_SCENE_BVH_H_

// XXX
#include <cstdio>

#include <algorithm>
#include <cstring>
#include <memory>
#include <utility>
#include <vector>

#include "aabb.h"
#include "../ray.h"

namespace rt { namespace scene {
  /**
   * Bounding Volume Hierarchy template class.
   *
   * Code based on notes from "Notes on efficient ray tracing", Solomon Boulos
   * <https://graphics.stanford.edu/~boulos/papers/efficient_notes.pdf>
   */
  template <class Surface>
  class BVH {
    private:
      std::shared_ptr<BVH<Surface>> m_left, m_right;
      const Surface *m_surface;
      AABB m_bbox;

      BVH(const std::vector<Surface> &surfaces, int axis);
      BVH(const Surface **surfaces, int numSurfaces, int axis);
    public:
      BVH(const std::vector<Surface> &surfaces);
      ~BVH();

      double intersect(const Ray &ray, glm::dvec4 &normal) const;

    private:
      static AABB m_surround(const Surface **surfaces, int numSurfaces);
      static int m_split(const Surface **surfaces, int numSurfaces,
          double pivot, int axis);
  };

  template <class Surface>
  BVH<Surface>::BVH(const std::vector<Surface> &surfaces)
    : BVH<Surface>(surfaces, 0)  // Start on the X axis
  {}

  template <class Surface>
  BVH<Surface>::BVH(const std::vector<Surface> &surfaces, int axis)
  {
    // Make an array of pointers to these surfaces
    const Surface **surfacePtrs = new const Surface *[surfaces.size()];
    for (size_t i = 0; i < surfaces.size(); ++i) {
      surfacePtrs[i] = &surfaces.data()[i];
    }

    *this = BVH<Surface>(surfacePtrs, surfaces.size(), axis);

    delete surfacePtrs;
  }

  template <class Surface>
  BVH<Surface>::BVH(const Surface **surfaces, int numSurfaces, int axis)
    : m_left(nullptr), m_right(nullptr),
      m_surface(nullptr)
  {
    fprintf(stderr, "BVH constructor, numSurfaces: %d\n", numSurfaces);

    // Handle the case with no surfaces
    if (numSurfaces <= 0) return;

    // Make a bounding box around all of the surfaces
    m_bbox = m_surround(surfaces, numSurfaces);
    fprintf(stderr, "m_bbox.min(): %g, %g, %g\n",
        m_bbox.min()[0], m_bbox.min()[1], m_bbox.min()[2]);
    fprintf(stderr, "m_bbox.max(): %g, %g, %g\n",
        m_bbox.max()[0], m_bbox.max()[1], m_bbox.max()[2]);

    // Handle the base case of one surface
    if (numSurfaces == 1)  {
      m_surface = surfaces[0];
      return;
    }

    // Sort the surfaces around a pivot point
    glm::dvec3 pivot = (m_bbox.max() + m_bbox.min()) / 2.0;
    int midPoint = m_split(surfaces, numSurfaces, pivot[axis], axis);
    if ((midPoint == numSurfaces) || (midPoint == 0)) {
      // Make an arbitrary split for surfaces with identical centroids
      midPoint = numSurfaces / 2;
    }
    /*
    for (int i = 0; i < numSurfaces; ++i) {
      fprintf(stderr, "surfaces[i]->centroid(): %g, %g, %g\n",
          surfaces[i]->centroid()[0],
          surfaces[i]->centroid()[1],
          surfaces[i]->centroid()[2]);
    }
    */

    // Recursively construct the BVH while using a different axis
    int nextAxis = (axis + 1) % 3;
    m_left = std::shared_ptr<BVH<Surface>>(
        new BVH<Surface>(surfaces, midPoint, nextAxis));
    m_right = std::shared_ptr<BVH<Surface>>(
        new BVH<Surface>(&surfaces[midPoint], numSurfaces - midPoint, nextAxis));
  }

  template <class Surface>
  BVH<Surface>::~BVH() {
  }

  template <class Surface>
  double BVH<Surface>::intersect(const Ray &ray, glm::dvec4 &normal) const {
    if ((!m_left && !m_surface) || !m_bbox.intersect(ray))
      return DBL_MAX;

    if (m_left) {
      assert(m_right);
      glm::dvec4 normal_left, normal_right;
      double t_left = m_left->intersect(ray, normal_left);
      double t_right = m_right->intersect(ray, normal_right);

      normal = t_left < t_right ? normal_left : normal_right;

      return std::min(t_left, t_right);
    }
   
    assert(m_surface);
    return m_surface->intersect(ray, normal);
  }

  template <class Surface>
  AABB BVH<Surface>::m_surround(const Surface **surfaces, int numSurfaces)
  {
    fprintf(stderr, "m_surround(), numSurfaces: %d\n", numSurfaces);
    AABB result;
    assert(numSurfaces > 0);
    for (int i = 0; i < numSurfaces; ++i) {
      auto sbbox = surfaces[i]->bbox();
      /*
      fprintf(stderr, "sbbox.min(): %g, %g, %g\n",
          sbbox.min()[0], sbbox.min()[1], sbbox.min()[2]);
      fprintf(stderr, "sbbox.max(): %g, %g, %g\n",
          sbbox.max()[0], sbbox.max()[1], sbbox.max()[2]);
          */
      result.min()[0] = std::min(result.min()[0], sbbox.min()[0]);
      result.min()[1] = std::min(result.min()[1], sbbox.min()[1]);
      result.min()[2] = std::min(result.min()[2], sbbox.min()[2]);
      result.max()[0] = std::max(result.max()[0], sbbox.max()[0]);
      result.max()[1] = std::max(result.max()[1], sbbox.max()[1]);
      result.max()[2] = std::max(result.max()[2], sbbox.max()[2]);
    }
    return result;
  }

  template <class Surface>
  int BVH<Surface>::m_split(const Surface **surfaces, int numSurfaces,
      double pivot, int axis)
  {
    // Sort the surfaces by their centroid with respect to the given axis.
    // Sorting is done around the given pivot point similar to the quicksort
    // algorithm.
    int j = numSurfaces - 1;
    int i = 0;
    const Surface *temp;
    do {
      while ((i < j) && (surfaces[i]->centroid()[axis] <= pivot)) ++i;
      while ((i < j) && (pivot < surfaces[j]->centroid()[axis])) --j;
      if (i < j) {
        temp = surfaces[i];
        surfaces[i] = surfaces[j];
        surfaces[j] = temp;
      } else if (i == j) {
        if (surfaces[i]->centroid()[axis] <= pivot)
          ++i;
      }
    } while (i < j);
#ifndef NDEBUG
    for (int k = 0; k < numSurfaces; ++k) {
      if (k < i)
        assert(surfaces[k]->centroid()[axis] <= pivot);
      else
        assert(surfaces[k]->centroid()[axis] > pivot);
    }
#endif
    return i;
  }
} }

#endif
