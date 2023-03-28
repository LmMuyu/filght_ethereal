#include "freertos_task.h"

task_linked_list_t *links;

static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void TaskFun(void *a);

#if (configSUPPORT_STATIC_ALLOCATION == 1)
TaskHandle_t *Create_Task_Static(TaskFunction_t *Task_Fn,
                                 const char *const taskName, /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
                                 const uint32_t Stack_Size,
                                 void *const pvParameters,
                                 priority_task_t uxPriority)
{
  StaticTask_t Buffer;
  StackType_t xStack[Stack_Size];

  Hal_Write_Buf("Create_Task_Static start\r\n");
  Hal_SendData();

  static TaskHandle_t xHandle;

  Hal_Write_Buf("xTaskCreateStatic start\r\n");
  Hal_SendData();

  xTaskCreateStatic(TaskFun, "rtos", 1024, NULL, 1, xStack, &Buffer);


  Hal_Write_Buf("xTaskCreateStatic create ok\r\n");
  Hal_SendData();

  static task_linked_list_t task;

  task.priority = uxPriority;
  task.TaskBuffer = &Buffer;
  task.Handle = xHandle;
  task.xStack = xStack;
  task.next = NULL;

  if (links == NULL)
    links = &task;
  else
  {
    task_linked_list_t *wlinks = links;

    while (links->next != NULL)
    {
      wlinks = links->next;
    }

    wlinks->next = &task;
  };

  Hal_Write_Buf("xTaskCreateStatic create ok\r\n");
  Hal_SendData();

  return (TaskHandle_t *)&xHandle;
}

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize)
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}

void TaskFun(void *a)
{

  Hal_Write_Buf("TaskFun start\r\n");
  Hal_SendData();
}

#endif
