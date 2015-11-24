#ifndef RT_RENDER_WORKER_POOL_H_
#define RT_RENDER_WORKER_POOL_H_

#include <memory>

#include "task.h"
#include "worker.h"

namespace rt { namespace render {
  class WorkerPool {
    friend Worker;
    private:
      std::vector<WorkerPtr> m_workers;
      std::vector<Worker *> m_readyWorkers;
      std::condition_variable m_ready;
      std::mutex m_readyMutex;
    public:
      WorkerPool(int numWorkers);
      ~WorkerPool();

      void dispatch(TaskPtr task);
    private:
      void addReadyWorker(Worker *worker);
  };
} }

#endif
