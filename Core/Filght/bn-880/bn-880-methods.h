#ifndef _HAL_BN880_METHODS_H_
#define _HAL_BN880_METHODS_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "stdbool.h"
#include "linearlist.h"
#include "string.h"
#include "stdio.h"
#include "usb.h"
#include "stdbool.h"

#define RX_PDAT_SIZE 128

  typedef struct BN880_Pdata *gps_struct_ptr;

  typedef struct BN880_Pdata
  {
    volatile bool start_symbol;
    uint8_t gps_pdata[RX_PDAT_SIZE];
    uint32_t index;
    bool (*Gs_Parser)(gps_struct_ptr gps_ptr, uint8_t str);
    void (*clear)(gps_struct_ptr gps_rx_pdata);
  } gps_datastruct_t;

  gps_datastruct_t *Create_Gps_Parser_Struct();
#ifdef __cplusplus
}
#endif
#endif