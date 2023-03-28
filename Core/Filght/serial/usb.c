#include "usb.h"
#include "string.h"
#include "stdio.h"

UART_HandleTypeDef uart1;
usart_methdos_t *serial;
bool isCreateSerial = false;
rx_struct_t rx_instance = {
    .max_size = Rx_Max_Size,
    .frame = 0,
};

void USART1_IRQHandler();
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);

usart_methdos_t *Hal_Struct_Uart_Init(void)
{
  if (isCreateSerial == false)
  {
    MX_USART1_UART_Init();

    static usart_buffer_t hhuart_buf = {
        .huart1 = &uart1,
        .len = 0,
        .huart_ing = false,
    };

    static usart_methdos_t uart_mets = {
        .huart_buf = &hhuart_buf,
        .Hal_ClearAll_Buf = Hal_ClearAll_Buf,
        .Hal_SendData = Hal_SendData,
        .Hal_Write_Buf = Hal_Write_Buf,
        .Hal_Insert_Data = Hal_Insert_Data,
        .Hal_Write_Float_Buffer = Hal_Write_Float_Buffer,
    };

    serial = &uart_mets;
    isCreateSerial = true;

    HAL_NVIC_EnableIRQ(USART1_IRQn);
    HAL_NVIC_SetPriority(USART1_IRQn, 1, 1);

    HAL_UART_Receive_IT(&uart1, &rx_instance.rx_store[rx_instance.frame], 1);

    return serial;
  }

  return serial;
}

huart_error_t Hal_Write_Buf(const char *data)
{
  // 优先级  Mutex  > Huart_ing
  if (HAL_UART_GetState(&uart1) != HAL_UART_STATE_RESET)
  {
    strcat((char *)serial->huart_buf->buf, data);
    serial->huart_buf->len += strlen(data);
  }

  return UART_OK;
}

void Hal_SendData(void)
{
  if (HAL_UART_GetState(&uart1) != HAL_UART_STATE_RESET &&
      serial->huart_buf->huart1->Instance == USART1 &&
      serial->huart_buf->len > 0)
  {
    if (HAL_UART_Transmit(serial->huart_buf->huart1, serial->huart_buf->buf, serial->huart_buf->len, Uart_Timeout) != HAL_OK)
    {
      uint8_t pdata = UART_SEND_ERR;
      HAL_UART_Transmit(serial->huart_buf->huart1, &pdata, 1, Uart_Timeout);
      serial->Hal_ClearAll_Buf();
    }
    else
      serial->Hal_ClearAll_Buf();
  }
}

huart_error_t Hal_Insert_Data(char *data, uint16_t pos)
{

  return UART_OK;
}

void Hal_ClearAll_Buf()
{
  memset(serial->huart_buf->buf, 0x00, serial->huart_buf->len);
  serial->huart_buf->len = 0;
}

huart_error_t Hal_Write_Float_Buffer(const char *pdata, const uint8_t len)
{
  // 优先级  Mutex  > Huart_ing
  if (HAL_UART_GetState(&uart1) != HAL_UART_STATE_RESET)
  {
    memcpy(serial->huart_buf->buf + serial->huart_buf->len, pdata, len);
    serial->huart_buf->len += len;
  }
  else
  {
    return UART_WRITE_ERR;
  }

  return UART_OK;
}

void MX_USART1_UART_Init()
{
  uart1.Instance = USART1;
  uart1.Init.BaudRate = 115200;
  uart1.Init.WordLength = UART_WORDLENGTH_8B;
  uart1.Init.StopBits = UART_STOPBITS_1;
  uart1.Init.Parity = UART_PARITY_NONE;
  uart1.Init.Mode = UART_MODE_TX_RX;
  uart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  uart1.Init.OverSampling = UART_OVERSAMPLING_16;
  uart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  uart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&uart1) != HAL_OK)
  {
    Error_Handler();
  }

  HAL_UART_MspInit(&uart1);
}

void HAL_UART_MspInit(UART_HandleTypeDef *uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if (uartHandle->Instance == USART1)
  {
    __HAL_RCC_USART1_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /*
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9 | GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef *uartHandle)
{

  if (uartHandle->Instance == USART1)
  {
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9 | GPIO_PIN_10);
    HAL_DMA_DeInit(uartHandle->hdmatx);
  }
}

void USART1_IRQHandler()
{
  HAL_UART_IRQHandler(&uart1);

  Hal_Write_Buf((char *)&rx_instance.rx_store[+rx_instance.frame - 1 < 0 ? 0 : rx_instance.frame - 1]);
  Hal_SendData();
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART1)
  {
    uint8_t data = rx_instance.rx_store[rx_instance.frame];
    rx_instance.frame += 1;

    if (rx_instance.frame == 1 && data == DFU_FLAG)
    {
      Hal_Write_Buf("dfu ok\r\n");
      Hal_SendData();

      Enter_Dfu();
    }

    Rx_Clear_Lists(&rx_instance, data);
    UART_Start_Receive_IT(huart, &rx_instance.rx_store[rx_instance.frame], 1);
  }
}
