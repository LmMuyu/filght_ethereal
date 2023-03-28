#ifndef _FreeRtos_Task_
#define _FreeRtos_Task_
#ifdef __cplusplus
extern "C"
{
#endif

#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "FreeRTOSConfig.h"
#include "usb.h"

  void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize);

  typedef struct Task_Linked_List task_linked_list_t;

  typedef enum Priority_Task
  {
    Task_Priority_Level_1 = 1,
    Task_Priority_Level_2 = 2,
    Task_Priority_Level_4 = 4,
    Task_Priority_Level_6 = 6,
    Task_Priority_Level_8 = 8,
    Task_Priority_Level_10 = 10,
  } priority_task_t;

  struct Task_Linked_List
  {
    priority_task_t priority;
    TaskHandle_t Handle;
    StaticTask_t *TaskBuffer;
    StackType_t *xStack;
    task_linked_list_t *next;
  };

  extern task_linked_list_t *links;

#if (configSUPPORT_STATIC_ALLOCATION == 1)
  /**
   *
   @brief 创建线程任务
   @param Task_Fn  运行函数
   @param pcName 线程名称
   @param Stack_Size 栈大小
   @param pvParameters 运行函数的参数
   @param uxPriority 线程优先级
   @return  task_linked_list_t 线程信息
   * */

  TaskHandle_t *Create_Task_Static(
      TaskFunction_t *Task_Fn,
      const char *const taskName,
      const uint32_t Stack_Size,
      void *const pvParameters,
      priority_task_t uxPriority);

#endif

#ifdef __cplusplus
}
#endif
#endif
