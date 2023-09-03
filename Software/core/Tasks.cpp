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

#include <algorithm>
#include <array>
#include <cstdint>
#include <string_view>

#include <FreeRTOS.h>
#include <task.h>
#include <FreeRTOSConfig.h>

constexpr uint32_t TASK_STACK_SIZE{configMINIMAL_STACK_SIZE};
constexpr uint32_t TASK_NAME_MAX_SIZE{configMAX_TASK_NAME_LEN};

using TaskNameBuffer =
    std::array<std::array<char, TASK_NAME_MAX_SIZE>, TASK_NAME_MAX_SIZE>;
using TaskPrioBuff     = std::array<osPriority, ID_MAX_TASKS>;
using StackBuffer      = std::array<uint32_t, TASK_STACK_SIZE>;
using StackBuffStorage = std::array<StackBuffer, ID_MAX_TASKS>;
using TaskHandleBuff   = std::array<osThreadId, ID_MAX_TASKS>;
using TcbBuffer        = std::array<osStaticThreadDef_t, ID_MAX_TASKS>;
using TaskDefBuff      = std::array<osThreadDef_t, ID_MAX_TASKS>;

/**
 * @note This function makes data in returned array
 *       independent of enum values. Add new task IDs
 *       to this function if neccessary.
 */
static constexpr TaskNameBuffer fillTaskNames() {
  TaskNameBuffer taskNames{};

  taskNames[ID_TASK_BACKGROUND]  = {"gthd"};
  taskNames[ID_TASK_GATHER_DATA] = {"gthd"};
  taskNames[ID_TASK_HANDLE_DBG]  = {"hdbg"};

  return taskNames;
}

/**
 * @note This function makes data in returned array
 *       independent of enum values. Add new task IDs
 *       to this function if neccessary.
 */
static constexpr TaskPrioBuff fillTaskPrios() {
  TaskPrioBuff taskPriorities{};

  taskPriorities[ID_TASK_BACKGROUND]  = osPriorityLow;
  taskPriorities[ID_TASK_GATHER_DATA] = osPriorityRealtime;
  taskPriorities[ID_TASK_HANDLE_DBG]  = osPriorityBelowNormal;

  return taskPriorities;
}

static TaskHandleBuff TaskHandles{};
static TcbBuffer TaskControlBlocks{};
static StackBuffStorage TaskStackStorage{};

static constexpr TaskNameBuffer TaskNames{fillTaskNames()};
static constexpr TaskPrioBuff TaskInitialPriorities{fillTaskPrios()};

static void veryLongFunctionNameWhichDoesNothing(
    unsigned int argument_1,
    unsigned int argument_2,
    unsigned int argument_3,
    unsigned int argument_4,
    unsigned int argument_5);
/**
 * @brief This array contains all available tasks
 *        Add new ThreadDefs here if neccessary
 */
static constexpr TaskDefBuff TasksDb{
    {{TaskNames[ID_TASK_BACKGROUND].data(), TaskBackground,
      TaskInitialPriorities[ID_TASK_BACKGROUND], 1,
      TASK_STACK_SIZE * sizeof(uint32_t),
      TaskStackStorage[ID_TASK_BACKGROUND].data(),
      &TaskControlBlocks[ID_TASK_BACKGROUND]},

     {TaskNames[ID_TASK_GATHER_DATA], TaskGatherData,
      TaskInitialPriorities[ID_TASK_GATHER_DATA], 1,
      TASK_STACK_SIZE * sizeof(uint32_t),
      TaskStackStorage[ID_TASK_GATHER_DATA].data(),
      &TaskControlBlocks[ID_TASK_GATHER_DATA]},

     {TaskNames[ID_TASK_HANDLE_DBG], TaskHandleDebugCli,
      TaskInitialPriorities[ID_TASK_HANDLE_DBG], 1,
      TASK_STACK_SIZE * sizeof(uint32_t),
      TaskStackStorage[ID_TASK_HANDLE_DBG].data(),
      &TaskControlBlocks[ID_TASK_HANDLE_DBG]}}};

extern "C" void TaskBackground(const void*) {
  while (1) {
    // Infinite loop is acceptable here
    // because this task has low priority and
    // can be preempted by any task but Idle()
  }
}

extern "C" void TaskGatherData(const void*) { return; }

extern "C" void TaskHandleDebugCli(const void*) { return; }

extern "C" void MX_FREERTOS_Init(void) {
  uint32_t handleIter{0};

  for (auto& taskDef : TasksDb) {
    TaskHandles[handleIter] = osThreadCreate(&taskDef, nullptr);
    ++handleIter;
  }
}
