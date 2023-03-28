#include "freertos_mes_queue.h"

QueueHandle_t mes_queue;

void New_Mes_Queue()
{
  mes_queue = xQueueCreate(QUEUE_LEN, (UBaseType_t)sizeof(queue_node_t));

  if (mes_queue == NULL)
  {
    Error_Handler();
  }
}

void Add_Queue_Node(queue_node_t *node)
{
  if (uxQueueSpacesAvailable(mes_queue) > 0)
  {
    xQueueSend(mes_queue, (void *)node, (TickType_t)0);
  }
}

queue_rec_state_t Queue_Receive(queue_node_t *node)
{
  if (uxQueueSpacesAvailable(mes_queue) > 0)
  {
    xQueueReceive(mes_queue, (void *)node, (TickType_t)0);

    return REC_OK;
  }
  else
  {
    return REC_NULL;
  }
}
