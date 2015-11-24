#include <cassert>

#include "workerPool.h"

#include "worker.h"

namespace rt { namespace render {
  Worker::Worker(WorkerPool *pool)
    : m_pool(pool), m_task(nullptr), m_taskReady(),
    m_thread([this] { this->m_run(); })
  {
  }

  Worker::~Worker()
  {
    this->join();
  }

  void Worker::run(TaskPtr task) {
    {
      // Set the task to be run
      std::unique_lock<std::mutex> lock(m_taskMutex);
      assert(!m_task);
      m_task = task;
    }
    // Wake up the worker thread
    m_taskReady.notify_one();
  }

  void Worker::join() {
    {
      // Lock the task mutex to set the join flag
      std::unique_lock<std::mutex> lock(m_taskMutex);
      m_join = true;
    }
    // Wake up the worker thread
    m_taskReady.notify_one();
  }

  void Worker::m_run() {
    while (true) {
      // Lock the task mutex early to avoid racing the main thread
      std::unique_lock<std::mutex> lock(m_taskMutex);
      // Add this worker to the queue of ready workers
      m_pool->addReadyWorker(this);
      // Wait for a task to be assigned to this worker
      while (!m_task && !m_join) {  // Loop to avoid spurious wakeup
        m_taskReady.wait(lock);
      }
      if (m_join)
        break;  // Join the main thread
      // Run the task
      m_task->run();
      m_task = nullptr;
    }
  }
} }
