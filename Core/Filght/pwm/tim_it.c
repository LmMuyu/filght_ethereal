#include "tim_it.h"

void TIM17_IRQHandler(void)
{
  Hal_Write_Buf("TIM17_IRQHandler \r\n");
  Hal_SendData();
}

void Tim17_IT_Function(TIM_HandleTypeDef *htimx)
{
  HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);
}