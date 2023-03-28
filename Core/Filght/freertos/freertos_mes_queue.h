#ifndef _HAL_RTOS_MES_QUEUE
#define _HAL_RTOS_MES_QUEUE
#ifdef __cplusplus
extern "C"
{
#endif
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "FreeRTOSConfig.h"
#include "queue.h"

#define QUEUE_LEN 64

  typedef struct queue_node queue_node_t;

  typedef enum queue_tag
  {
    motor_tag = 0x00,
    bmp_tag = 0x01,
    other_tag = 0xff,
  } queue_tag_t;

  typedef enum
  {
    REC_OK,
    REC_NULL,
    REC_ERROR,
  } queue_rec_state_t;

  struct queue_node
  {
    queue_tag_t tag[20];
    void *pdata;
  };

  extern QueueHandle_t mes_queue;

  void New_Mes_Queue();
  void Add_Queue_Node(queue_node_t *node);
  queue_rec_state_t Queue_Receive(queue_node_t *node);

#ifdef __cplusplus
}
#endif
#endif