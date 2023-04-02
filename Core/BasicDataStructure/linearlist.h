#ifndef _HAL_LINERLIST_H_
#define _HAL_LINERLIST_H_
#ifdef __cplusplus
extern "C"
{
#endif
#include "string.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdint.h"

#define FACTOR 1.5f

  typedef int LinearType;
  typedef void (*ForEach_CallBack)(LinearType *pdata, uint32_t index, LinearType *lists);
  typedef struct linearlist linearlist_t;

  struct linearlist
  {
    LinearType *list;
    uint32_t index;
    uint32_t max_size;
    bool (*empty)(linearlist_t *list);
    uint32_t (*size)(linearlist_t *list);
    LinearType (*get)(linearlist_t *list, uint32_t i);
    bool (*erase)(linearlist_t *list, uint32_t i);
    bool (*set)(linearlist_t *list, LinearType *pdata, uint32_t i);
    void (*foreach)(linearlist_t *list, ForEach_CallBack callbackFn);
    LinearType *(*back)(linearlist_t *list);      /*返回对向量中最后一个元素的引用*/
    LinearType *(*back_next)(linearlist_t *list); /*返回对向量中最后一个元素的下一个引用*/
    void (*clear)(linearlist_t *list);            /*清空线性表*/
  };

  linearlist_t *Create_LinearList(uint32_t length);

#ifdef __cplusplus
}
#endif
#endif