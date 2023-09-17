/**
 * @file MockFreeRTOS.hpp
 * @author Commentsareuseless
 * @brief FreeRTOS api mocks and fakes
 * @version 0.1
 * 
 * Copyright: Macieg :)
 * 
 */
#include "MockFreeRTOS.hpp"

namespace mock
{
/*
=========== The hackest hack of all hacks ============
While using this mock we're balancing on segfault edge
and all of this, just to create proper mock of C-style
function.

Pointer is used instead of normal static object declaration
so we can create and destroy mock object during single
test case.
*/
FreeRTOS* FreeRtosMock{nullptr};
} // namespace mock

extern "C" TaskHandle_t xTaskCreateStatic(TaskFunction_t pxTaskCode,
                                          const char* const pcName,
                                          const uint32_t ulStackDepth,
                                          void* const pvParameters,
                                          UBaseType_t uxPriority,
                                          StackType_t* const puxStackBuffer,
                                          StaticTask_t* const pxTaskBuffer) {
  return mock::FreeRtosMock->xTaskCreateStatic(pxTaskCode, pcName, ulStackDepth,
                                               pvParameters, uxPriority,
                                               puxStackBuffer, pxTaskBuffer);
}

extern "C" void vTaskStartScheduler(void) { return; }
