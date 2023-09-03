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

#include <array>
#include <cstddef>
#include <cstdint>

constexpr uint32_t TASK_STACK_SIZE{configMINIMAL_STACK_SIZE};
constexpr uint32_t TASK_NAME_MAX_SIZE{configMAX_TASK_NAME_LEN};

typedef uint32_t TaskPrio_t;

using TaskNameBuffer = std::array<char, TASK_NAME_MAX_SIZE>;
using StackBuffer    = std::array<StackType_t, TASK_STACK_SIZE>;

struct Task {
  TaskHandle_t handle;
  TaskFunction_t callback;
  TaskPrio_t priority;
  TaskNameBuffer name;
  StaticTask_t tcb;
  StackBuffer stack;
};

static constexpr std::array<Task, ID_MAX_TASKS> initTaskData() {
  std::array<Task, ID_MAX_TASKS> data{};

  data[ID_TASK_BACKGROUND] = {
      nullptr, TaskBackground, TaskPriority::BACKGROUND, {"bkg"}, {}, {}};

  data[ID_TASK_GATHER_DATA] = {
      nullptr, TaskGatherData, TaskPriority::HIGH, {"gdt"}, {}, {}};

  data[ID_TASK_HANDLE_DBG] = {
      nullptr, TaskHandleDebugCli, TaskPriority::NORMAL, {"dbg"}, {}, {}};

  return data;
}

static std::array<Task, ID_MAX_TASKS> TaskStorage{initTaskData()};

extern "C" void TaskBackground(void*) {
  while (1) {
    // Infinite loop is acceptable here
    // because this task has low priority and
    // can be preempted by any task but Idle()
  }
}

extern "C" void TaskGatherData(void*) { return; }

extern "C" void TaskHandleDebugCli(void*) { return; }

void InitRtosTasks(void) {
  for (auto& taskDesc : TaskStorage) {
    taskDesc.handle = xTaskCreateStatic(
        taskDesc.callback, taskDesc.name.data(), taskDesc.stack.size(), nullptr,
        taskDesc.priority, taskDesc.stack.data(), &taskDesc.tcb);
  }
}
