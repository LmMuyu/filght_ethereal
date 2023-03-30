#ifndef _HAL_MITT_H_
#define _HAL_MITT_H_
#ifdef __cplusplus
extern "C"
{
#endif

#include "main.h"
#include "freertos_mes_queue.h"
#include "stdlib.h"
#include "string.h"
#include "linearlist.h"
#include "stdbool.h"
#include "usb.h"

#define HEIGHT(node) ((node == NULL) ? 0 : (((avlnode *)(node))->height))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

  typedef struct Mitter *mitt_t;
  typedef int elementType;
  typedef void call_back_pdata;
  typedef union Arg_Pdata arg_pdata_t;
  typedef void (*Mitt_CallBack)(arg_pdata_t arg);

  typedef struct mitt_node
  {
    elementType key;
    linearlist_t *mitter_event;
    uint32_t index;
  } mitt_node_t;

  union Arg_Pdata
  {
    elementType key;
    elementType *pdata;
    char max_pdata[33];
  };

  typedef struct Mitter
  {
    mitt_node_t *mitt;
    uint32_t index;
    void (*mitt_on)(mitt_t mitt, queue_tag_t tag, Mitt_CallBack callbackFn);
    void (*mitt_off)(mitt_t mitt, queue_tag_t tag, Mitt_CallBack callbackFn);
    void (*mitt_emit)(mitt_t mitt, queue_tag_t tag, arg_pdata_t arg);
  } mitter_t;

  mitt_t Mitter_Bus(uint32_t mitt_len);

#ifdef __cplusplus
}
#endif
#endif