

#ifndef _HAL_QUEUE_
#define _HAL_QUEUE_
#ifdef __cplusplus
extern "C"
{
#endif

#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"

  typedef struct Queue_Node queue_node_t;
  typedef void *ElemType;

  typedef struct Queue_Node
  {
    ElemType *data;
    queue_node_t *next;
  } queue_node_t;

  typedef struct Queue_List
  {
    queue_node_t *front;
    queue_node_t *tail;
    int len;
  } queue_list_t;

  queue_list_t *QueueLists();
  void enqueue(queue_list_t *q, ElemType *data);
  bool dequeue(queue_list_t *q, ElemType *data);

#ifdef __cplusplus
}
#endif
#endif