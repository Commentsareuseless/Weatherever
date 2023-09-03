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

#include "FreeRTOSConfig.h"
#include <cstdint>

#include <FreeRTOS.h>
#include <task.h>

enum TaskIDs : uint8_t
{
  ID_TASK_BACKGROUND,
  ID_TASK_GATHER_DATA,
  ID_TASK_HANDLE_DBG,

  ID_MAX_TASKS
};

enum TaskPriority : uint8_t
{
  IDLE       = tskIDLE_PRIORITY,
  BACKGROUND = tskIDLE_PRIORITY + 1,
  LOW        = tskIDLE_PRIORITY + 2,
  NORMAL     = tskIDLE_PRIORITY + 3,
  HIGH       = tskIDLE_PRIORITY + 4,
  REAL_TIME  = configMAX_PRIORITIES - 1
};

/**
 * @brief This should be called before vTaskStartScheduler()
 *        so all tasks can be properly created and initialized
 * 
 */
void InitRtosTasks(void);

/**
 * @brief Get handle of specified task ID
 * 
 * @param taskId - Task ID
 */
TaskHandle_t GetTask(TaskIDs taskId);

/**
 * @brief Most of the processing is executed here.
 *        Basically all tasks that are not real-time critical
 *        should be executed in background
 */
extern "C" void TaskBackground(void*);

/**
 * @brief This is high priority task which should not
 *        contain any blocking or expensive processing
 *        as it could starve other tasks.
 */
extern "C" void TaskGatherData(void*);

/**
 * @brief Debug functionality could be placed in background task
 *        but executing it with separate task has an edge of
 *        being able to be suspended or completly disabled and removed
 *        in production build, so it has no affect on performance
 *        of application as a whole.
 */
extern "C" void TaskHandleDebugCli(void*);
