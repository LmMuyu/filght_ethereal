#include "queue.h"

queue_list_t *QueueLists()
{
  static queue_list_t q_list;

  q_list.front = (queue_node_t *)malloc(sizeof(queue_node_t));
  q_list.tail = (queue_node_t *)malloc(sizeof(queue_node_t));

  q_list.front = NULL;
  q_list.tail = NULL;

  return &q_list;
}

void enqueue(queue_list_t *q, ElemType *data)
{
  queue_node_t node = {
      node.data = data,
      .next = (queue_node_t *)malloc(sizeof(queue_node_t)),
  };

  if (NULL == q->front)
  {
    q->tail = q->front = &node;
  }
  else
  {
    q->tail->next = &node;
    q->tail = &node;
  }

  q->len++;
}

bool dequeue(queue_list_t *q, ElemType *data)
{
  if (q->len == 0)
  {
    return false;
  };

  queue_node_t *temp = q->front;
  *data = *temp->data;

  q->front = q->front->next;

  if (q->tail == temp)
  {
    q->front = q->tail;
  }

  free(temp);
  q->len--;

  return true;
}