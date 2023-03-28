#include "pwm.h"

uint16_t Heavy_Num(uint32_t hz, uint16_t psc);
uint16_t Init_Computed_Pulse(uint16_t n, float init_pluse);
void Pwm_Port_Init(GPIO_TypeDef *htimx_reg, uint32_t pin, TIM_TypeDef *htimx, unsigned int htimx_ch);
void Tim_OcConfig(TIM_HandleTypeDef *htim, uint16_t pulse, htimx_handle_t *htimx_h);
uint32_t Timx_Alt_And_Clk(TIM_TypeDef *timx, uint32_t htimx_ch);

char pwm_pdata[64];

timx_handle_mes_t timx_handle_mes = {
    .Pwm_Timx_Create = Pwm_Timx_Create,
    .Pwm_Pulse_Change = Pwm_Pulse_Change,
    .Pwm_Start = Pwm_Tim_Start,
};

htimx_handle_t *Pwm_Timx_Create(uint32_t hz, uint16_t psc, htimx_port_init_t *timx_port)
{
  Pwm_Port_Init(timx_port->htimx_reg, timx_port->htimx_pin, timx_port->htimx, timx_port->htimx_ch); // 定时器端口初始化
  uint16_t timer_arr = Heavy_Num(hz, psc);

  if (timer_arr == MAX_OVER_NUM)
  {
    Error_Handler();
  }

  TIM_HandleTypeDef Timx;
  Timx.Instance = timx_port->htimx;
  Timx.Init.Period = timer_arr - 1;
  Timx.Init.Prescaler = psc - 1;
  Timx.Init.CounterMode = TIM_COUNTERMODE_UP;
  Timx.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  Timx.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;

  if (HAL_TIM_Base_Init(&Timx) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_TIM_OnePulse_Init(&Timx, TIM_OPMODE_REPETITIVE) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_TIM_PWM_Init(&Timx) != HAL_OK)
  {
    Error_Handler();
  }

  static htimx_handle_t htimx_handle;
  htimx_handle.htimx = &Timx;
  htimx_handle.k_pulse = timx_port->k_init_pluse;
  htimx_handle.freq_arr = timer_arr;
  htimx_handle.htimx_ch = timx_port->htimx_ch;

  uint16_t m_pluse = Init_Computed_Pulse(timer_arr, timx_port->k_init_pluse);

  Tim_OcConfig(&Timx, m_pluse, &htimx_handle);
  Pwm_Tim_Start(&htimx_handle);

  return &htimx_handle;
}

int motor_compare[4];
int motor_id = 0;

uint8_t Pwm_Pulse_Change(htimx_handle_t *TIMx, float k_pluse)
{
  uint16_t m_pluse = Init_Computed_Pulse(TIMx->freq_arr, k_pluse) - 1;
  __HAL_TIM_SetCompare(TIMx->htimx, TIMx->htimx_ch, m_pluse);
  TIMx->k_pulse = k_pluse;

  motor_compare[motor_id] = (__HAL_TIM_GetCompare(TIMx->htimx, TIMx->htimx_ch));
  motor_id += 1;

  // sprintf(pwm_pdata, "Tim_Compare:%d\n", __HAL_TIM_GetCompare(TIMx->htimx, TIMx->htimx_ch));
  // Hal_Write_Buf(pwm_pdata);
  // Hal_SendData();
  if (motor_id == 4)
  {
    sprintf(pwm_pdata, "%d,%d,%d,%d\n", motor_compare[0], motor_compare[1], motor_compare[2], motor_compare[3]);
    Vofa_Fire_Water(pwm_pdata);
    motor_id = 0;
  }

  return 0;
}

void Pwm_Tim_Start(htimx_handle_t *TIMx)
{
  HAL_TIM_PWM_Start(TIMx->htimx, TIMx->htimx_ch);
}

uint16_t Heavy_Num(uint32_t hz, uint16_t psc)
{
  uint16_t timer_arr = (uint16_t)((uint32_t)APB_Time_TICK / (hz * (uint32_t)psc));

  if (timer_arr > (uint16_t)MAX_OVER_NUM)
  {
    return (uint16_t)MAX_OVER_NUM;
  }

  return timer_arr;
}

uint16_t Init_Computed_Pulse(uint16_t n, float k_init_pluse)
{
  if (k_init_pluse > 1.0f)
  {
    k_init_pluse = 1 / k_init_pluse;
  }

  uint16_t pluse = n * k_init_pluse;
  return pluse;
}

void Pwm_Port_Init(GPIO_TypeDef *htimx_reg, uint32_t pin, TIM_TypeDef *htimx, unsigned int htimx_ch)
{

  uint32_t Timx_Alt = Timx_Alt_And_Clk(htimx, htimx_ch);

  if (Timx_Alt == 0)
  {
    Hal_Write_Buf("AF GPIO Error\n");
    Hal_SendData();
    Error_Handler();
  }

  GPIO_InitTypeDef Pwm_Port = {
      .Pin = pin,
      .Pull = GPIO_NOPULL,
      .Mode = GPIO_MODE_AF_PP,
      .Speed = GPIO_SPEED_FREQ_HIGH,
      .Alternate = Timx_Alt,
  };

  HAL_GPIO_Init(htimx_reg, &Pwm_Port);
}

void Tim_OcConfig(TIM_HandleTypeDef *htim, uint16_t pulse, htimx_handle_t *htimx_h)
{
  TIM_OC_InitTypeDef Timx_Oc = {
      .Pulse = pulse - 1, // 占空比
      .OCFastMode = TIM_OCFAST_DISABLE,
      .OCMode = TIM_OCMODE_PWM1,
      .OCPolarity = TIM_OCPOLARITY_HIGH, // pwm前半段为高电平
                                         // .OCNPolarity = TIM_OCPOLARITY_HIGH, // pwm后半段为低电平
  };

  HAL_TIM_PWM_ConfigChannel(htim, &Timx_Oc, htimx_h->htimx_ch);
}

uint32_t Timx_Alt_And_Clk(TIM_TypeDef *timx, uint32_t htimx_ch)
{
  /*
  PWM输出排针
  PWM2——PA7——TIM17_CH1
  PWM3——PA11——TIM4_CH1
  PWM4——PA12——TIM4_CH2
  PWM7——PA2——TIM15_CH1
  */

  uint32_t GPIO_AF = NULL;

  if (timx == TIM17)
  {
    GPIO_AF = GPIO_AF1_TIM17;
  }
  else if (timx == TIM15 && htimx_ch == TIM_CHANNEL_1)
  {
    GPIO_AF = GPIO_AF9_TIM15;
  }
  else if (timx == TIM4 && htimx_ch == TIM_CHANNEL_1)
  {
    GPIO_AF = GPIO_AF10_TIM4;
  }
  else if (timx == TIM4 && htimx_ch == TIM_CHANNEL_2)
  {
    GPIO_AF = GPIO_AF10_TIM4;
  }

  if (timx == TIM17)
  {
    __HAL_RCC_TIM17_CLK_ENABLE();
  }
  else if (timx == TIM15)
  {
    __HAL_RCC_TIM15_CLK_ENABLE();
  }
  else if (timx == TIM4)
  {
    __HAL_RCC_TIM4_CLK_ENABLE();
  }

  if (GPIO_AF != NULL)
  {
    return GPIO_AF;
  }

  return 0;
}

timx_handle_mes_t *Htimx_Get_Methods()
{
  return &timx_handle_mes;
}