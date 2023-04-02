#include "linearlist.h"

bool List_Adjust(linearlist_t *list, float C);
bool empty(linearlist_t *list);
uint32_t size(linearlist_t *list);
LinearType get(linearlist_t *list, uint32_t i);
bool erase(linearlist_t *list, uint32_t i);
bool set(linearlist_t *list, LinearType *pdata, uint32_t i);
void foreach (linearlist_t *list, ForEach_CallBack callbackFn);
LinearType *back(linearlist_t *list); /*返回对向量中最后一个元素的引用*/
void clear(linearlist_t *list);
LinearType *back_next(linearlist_t *list);

linearlist_t *Create_LinearList(uint32_t length)
{
  LinearType *list = (LinearType *)malloc(sizeof(LinearType) * length);

  static linearlist_t linearlist = {
      .empty = empty,
      .erase = erase,
      .foreach = foreach,
      .get = get,
      .index = 0,
      .set = set,
      .size = size,
      .back = back,
      .clear = clear,
      .back_next = back_next,
  };

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

  list->list[i] = (LinearType)pdata;
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

bool List_Adjust(linearlist_t *list, float C)
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

LinearType *back(linearlist_t *list)
{
  return list->list + (list->index == 0 ? 0 : list->index - 1);
}

LinearType *back_next(linearlist_t *list)
{
  LinearType *linear_count_ptr = back(list);

  if (list->index == list->max_size)
  {
    return linear_count_ptr;
  }

  return linear_count_ptr + 1;
}

void clear(linearlist_t *list)
{
  memset(list->list, 0x00, list->index);
  list->index = 0;
}