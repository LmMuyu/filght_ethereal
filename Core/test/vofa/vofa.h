#ifndef _HAL_VOFA_
#define _HAL_VOFA_
#ifdef __cplusplus
extern "C"
{
#endif
#include "main.h"
#include "usb.h"
#include "string.h"
#include "stdio.h"

#define VOFA_CH_SIZE 32

  typedef union Vofa_Just_Float_Data
  {
    float fv;
    char pdata[4];
  } vofa_just_float_t;

  struct Vofa
  {
    char ch_vofa[16];
    char tail[4];
  };

  extern struct Vofa vofa;

  void Vofa_Just_Float(float ch[3]);
  void Vofa_Fire_Water(char *pdata);
  void Vofa_Protocol_Group(char *protocol_group, float f_v, uint8_t index);

#ifdef __cplusplus
}
#endif
#endif