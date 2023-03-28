#ifndef _HAL_MOTOR_
#define _HAL_MOTOR_
#ifdef __cplusplus
extern "C"
{
#endif
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "FreeRTOSConfig.h"
#include "freertos_task.h"
#include "usb.h"
#include "stdio.h"
#include "pwm.h"

#define KF 0.6f

#define MOTOR_K_MIN 0.4000f
#define MOTOR_K_MAX 0.8000f

#define MOTOR_PWM_HZ 400
#define MOTOR_PWM_PSC 8

  typedef struct
  {
    float az;
    float gravity_g;
  } expected_thrust_t;

  typedef struct Mx_Pluse
  {
    float mx;
    float pre_d_mx;
  } mx_pluse_t;

  typedef struct Motor_Dist_Value
  {
    mx_pluse_t M1;
    mx_pluse_t M2;
    mx_pluse_t M3;
    mx_pluse_t M4;
  } motor_dist_value_t;

  typedef struct MOTOR
  {
    htimx_handle_t *Mx;
    htimx_port_init_t *htimx_p;
  } motor_t;

  extern timx_handle_mes_t *htimx_methods;
  extern motor_dist_value_t MotorDistValue;
  extern motor_t Motors[4];

  void Motor_Init();
  float Expected_Thrust_Computed(expected_thrust_t expected_thrust);
  void Motor_Control_Distribution(float euler[3], expected_thrust_t *expected_thrust);
  void Diff_Pwm_Motor_Pluse(motor_dist_value_t *motor_pluse, float M1_p, float M2_p, float M3_p, float M4_p);

#ifdef __cplusplus
}
#endif
#endif
