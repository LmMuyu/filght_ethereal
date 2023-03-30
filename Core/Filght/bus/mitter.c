#include "mitter.h"

// void Task_Run_Fnction(void *args);

// StackType_t xStake[Stack_Size];
// StaticTask_t Handler_Task;
// TaskHandle_t queue_handle_task;

// const char *Mitter_Name = "Receive_queue";

// void Queue_Receive_Handle()
// {
//   queue_handle_task = xTaskCreateStatic(Task_Run_Fnction, Mitter_Name, Stack_Size, NULL, Task_Priority_Level_10, xStake, &Handler_Task);

//   if (queue_handle_task == NULL)
//   {
//     Error_Handler();
//   }
// }

// void Task_Run_Fnction(void *args)
// {
//   queue_node_t *node;

//   New_Mes_Queue();
//   mitt_t mitt = Mitter_Bus();

//   while (1)
//   {
//     queue_rec_state_t state = Queue_Receive(node);

//     if (state == REC_OK)
//     {
//       mitt->mitt_emit(node->tag, node);
//     }

//     vTaskDelay(pdMS_TO_TICKS(15));
//   }
// }