/**
 * @file main.cpp
 * @author Commentsareuselss
 * @brief Program entry point
 * @date 2023-09-03
 * 
 * Copyright Macieg :) 2023
 * 
 */
#include <cstdio>

#include <main.h>

#include <crc.h>
#include <dma.h>
#include <gpio.h>
#include <i2c.h>
#include <rtc.h>
#include <spi.h>
#include <tim.h>
#include <usart.h>

#include <FreeRTOS.h>
#include <task.h>

#define __noreturn __attribute__((noreturn))

extern "C" __noreturn void vAssertCalled(const char*, int);
extern "C" void SystemClock_Config(void);

int main() {
  HAL_Init();
  SystemClock_Config();

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

  vTaskStartScheduler();

  while (1) { ; }
}

void vAssertCalled(const char*, int) {
  while (1) {
    ; // Inifinite limbo :O
  }
}
