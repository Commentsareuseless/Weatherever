/**
 * @file Tasks.cpp
 * @author Commentsareuseless
 * @brief Definition of all created tasks
 * @date 2023-08-27
 *
 * @note 1 Functions declared with C linkage
 *         for ABI compatibility
 * Copyright: Macieg :)
 *
 */
#include "Tasks.hpp"

#include <cstddef>

extern "C" void TaskBackground(void*) {
  while (1) {
    // Infinite loop is acceptable here
    // because this task has low priority and
    // can be preempted by any task but Idle()
  }
}

extern "C" void TaskGatherData(void*) { return; }

extern "C" void TaskHandleDebugCli(void*) { return; }

namespace task
{

static constexpr std::array<Task, ID_MAX_TASKS> initTaskData() {
  std::array<Task, ID_MAX_TASKS> data{};

  data[ID_TASK_BACKGROUND] = {
      nullptr, TaskBackground, Priority::BACKGROUND, {"bkg"}, {}, {}};

  data[ID_TASK_GATHER_DATA] = {
      nullptr, TaskGatherData, Priority::HIGH, {"gdt"}, {}, {}};

  data[ID_TASK_HANDLE_DBG] = {
      nullptr, TaskHandleDebugCli, Priority::NORMAL, {"dbg"}, {}, {}};

  return data;
}

static std::array<Task, ID_MAX_TASKS> TaskStorage{initTaskData()};

int InitRtosTasks(void) {
  int failedTaskIdx{1};

  for (auto& taskDesc : TaskStorage) {
    taskDesc.handle = xTaskCreateStatic(
        taskDesc.callback, taskDesc.name.data(), taskDesc.stack.size(), nullptr,
        taskDesc.priority, taskDesc.stack.data(), &taskDesc.tcb);

    if (nullptr == taskDesc.handle) {
      // Got an error creating task
      return -failedTaskIdx;
    }
    ++failedTaskIdx;
  }

  return 0;
}

void StartScheduler(void) { vTaskStartScheduler(); }

TaskHandle_t GetTask(ID taskId) { return TaskStorage.at(taskId).handle; }

} // namespace task
