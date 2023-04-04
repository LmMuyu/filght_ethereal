#ifndef _HAL_BN880_H_
#define _HAL_BN880_H_
#ifdef __cplusplus
extern "C"
{
#endif

#include "main.h"
#include "task.h"
#include "usb.h"
#include "stdio.h"
#include "mitter.h"
#include "FreeRTOS.h"
#include "string.h"
#include "mitter.h"
#include "freertos_task.h"
#include "FreeRTOSConfig.h"
#include "bn-880-methods.h"
#include "freertos_mes_queue.h"
#include "stm32f3xx_hal_uart.h"
#include "stdbool.h"

#define STACK_SIZE 128

#define GPS_GPIO_REG GPIOA
#define GPS_TX GPIO_PIN_14
#define GPS_RX GPIO_PIN_15

  typedef struct BN_UART_Rx
  {
    UART_HandleTypeDef *huart;
    uint8_t *tmp;
  } bn_uart_pdata_rx_t;

  extern bn_uart_pdata_rx_t bn880_pdata_rx;
  extern gps_struct_ptr gps_pdata;

  void Bn880_handle();

#ifdef __cplusplus
}
#endif
#endif