#ifndef RT_RENDER_TASK_H_
#define RT_RENDER_TASK_H_

#include <memory>

namespace rt { namespace render {
  class Task {
    public:
      Task();
      virtual ~Task();

      virtual void run() = 0;
  };

  typedef std::shared_ptr<Task> TaskPtr;
} }

#endif
