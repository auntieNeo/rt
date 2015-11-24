#ifndef RT_RENDER_WORKER_H_
#define RT_RENDER_WORKER_H_

#include <condition_variable>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

#include "task.h"

namespace rt { namespace render {
  class WorkerPool;
  class Worker {
    private:
      WorkerPool *m_pool;
      TaskPtr m_task;
      std::mutex m_taskMutex;
      std::condition_variable m_taskReady;
      std::thread m_thread;
      bool m_join;
    public:
      Worker(WorkerPool *pool);
      ~Worker();

      void run(TaskPtr task);
      void join();

    private:
      void m_run();
  };

  typedef std::shared_ptr<Worker> WorkerPtr;
} }

#endif
