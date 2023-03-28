#ifndef _HAl_USB_MODE_
#define _HAl_USB_MODE_
#ifdef __cplusplus
extern "C"
{
#endif
#include "main.h"
#include "string.h"
#include "usb.h"
#include "common.h"

#define DFU_FLAG 0xff
#define Rx_STORE_CLEAR 0xfe

  void Enter_Dfu();
  void Rx_Clear_Lists(rx_struct_t *stroe_l, uint8_t flag);

#ifdef __cplusplus
}
#endif
#endif