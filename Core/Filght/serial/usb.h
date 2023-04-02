#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "main.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "usb_mode.h"
#include "common.h"
#include "stdbool.h"
#include "mitt.h"
#include "mitter.h"

#define MAX_BUFFER_SIZE 1024

#define Uart_Timeout 100

  typedef enum Huart_Error
  {
    UART_OK = 0x00,
    UART_WRITE_ERR = 0x01,
    UART_INSERT_ERR = 0x02,
    UART_SEND_ERR = 0x03,
    UART_BOUNDARY_ERR = 0x04, /**插入数据超出边界*/
  } huart_error_t;

  typedef struct Usart_Methdos usart_methdos_t;

  typedef struct Usart_Buffer
  {
    UART_HandleTypeDef *huart1;
    uint8_t buf[256];
    uint16_t len;
    bool huart_ing;
  } usart_buffer_t;

  struct Usart_Methdos
  {
    usart_buffer_t *huart_buf;
    huart_error_t (*Hal_Write_Buf)(const char *data);
    void (*Hal_SendData)(void);
    huart_error_t (*Hal_Write_Float_Buffer)(const char *pdata, const uint8_t len);
    huart_error_t (*Hal_Insert_Data)(char *data, uint16_t pos);
    void (*Hal_ClearAll_Buf)(void);
  };

  extern UART_HandleTypeDef uart1;
  extern usart_methdos_t *serial;
  extern bool isCreateSerial;
  extern rx_struct_t rx_instance;

  void MX_USART1_UART_Init(void);

  usart_methdos_t *Hal_Struct_Uart_Init(void);

  huart_error_t Hal_Write_Buf(const char *data);

  void Hal_SendData(void);

  huart_error_t Hal_Insert_Data(char *data, uint16_t pos);

  void Hal_ClearAll_Buf(void);

  huart_error_t Hal_Write_Float_Buffer(const char *pdata, const uint8_t len);

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */
