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

#include <drv/CortexM0Plus.hpp>

#include <FreeRTOS.h>
#include <task.h>

#include "Tasks.hpp"

#define __noreturn __attribute__((noreturn))

extern "C" __noreturn void vAssertCalled(const char*, int);

int main() {

  drv::CortexM0Plus::InitHalSystem();
  drv::CortexM0Plus::InitPeripherals();

  task::InitRtosTasks();

  vTaskStartScheduler();

  /* If all is well, the scheduler will now be running, and the following
	line will never be reached.  If the following line does execute, then there
	was	insufficient FreeRTOS heap memory available for the idle and/or timer
	tasks to be created.  See the memory management section on the FreeRTOS web
	site, or the FreeRTOS tutorial books for more details. */
  for (;;) { ; }
}

void vAssertCalled(const char*, int) {
  while (1) {
    ; // Inifinite limbo :O
  }
}
