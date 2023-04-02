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
#include "mitter.h"
#include "FreeRTOSConfig.h"
#include "freertos_task.h"
#include "freertos_mes_queue.h"
#include "stm32f3xx_hal_uart.h"

#define STACK_SIZE configMINIMAL_STACK_SIZE

#define GPS_GPIO_REG GPIOA
#define GPS_TX GPIO_PIN_14
#define GPS_RX GPIO_PIN_15
#define Rx_Size 64

#define _HAL_UART_RX_FLAG_ 1

  typedef struct BN_UART_Rx
  {
    UART_HandleTypeDef *huart;
    uint32_t index;
    uint8_t rx_pdata[Rx_Size];
  } bn_uart_pdata_rx_t;

  extern StaticTask_t xTaskBuffer;
  extern StackType_t xStack[STACK_SIZE];
  extern TaskHandle_t xQmc_Handle;
  extern bn_uart_pdata_rx_t bn880_pdata_rx;

  void Bn880_handle();

#ifdef __cplusplus
}
#endif
#endif