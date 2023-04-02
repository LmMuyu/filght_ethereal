#include "bn-880.h"

void Gps_Uart_Init();
void Enable_Uart_Pin();
void USART2_IRQHandler();
void Bn880_RxUart_CallBack(arg_pdata_t arg);

StaticTask_t xTaskBuffer;
StackType_t xStack[STACK_SIZE];
TaskHandle_t xQmc_Handle = NULL;
bn_uart_pdata_rx_t bn880_pdata_rx;

void vTaskCode(void *pvParameters)
{
  Gps_Uart_Init();

  Mitt_Get()->mitt_on(Mitt_Get(), UART_2_TAG, Bn880_RxUart_CallBack);

  while (1)
  {
    HAL_UART_Transmit(bn880_pdata_rx.huart, (uint8_t *)"vTaskcode", 10, 1000);

    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void Gps_Uart_Init()
{
  Enable_Uart_Pin();

  UART_InitTypeDef huart2 = {
      .BaudRate = 9600,
      .Mode = UART_MODE_TX_RX,
      .Parity = UART_PARITY_NONE,
      .StopBits = UART_STOPBITS_1,
      .HwFlowCtl = UART_HWCONTROL_NONE,
      .WordLength = UART_WORDLENGTH_8B,
      .OverSampling = UART_OVERSAMPLING_16,
      .OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE,
  };

  static UART_HandleTypeDef huart_handler = {
      .Instance = USART2,
      .AdvancedInit = {
          .AdvFeatureInit = UART_ADVFEATURE_NO_INIT,
      },
  };

  huart_handler.Init = huart2;
  bn880_pdata_rx.huart = &huart_handler;

  HAL_NVIC_EnableIRQ(USART2_IRQn);
  HAL_NVIC_SetPriority(USART2_IRQn, 2, 2);

  if (HAL_UART_Init(&huart_handler) != HAL_OK)
  {
    Error_Handler();
  }

  HAL_UART_Receive_IT(bn880_pdata_rx.huart, &(bn880_pdata_rx.rx_pdata[bn880_pdata_rx.index]), 1);
}

void Enable_Uart_Pin()
{
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_USART2_CLK_ENABLE();

  GPIO_InitTypeDef hgpio = {
      .Pin = GPS_RX | GPS_TX,
      .Pull = GPIO_NOPULL,
      .Mode = GPIO_MODE_AF_PP,
      .Speed = GPIO_SPEED_FREQ_HIGH,
      .Alternate = GPIO_AF7_USART2,
  };

  GPIO_InitTypeDef swd;
  swd.Pin = GPIO_PIN_13 | GPIO_PIN_14;
  swd.Mode = GPIO_MODE_OUTPUT_PP;
  swd.Pull = GPIO_NOPULL;
  swd.Speed = GPIO_SPEED_FREQ_HIGH;

  HAL_GPIO_Init(GPIOA, &swd);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_13 | GPIO_PIN_14, GPIO_PIN_RESET);

  HAL_GPIO_Init(GPS_GPIO_REG, &hgpio);
}

void Bn880_handle()
{
  xQmc_Handle = xTaskCreateStatic(
      vTaskCode,             /* Function that implements the task. */
      "bn880",               /* Text name for the task. */
      STACK_SIZE,            /* Number of indexes in the xStack array. */
      NULL,                  /* Parameter passed into the task. */
      Task_Priority_Level_8, /* Priority at which the task is created. */
      xStack,                /* Array to use as the task's stack. */
      &xTaskBuffer);         /* Variable to hold the task's data structure. */
}

void USART2_IRQHandler()
{
  HAL_UART_IRQHandler(bn880_pdata_rx.huart);
}

char pdata_bn[32];

void Bn880_RxUart_CallBack(arg_pdata_t arg)
{
  // instance_rx.index += 1;
  HAL_UART_Receive_IT(bn880_pdata_rx.huart, &(bn880_pdata_rx.rx_pdata[bn880_pdata_rx.index]), 1);

  sprintf(pdata_bn, " gps_info:%s\n", (char *)bn880_pdata_rx.rx_pdata);
  Hal_Write_Buf(pdata_bn);
  Hal_SendData();
}