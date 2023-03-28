#ifndef _HAL_MITT_H_
#define _HAL_MITT_H_
#ifdef __cplusplus
extern "C"
{
#endif

#include "main.h"
#include "freertos_mes_queue.h"
#include "stdlib.h"
#include "usb.h"
#include "string.h"

#define HEIGHT(node) ((node == NULL) ? 0 : (((avlnode *)(node))->height))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

  typedef void (*mitter_backfn)(queue_node_t *);

  typedef int elementType;

  typedef struct mitt_node
  {
    elementType key;
    mitter_backfn *mitter_event;
    uint32_t index;
  } mitt_node_t;

  typedef struct Mitter
  {
    mitt_node_t mitt[20];
    uint32_t index;
    void (*mitt_on)(queue_tag_t tag, mitter_backfn mitter_bcfn);
    void (*mitt_off)(queue_tag_t tag, mitter_backfn mitter_bcfn);
    void (*mitt_emit)(queue_tag_t tag, queue_node_t *node);
  } mitter_t, *mitt_t;

  extern mitter_t *mitt;

  void mitt_on(queue_tag_t tag, mitter_backfn mitter_bcfn);
  void mitt_off(queue_tag_t tag, mitter_backfn mitter_bcfn);
  void mitt_emit(queue_tag_t tag, queue_node_t *node);

  mitt_t Mitter_Bus();

#ifdef __cplusplus
}
#endif
#endif