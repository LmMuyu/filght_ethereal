#ifndef _HAL_SBUS_H_
#define _HAL_SBUS_H_
#ifdef __cplusplus
extern "C"
{
#endif
#include "main.h"
#include "stm32f3xx_hal_uart.h"
#include "mitter.h"
#include "usb.h"

#define SBUS_PROTOCOL_SIZE 25
#define BAUD_RATE 100000
#define UART3_REG GPIOB
#define UART3_RX GPIO_PIN_11

  typedef union SBUS sbus_protocol_frame_t;
  typedef void (*Frame_CallbackFn)(sbus_protocol_frame_t *frame);

  typedef struct HUART3
  {
    UART_HandleTypeDef *huart;
    uint8_t *temp;
  } huart3_t;

  union SBUS
  {
    bool set_clear;
    uint8_t start_frame;
    uint8_t data_frame[SBUS_PROTOCOL_SIZE - 3];
    uint8_t status_frame;
    uint8_t stop_frame;
    uint8_t channel_frame;
    uint8_t protocol_frame[SBUS_PROTOCOL_SIZE];
  };

  extern huart3_t *huart3;
  extern sbus_protocol_frame_t *arg_sbus_frame;

  void Sbus_Init();
  void Sbus_Pdata_Rx_Start(sbus_protocol_frame_t *sbus_frame, Frame_CallbackFn callbackFn);

#ifdef __cplusplus
}
#endif
#endif