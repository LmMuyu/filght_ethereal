#ifndef _HAL_PWM_
#define _HAL_PWM_
#ifdef __cplusplus
extern "C"
{
#endif

#include "main.h"
#include "stm32f3xx_hal_tim.h"
#include "main.h"
#include "usb.h"
#include "stdio.h"
#include "tim_it.h"
#include "stdlib.h"
#include "vofa.h"

#define MAX_PWM 8
#define MAX_OVER_NUM 65535
#define APB_Time_TICK (HAL_RCC_GetPCLK1Freq() * 2)

/*定义PWM分辨率*/
#define PWM_RESOLUTION 1000

#define INIT_PULSE 0.1 // ms
#define MIN_PULSE 0.1  // ms
#define MAX_PULSE 0.2  // ms

  typedef struct Timx_Handle_Mes timx_handle_mes_t;

  extern timx_handle_mes_t timx_handle_mes;

  typedef struct Timx_Port
  {
    GPIO_TypeDef *htimx_reg;
    uint32_t htimx_pin;
    TIM_TypeDef *htimx;
    const float k_init_pluse;
    unsigned int htimx_ch;
  } htimx_port_init_t;

  typedef struct HTIMx
  {
    TIM_HandleTypeDef *htimx;
    float k_pulse;
    uint16_t freq_arr;
    unsigned int htimx_ch;
  } htimx_handle_t;

  typedef struct Timx_Handle_Mes
  {
    htimx_handle_t *(*Pwm_Timx_Create)(uint32_t hz, uint16_t psc, htimx_port_init_t *timx_port);
    uint8_t (*Pwm_Pulse_Change)(htimx_handle_t *TIMx, float k_pluse);
    void (*Pwm_Start)(htimx_handle_t *TIMx);
  } timx_handle_mes_t;

  timx_handle_mes_t *Htimx_Get_Methods();
  htimx_handle_t *Pwm_Timx_Create(uint32_t hz, uint16_t psc, htimx_port_init_t *timx_port);
  uint8_t Pwm_Pulse_Change(htimx_handle_t *TIMx, float k_pluse);
  void Pwm_Tim_Start(htimx_handle_t *TIMx);

#ifdef __cplusplus
}
#endif
#endif