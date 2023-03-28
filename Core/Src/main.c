#include "main.h"
#include "gpio.h"
#include "system.h"
#include "mpu6050.h"
#include "usb.h"
#include "task.h"
#include "pwm.h"
#include "motor_control.h"
#include "bmp280_task.h"
#include "filght_mode.h"
#include "mitter.h"
#include "gps.h"

int main(void)
{
  HAL_Init();
  SystemClock_Config();

  MX_GPIO_Init();
  Hal_Struct_Uart_Init();

  HAL_Delay(7000);

  // Motor_Init();
  // Mpu6050_Task_Handle();
  // Bmp280_Init_Handle_Task();
  qmc5883l_handle();
  // Queue_Receive_Handle();

  // filght_mode_t *filght_mode = State_Machine();

  vTaskStartScheduler();
  osKernelStart();
  /* USER CODE END 3 */
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);
    HAL_Delay(1000);
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}

#endif /* USE_FULL_ASSERT */
