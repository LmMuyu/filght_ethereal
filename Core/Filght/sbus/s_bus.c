#include "s_bus.h"
#include "stm32f3xx_hal_uart_ex.h"

huart3_t *huart3;
sbus_protocol_frame_t *sbus;
Frame_CallbackFn Sbus_Rx_CallbackFn;
char pdata_uart3[24];

void UART_Init();
void Uart_Enable_Pin();
void Sbus_Parser_Callback(arg_pdata_t arg);

void Sbus_Init()
{
  UART_Init();

  if (HAL_UART_GetState(huart3->huart) == HAL_UART_STATE_READY)
  {
    Mitt_Get()->mitt_on(Mitt_Get(), USARTE_3_TAG, Sbus_Parser_Callback);
    huart3->temp = (uint8_t *)malloc(sizeof(uint8_t) * SBUS_PROTOCOL_SIZE);
    memset(huart3->temp, 0x00, SBUS_PROTOCOL_SIZE);
  }
}

void Sbus_Pdata_Rx_Start(sbus_protocol_frame_t *sbus_frame, Frame_CallbackFn callbackFn)
{
  sbus = sbus_frame;
  Sbus_Rx_CallbackFn = callbackFn;

  HAL_NVIC_EnableIRQ(USART3_IRQn);
  HAL_NVIC_SetPriority(USART3_IRQn, 2, 2);

  HAL_UARTEx_ReceiveToIdle_IT(huart3->huart, huart3->temp, SBUS_PROTOCOL_SIZE);
}

void UART_Init()
{
  Uart_Enable_Pin();

  static UART_HandleTypeDef huart = {0};
  UART_AdvFeatureInitTypeDef huart_adv = {0};
  huart_adv.AdvFeatureInit = UART_ADVFEATURE_RXINVERT_INIT;
  huart_adv.RxPinLevelInvert = UART_ADVFEATURE_RXINV_ENABLE;

  huart.Instance = USART3;
  huart.Init.BaudRate = BAUD_RATE;
  huart.Init.Mode = UART_MODE_RX;
  huart.Init.StopBits = UART_STOPBITS_2;
  huart.Init.WordLength = UART_WORDLENGTH_9B;
  huart.Init.Parity = UART_PARITY_EVEN;
  huart.Init.OverSampling = UART_OVERSAMPLING_8;
  huart.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart.AdvancedInit = huart_adv;

  if (HAL_UART_Init(&huart) != HAL_OK)
  {
    Error_Handler();
  }

  static huart3_t h3 = {0};
  h3.huart = &huart;
  huart3 = &h3;
}

void Uart_Enable_Pin()
{
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_USART3_CLK_ENABLE();

  GPIO_InitTypeDef hgpio = {
      .Pin = UART3_RX,
      .Pull = GPIO_NOPULL,
      .Mode = GPIO_MODE_AF_PP,
      .Speed = GPIO_SPEED_FREQ_HIGH,
      .Alternate = GPIO_AF7_USART3,
  };

  HAL_GPIO_Init(UART3_REG, &hgpio);
}

void Sbus_Parser_Callback(arg_pdata_t arg)
{
  if (huart3->temp[0] == 0x0f && huart3->temp[24] == 0x00)
  {
    strncpy((char *)sbus->protocol_frame, (char *)huart3->temp, SBUS_PROTOCOL_SIZE);
    memset(huart3->temp, 0x00, SBUS_PROTOCOL_SIZE);
    Sbus_Rx_CallbackFn(sbus);
  }
  else
  {
    memset(huart3->temp, 0x00, SBUS_PROTOCOL_SIZE);
  }

  HAL_UARTEx_ReceiveToIdle_IT(huart3->huart, huart3->temp, SBUS_PROTOCOL_SIZE);
}

void USART3_IRQHandler()
{
  HAL_UART_IRQHandler(huart3->huart);
}
