#ifndef _HHAL_MITTER_H_
#define _HHAL_MITTER_H_

#ifdef __cplusplus
extern "C"
{
#endif
#include "main.h"

#ifndef _MITT_
#define _MITT_

#include "mitt.h"

#endif

  extern mitt_t Mitter_mitt;

  void Mitt_Init();
  mitt_t Mitt_Get();

#ifdef __cplusplus
}
#endif
#endif