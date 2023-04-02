#include "mitt.h"

void mitt_on(mitter_t *mitt, queue_tag_t tag, Mitt_CallBack callbackFn);
void mitt_off(mitter_t *mitt, queue_tag_t tag, Mitt_CallBack callbackFn);
void mitt_emit(mitter_t *mitt, queue_tag_t tag, arg_pdata_t arg);
bool mitt_find(mitter_t *mitt, mitt_node_t *reac_node, elementType tag);
void mitt_node_set(mitter_t *mitt, mitt_node_t *node);

char pdata_mitt[32];

mitt_t Mitter_Bus(uint32_t mitt_len)
{

  static mitter_t mitt = {
      .index = 0,
      .mitt_emit = mitt_emit,
      .mitt_off = mitt_off,
      .mitt_on = mitt_on,
  };

  mitt.mitt_nodes = calloc(5, sizeof(mitt_node_t));
  mitt.size_max = 5;

  return &mitt;
}

void mitt_on(mitter_t *mitt, queue_tag_t tag, Mitt_CallBack callbackFn)
{
  volatile bool flag = false;
  mitt_node_t *node;

  flag = mitt_find(mitt, node, (elementType)tag);
  flag && (node->mitter_event->set(node->mitter_event, (LinearType *)callbackFn, node->mitter_event->index));

  if (!flag)
  {
    static mitt_node_t new_node;
    new_node.index = 0;
    new_node.key = (elementType)tag;
    new_node.mitter_event = Create_LinearList(sizeof(Mitt_CallBack) * 5);
    new_node.mitter_event->set(new_node.mitter_event, (LinearType *)callbackFn, 0);

    mitt_node_set(mitt, &new_node);
  }
}

void mitt_off(mitter_t *mitt, queue_tag_t tag, Mitt_CallBack callbackFn)
{
}

void mitt_emit(mitter_t *mitt, queue_tag_t tag, arg_pdata_t arg)
{
  for (size_t i = 0; i < mitt->index; i++)
  {
    if (mitt->mitt_nodes[i].key == (elementType)tag)
    {
      for (size_t j = 0; j < mitt->mitt_nodes[i].mitter_event->index; j++)
      {
        ((void (*)(arg_pdata_t))(mitt->mitt_nodes[i].mitter_event->get(mitt->mitt_nodes[i].mitter_event, j)))(arg);
      }
    }
  }
}

bool mitt_find(mitter_t *mitt, mitt_node_t *reac_node, elementType tag)
{
  for (size_t i = 0; i < mitt->index; i++)
  {
    if (mitt->mitt_nodes[i].key == (elementType)tag)
    {
      reac_node = mitt->mitt_nodes + i;
      return true;
    }
  }

  return false;
}

void mitt_node_set(mitter_t *mitt, mitt_node_t *node)
{
  if (mitt->index + 1 >= 5)
    return;

  mitt->mitt_nodes[mitt->index] = *node;
  mitt->index += 1;
}