#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/* Ensure definitions are only used by the compiler, and not by the assembler.
 */
#if defined(__ICCARM__) || defined(__CC_ARM) || defined(__GNUC__)
#  include <stdint.h>
extern uint32_t SystemCoreClock;
#endif

extern void vAssertCalled(const char*, int);

#define configENABLE_FPU 0
#define configENABLE_MPU 0

#define configUSE_PREEMPTION                    1
#define configUSE_PORT_OPTIMISED_TASK_SELECTION 0
#define configUSE_TICKLESS_IDLE                 0
#define configUSE_IDLE_HOOK                     0
#define configUSE_TICK_HOOK                     0
#define configCPU_CLOCK_HZ                      (SystemCoreClock)
#define configTICK_RATE_HZ                      ((TickType_t)1000)
#define configMAX_PRIORITIES                    (7)
#define configMINIMAL_STACK_SIZE                ((uint16_t)0x80)
#define configMAX_TASK_NAME_LEN                 (4)
#define configUSE_16_BIT_TICKS                  0
#define configTICK_TYPE_WIDTH_IN_BITS           TICK_TYPE_WIDTH_32_BITS
#define configIDLE_SHOULD_YIELD                 0
#define configUSE_TASK_NOTIFICATIONS            1
#define configTASK_NOTIFICATION_ARRAY_ENTRIES   1
#define configUSE_MUTEXES                       0
#define configUSE_RECURSIVE_MUTEXES             0
#define configUSE_COUNTING_SEMAPHORES           0
#define configUSE_ALTERNATIVE_API               0 /* Deprecated! */
#define configQUEUE_REGISTRY_SIZE               8
#define configUSE_QUEUE_SETS                    1
#define configUSE_TIME_SLICING                  0
#define configUSE_NEWLIB_REENTRANT              0
#define configENABLE_BACKWARD_COMPATIBILITY     0

#define configNUM_THREAD_LOCAL_STORAGE_POINTERS 5
#define configUSE_MINI_LIST_ITEM                1
#define configSTACK_DEPTH_TYPE                  uint16_t
#define configMESSAGE_BUFFER_LENGTH_TYPE        size_t
#define configHEAP_CLEAR_MEMORY_ON_FREE         1

/* Memory allocation related definitions. */
#define configSUPPORT_STATIC_ALLOCATION  1
#define configSUPPORT_DYNAMIC_ALLOCATION 0
#define configTOTAL_HEAP_SIZE            0

/* Hook function related definitions. */
#define configUSE_IDLE_HOOK                0
#define configUSE_TICK_HOOK                0
#define configCHECK_FOR_STACK_OVERFLOW     2
#define configUSE_MALLOC_FAILED_HOOK       0
#define configUSE_DAEMON_TASK_STARTUP_HOOK 0
#define configUSE_SB_COMPLETED_CALLBACK    0

/* Run time and task stats gathering related definitions. */
#define configGENERATE_RUN_TIME_STATS        0
#define configUSE_TRACE_FACILITY             0
#define configUSE_STATS_FORMATTING_FUNCTIONS 0

/* Co-routine related definitions. */
#define configUSE_CO_ROUTINES           0
#define configMAX_CO_ROUTINE_PRIORITIES 0

/* Software timer related definitions. */
#define configUSE_TIMERS 0
// #define configTIMER_TASK_PRIORITY    3
// #define configTIMER_QUEUE_LENGTH     10
// #define configTIMER_TASK_STACK_DEPTH configMINIMAL_STACK_SIZE

/* Interrupt nesting behaviour configuration. */
/* 
#define configKERNEL_INTERRUPT_PRIORITY [dependent of processor]
 #define configMAX_SYSCALL_INTERRUPT_PRIORITY \
   [dependent on processor and application]
 #define configMAX_API_CALL_INTERRUPT_PRIORITY \
   [dependent on processor and application]
*/

/* Define to trap errors during development. */
#define configASSERT(x) \
  if ((x) == 0) vAssertCalled(__FILE__, __LINE__)

/* FreeRTOS MPU specific definitions. */
// Cortex M0+ has no MPU
// #define configINCLUDE_APPLICATION_DEFINED_PRIVILEGED_FUNCTIONS 0
// #define configTOTAL_MPU_REGIONS                                8 /* Default value. */
// #define configTEX_S_C_B_FLASH                                  0x07UL /* Default value. */
// #define configTEX_S_C_B_SRAM                                   0x07UL /* Default value. */
// #define configENFORCE_SYSTEM_CALLS_FROM_KERNEL_ONLY            1
// #define configALLOW_UNPRIVILEGED_CRITICAL_SECTIONS             1
// #define configENABLE_ERRATA_837070_WORKAROUND                  1
// #define configUSE_MPU_WRAPPERS_V1                              0
// #define configPROTECTED_KERNEL_OBJECT_POOL_SIZE                10
// #define configSYSTEM_CALL_STACK_SIZE                           128

/* ARMv8-M secure side port related definitions. */
// #define secureconfigMAX_SECURE_CONTEXTS 5

/* Optional functions - most linkers will remove unused functions anyway. */
#define INCLUDE_vTaskPrioritySet             1
#define INCLUDE_uxTaskPriorityGet            1
#define INCLUDE_vTaskDelete                  1
#define INCLUDE_vTaskSuspend                 1
#define INCLUDE_xResumeFromISR               1
#define INCLUDE_vTaskDelayUntil              1
#define INCLUDE_vTaskDelay                   1
#define INCLUDE_xTaskGetSchedulerState       1
#define INCLUDE_xTaskGetCurrentTaskHandle    1
#define INCLUDE_uxTaskGetStackHighWaterMark  0
#define INCLUDE_uxTaskGetStackHighWaterMark2 0
#define INCLUDE_xTaskGetIdleTaskHandle       0
#define INCLUDE_eTaskGetState                0
#define INCLUDE_xEventGroupSetBitFromISR     1
#define INCLUDE_xTimerPendFunctionCall       0
#define INCLUDE_xTaskAbortDelay              0
#define INCLUDE_xTaskGetHandle               0
#define INCLUDE_xTaskResumeFromISR           1

#define vPortSVCHandler     SVC_Handler
#define xPortPendSVHandler  PendSV_Handler
#define xPortSysTickHandler SysTick_Handler

#if defined(__ICCARM__) || defined(__CC_ARM) || defined(__GNUC__)
void PreSleepProcessing(uint32_t ulExpectedIdleTime);
void PostSleepProcessing(uint32_t ulExpectedIdleTime);
#endif /* defined(__ICCARM__) || defined(__CC_ARM) || defined(__GNUC__) */

/* The configPRE_SLEEP_PROCESSING() and configPOST_SLEEP_PROCESSING() macros
allow the application writer to add additional code before and after the MCU is
placed into the low power state respectively. */
#if configUSE_TICKLESS_IDLE == 1
#  define configPRE_SLEEP_PROCESSING(__x__) \
    do {                                    \
      __x__ = 0;                            \
      PreSleepProcessing(__x__);            \
    } while (0)
#  define configPOST_SLEEP_PROCESSING PostSleepProcessing
#endif /* configUSE_TICKLESS_IDLE == 1 */

/* A header file that defines trace macro can be included here. */

#endif /* FREERTOS_CONFIG_H */