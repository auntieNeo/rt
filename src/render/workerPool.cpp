#include "workerPool.h"

namespace rt { namespace render {
  WorkerPool::WorkerPool(int numWorkers)
    : m_readyWorkers(), m_ready(), m_readyMutex()
  {
    for (int i = 0; i < numWorkers; ++i)
      m_workers.push_back(WorkerPtr(new Worker(this)));
  }

  WorkerPool::~WorkerPool()
  {
    // TODO: Join all of the worker threads
  }

  void WorkerPool::dispatch(TaskPtr task) {
    // Wait for a worker thread to become available
    std::unique_lock<std::mutex> lock(m_readyMutex);
    while (m_readyWorkers.size() <= 0) {  // Loop to avoid spurious wakeup
      m_ready.wait(lock);
    }
    // Run the task on an available worker
    m_readyWorkers.back()->run(task);
    m_readyWorkers.pop_back();
  }

  void WorkerPool::addReadyWorker(Worker *worker) {
    {
      // Add this worker to the list of ready workers
      std::unique_lock<std::mutex> lock(m_readyMutex);
      m_readyWorkers.push_back(worker);
    }
    // Wake up the main thread
    m_ready.notify_one();
  }
} }
