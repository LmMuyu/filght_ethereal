#include "mitt.h"

void mitt_on(mitter_t *mitt, queue_tag_t tag, Mitt_CallBack callbackFn);
void mitt_off(mitter_t *mitt, queue_tag_t tag, Mitt_CallBack callbackFn);
void mitt_emit(mitter_t *mitt, queue_tag_t tag, arg_pdata_t arg);

// void OutputCallBackFn(LinearType *pdata, uint32_t index, LinearType *lists);

mitt_t Mitter_Bus(uint32_t mitt_len)
{

  static mitter_t mitt = {
      .index = 0,
      .mitt_emit = mitt_emit,
      .mitt_off = mitt_off,
      .mitt_on = mitt_on,
  };

  mitt.mitt = calloc(mitt_len, sizeof(mitter_t) * mitt_len);

  return &mitt;
}

void mitt_on(mitter_t *mitt, queue_tag_t tag, Mitt_CallBack callbackFn)
{
  bool flag = false;
  mitt_node_t *node;

  for (size_t i = 0; i < mitt->index; i++)
  {
    if (mitt->mitt[i].key == (elementType)tag)
    {
      flag = true;

      node = mitt->mitt + i;
      node->mitter_event->set(node->mitter_event, (LinearType *)callbackFn, node->mitter_event->index);
    }
  }

  if (flag)
  {
    static mitt_node_t new_node;
    new_node.index = 0;
    new_node.key = (elementType)tag;
    new_node.mitter_event = Create_LinearList(5);
    new_node.mitter_event->set(new_node.mitter_event, (LinearType *)callbackFn, 0);
    mitt->index += 1;
  }
}

void mitt_off(mitter_t *mitt, queue_tag_t tag, Mitt_CallBack callbackFn)
{
}

void mitt_emit(mitter_t *mitt, queue_tag_t tag, arg_pdata_t arg)
{
  for (size_t i = 0; i < mitt->index; i++)
  {
    if (mitt->mitt[i].key == (elementType)tag)
    {
      for (size_t j = 0; j < mitt->mitt->mitter_event->index; j++)
      {
        ((void (*)(arg_pdata_t arg))(mitt->mitt->mitter_event->get(mitt->mitt->mitter_event, j)))(arg);
      }
    }
  }
}
