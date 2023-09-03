/**
 * @file Tasks.hpp
 * @author Commentsareuseless
 * @brief Declaration of all created tasks
 * @date 2023-08-27
 *
 * @note 1 Functions declared with C linkage
 *         for ABI compatibility
 * Copyright: Macieg :)
 *
 */
#pragma once

#include <cstdint>

enum TaskIDs : uint8_t
{
  ID_TASK_BACKGROUND,
  ID_TASK_GATHER_DATA,
  ID_TASK_HANDLE_DBG,

  ID_MAX_TASKS
};

/**
 * @brief Most of the processing is executed here.
 *        Basically all tasks that are not real-time critical
 *        should be executed in background
 */
extern "C" void TaskBackground(const void*);

/**
 * @brief This is high priority task which should not
 *        contain any blocking or expensive processing
 *        as it could starve other tasks.
 */
extern "C" void TaskGatherData(const void*);

/**
 * @brief Debug functionality could be placed in background task
 *        but executing it with separate task has an edge of
 *        being able to be suspended or completly disabled and removed
 *        in production build, so it has no affect on performance
 *        of application as a whole.
 */
extern "C" void TaskHandleDebugCli(const void*);
