/**
 * @file CortexM0Plus.cpp
 * @author Commentsareuseless
 * @brief 
 * @date 2023-09-12
 * 
 * Copyright: Macieg :)
 * 
 */
#include "CortexM0Plus.hpp"

#include <main.h>
#include <crc.h>
#include <dma.h>
#include <gpio.h>
#include <i2c.h>
#include <rtc.h>
#include <spi.h>
#include <tim.h>
#include <usart.h>

// This function is defined in main.c (generated)
extern "C" void SystemClock_Config();

namespace drv
{

void CortexM0Plus::InitHalSystem() {
  HAL_Init();
  SystemClock_Config();
}

void CortexM0Plus::InitPeripherals() {
  MX_CRC_Init();
  MX_DMA_Init();
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_I2C2_Init();
  MX_RTC_Init();
  MX_SPI1_Init();
  MX_TIM16_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
}

} // namespace drv