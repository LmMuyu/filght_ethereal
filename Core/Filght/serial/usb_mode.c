#include "usb_mode.h"

void Enter_Dfu()
{
  __disable_irq();

  SysTick->CTRL = 0;
  SysTick->LOAD = 0;
  SysTick->VAL = 0;

  HAL_RCC_DeInit();

  for (size_t i = 0; i < 8; i++)
  {
    NVIC->ICER[i] = 0xffff;
    NVIC->ICPR[i] = 0xffff;
  }

  __enable_irq();
}

void Rx_Clear_Lists(rx_struct_t *stroe_l, uint8_t flag)
{
  if (stroe_l->frame > 0 && flag == Rx_STORE_CLEAR)
  {
    Hal_Write_Buf("clear lists\r\n");
    Hal_SendData();

    memset(stroe_l->rx_store, 0x00, stroe_l->frame);
    stroe_l->frame = 0;
  }
}