#include "mitt.h"

mitter_t *mitt;

mitt_t Mitter_Bus()
{
  if (mitt == NULL)
  {
    Hal_Write_Buf("MITTER \n");
    Hal_SendData();

    mitt = calloc(1, sizeof(mitter_t));
    mitt->index = 0;
    mitt->mitt_emit = mitt_emit;
    mitt->mitt_off = mitt_off;
    mitt->mitt_on = mitt_on;
  }

  return mitt;
}

void mitt_on(queue_tag_t tag, mitter_backfn mitter_bcfn)
{
  mitt_node_t *node;

  for (size_t i = 0; i < mitt->index; i++)
  {
    if ((mitt->mitt + i)->key == (elementType)tag)
    {
      node = mitt->mitt + i;
    }

    *(node->mitter_event + node->index) = mitter_bcfn;
    node->index += 1;
  }

  if (node == NULL)
  {
    static mitt_node_t new_node;
    new_node.index = 0;
    new_node.key = (elementType)tag;
    new_node.mitter_event = calloc(20, sizeof(mitter_backfn));

    *(mitt->mitt + mitt->index) = new_node;
    mitt->index += 1;
  }
}

void mitt_off(queue_tag_t tag, mitter_backfn mitter_bcfn)
{
}

void mitt_emit(queue_tag_t tag, queue_node_t *node)
{
  for (size_t i = 0; i < mitt->index; i++)
  {
    if ((elementType)tag == (elementType)mitt->mitt[i].key)
    {
      for (size_t j = 0; j < mitt->mitt[j].index; j++)
      {
        mitt->mitt[j].mitter_event[j](node);
      }
    }
  }
}