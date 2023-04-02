#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdint.h"

#define FACTOR 1.5f

typedef int LinearType;
typedef void (*ForEach_CallBack)(LinearType *pdata, uint32_t index, LinearType *lists);
typedef struct linearlist linearlist_t;
typedef void (*func_t)();

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
};

typedef struct BASE_STRUCT
{
  float max;
  float pos_x;
  float pos_y;
} base_t;

linearlist_t *Create_LinearList(uint32_t length);

bool List_Adjust(linearlist_t *list, uint32_t C);
bool empty(linearlist_t *list);
uint32_t size(linearlist_t *list);
LinearType get(linearlist_t *list, uint32_t i);
bool erase(linearlist_t *list, uint32_t i);
bool set(linearlist_t *list, LinearType *pdata, uint32_t i);
void foreach (linearlist_t *list, ForEach_CallBack callbackFn);

linearlist_t *Create_LinearList(uint32_t length)
{
  LinearType *list = (LinearType *)malloc(sizeof(LinearType) * length);

  static linearlist_t linearlist = {0};

  linearlist.empty = empty,
  linearlist.erase = erase,
  linearlist.foreach = foreach,
  linearlist.get = get,
  linearlist.index = 0,
  linearlist.set = set,
  linearlist.size = size,
  linearlist.list = list;
  linearlist.max_size = length;

  return &linearlist;
}

bool empty(linearlist_t *list)
{
  return list->index <= 0 ? true : false;
}

uint32_t size(linearlist_t *list)
{
  return list->index;
}

LinearType get(linearlist_t *list, uint32_t i)
{
  if (i >= list->max_size || i < 0)
  {
    return (LinearType)-1;
  }

  return list->list[i];
}

bool erase(linearlist_t *list, uint32_t i)
{
  if (i >= list->max_size || i < 0)
  {
    return false;
  }

  strncpy((char *)list->list + i, (char *)list->list + i + 1, list->index - i);
  *(list->list + list->index - 1) = 0x00;

  list->index -= 1;

  return true;
}

bool set(linearlist_t *list, LinearType *pdata, uint32_t i)
{
  if (i >= list->max_size || i < 0)
  {
    return false;
  }

  if (list->index == list->max_size)
  {
    if (List_Adjust(list, FACTOR) == false)
    {
      return false;
    }
  }

  list->list[i] = (LinearType)(pdata);
  list->index += 1;

  return true;
}

void foreach (linearlist_t *list, ForEach_CallBack callbackFn)
{
  for (size_t i = 0; i < list->index; i++)
  {
    callbackFn(list->list + i, i, list->list);
  }
}

bool List_Adjust(linearlist_t *list, uint32_t C)
{
  const int newLength = (int)((float)list->max_size) * C;

  LinearType *newlist = (LinearType *)malloc(sizeof(LinearType) * newLength);

  if (newlist == NULL)
  {
    return false;
  }

  strcpy((char *)newlist, (char *)list->list);

  free(list->list);

  list->list = newlist;
  list->max_size = newLength;

  return true;
}

void func()
{
  printf("test\n");
}

int main(int argc, char const *argv[])
{
  linearlist_t *list = Create_LinearList(sizeof(int) * 5);

  list->set(list, (LinearType *)"abdw", 0);

  printf("%s\n", (char *)list->get(list, 0));

  return 0;
}
