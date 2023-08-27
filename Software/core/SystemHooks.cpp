/**
 * @file SystemHooks.cpp
 * @author Commentsareuseless
 * @brief Implementation of hooks supported by FreeRTOS
 * @date 2023-08-26
 * 
 * Copyright: Macieg :)
 * 
 */
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

extern "C" void vApplicationGetIdleTaskMemory(
    StaticTask_t** ppxIdleTaskTCBBuffer,
    StackType_t** ppxIdleTaskStackBuffer,
    uint32_t* pulIdleTaskStackSize);

extern "C" void vApplicationIdleHook(void);
extern "C" void vApplicationStackOverflowHook(TaskHandle_t xTask,
                                              signed char* pcTaskName);

/* USER CODE BEGIN 2 */
__weak extern "C" void vApplicationIdleHook(void) {
  /* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
   to 1 in FreeRTOSConfig.h. It will be called on each iteration of the idle
   task. It is essential that code added to this hook function never attempts
   to block in any way (for example, call xQueueReceive() with a block time
   specified, or call vTaskDelay()). If the application makes use of the
   vTaskDelete() API function (as this demo application does) then it is also
   important that vApplicationIdleHook() is permitted to return to its calling
   function, because it is the responsibility of the idle task to clean up
   memory allocated by the kernel to any task that has since been deleted. */
}

__weak extern "C" void vApplicationStackOverflowHook(TaskHandle_t xTask,
                                                     signed char* pcTaskName) {
  /* Run time stack overflow checking is performed if
   configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2. This hook function is
   called if a stack overflow is detected. */
}

__weak extern "C" void PreSleepProcessing(uint32_t ulExpectedIdleTime) {
  /* place for user code */
}

__weak extern "C" void PostSleepProcessing(uint32_t ulExpectedIdleTime) {
  /* place for user code */
}

static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory(StaticTask_t** ppxIdleTaskTCBBuffer,
                                   StackType_t** ppxIdleTaskStackBuffer,
                                   uint32_t* pulIdleTaskStackSize) {
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}

/* 	Call vTaskSwitchContext() from a function with the used attribute set
    so that link time optimisation does not remove the symbol.*/
static void __attribute__((used)) vPortNoOptimize() { vTaskSwitchContext(); }
