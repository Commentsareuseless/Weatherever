/**
 * @file MockFreeRTOS.hpp
 * @author Commentsareuseless
 * @brief FreeRTOS api mocks and fakes
 * @version 0.1
 * 
 * Copyright: Macieg :)
 * 
 */
#pragma once

#include "gmock/gmock.h"
#include <gmock/gmock.h>

#include <FreeRTOS.h>
#include <task.h>

namespace mock
{

class IFreeRTOS
{
public:
  virtual TaskHandle_t xTaskCreateStatic(TaskFunction_t pxTaskCode,
                                         const char* const pcName,
                                         const uint32_t ulStackDepth,
                                         void* const pvParameters,
                                         UBaseType_t uxPriority,
                                         StackType_t* const puxStackBuffer,
                                         StaticTask_t* const pxTaskBuffer) = 0;

  virtual ~IFreeRTOS() = default;
};

class FreeRTOS : public IFreeRTOS
{
public:
  MOCK_METHOD(TaskHandle_t,
              xTaskCreateStatic,
              (TaskFunction_t pxTaskCode,
               const char* const pcName,
               const uint32_t ulStackDepth,
               void* const pvParameters,
               UBaseType_t uxPriority,
               StackType_t* const puxStackBuffer,
               StaticTask_t* const pxTaskBuffer));
};

extern FreeRTOS* FreeRtosMock;

} // namespace mock