#include "motor_control.h"

void Diff_And_PreRatio_Ratio(mx_pluse_t *mx_t, float pre_mxp, float diff_mx_p);

char motor_pdata[64];

// 电机状态数组
motor_t Motors[4];

timx_handle_mes_t *htimx_methods;

motor_dist_value_t MotorDistValue = {
    .M1 = {
        .mx = MOTOR_K_MIN,
        .pre_d_mx = 0,
    },
    .M2 = {
        .mx = MOTOR_K_MIN,
        .pre_d_mx = 0,
    },
    .M3 = {
        .mx = MOTOR_K_MIN,
        .pre_d_mx = 0,
    },
    .M4 = {
        .mx = MOTOR_K_MIN,
        .pre_d_mx = 0,
    },
};

void Motor_Init()
{

  /*
  PWM输出排针
  PWM2——PA7——TIM17_CH1
  PWM3——PA11——TIM4_CH1
  PWM4——PA12——TIM4_CH2
  PWM7——PA2——TIM15_CH1
  */

  htimx_methods = Htimx_Get_Methods();

  static htimx_port_init_t htimx_m1 = {
      .htimx = TIM17,
      .htimx_pin = GPIO_PIN_7,
      .htimx_reg = GPIOA,
      .k_init_pluse = MOTOR_K_MIN,
      .htimx_ch = TIM_CHANNEL_1,
  };

  static htimx_port_init_t htimx_m2 = {
      .htimx = TIM4,
      .htimx_pin = GPIO_PIN_11,
      .htimx_reg = GPIOA,
      .k_init_pluse = MOTOR_K_MIN,
      .htimx_ch = TIM_CHANNEL_1,
  };

  static htimx_port_init_t htimx_m3 = {
      .htimx = TIM4,
      .htimx_pin = GPIO_PIN_11,
      .htimx_reg = GPIOA,
      .k_init_pluse = MOTOR_K_MIN,
      .htimx_ch = TIM_CHANNEL_2,
  };

  static htimx_port_init_t htimx_m4 = {
      .htimx = TIM15,
      .htimx_pin = GPIO_PIN_2,
      .htimx_reg = GPIOA,
      .k_init_pluse = MOTOR_K_MIN,
      .htimx_ch = TIM_CHANNEL_1,
  };

  Motors[0].Mx = htimx_methods->Pwm_Timx_Create(MOTOR_PWM_HZ, MOTOR_PWM_PSC, &htimx_m1);
  Motors[0].htimx_p = &htimx_m1;

  Motors[1].Mx = htimx_methods->Pwm_Timx_Create(MOTOR_PWM_HZ, MOTOR_PWM_PSC, &htimx_m2);
  Motors[1].htimx_p = &htimx_m2;

  Motors[2].Mx = htimx_methods->Pwm_Timx_Create(MOTOR_PWM_HZ, MOTOR_PWM_PSC, &htimx_m3);
  Motors[2].htimx_p = &htimx_m3;

  Motors[3].Mx = htimx_methods->Pwm_Timx_Create(MOTOR_PWM_HZ, MOTOR_PWM_PSC, &htimx_m4);
  Motors[3].htimx_p = &htimx_m4;
}

float Expected_Thrust_Computed(expected_thrust_t expected_thrust)
{
  return KF * (expected_thrust.az + expected_thrust.gravity_g);
}

void Motor_Control_Distribution(float euler[3], expected_thrust_t *expected_thrust)
{
  // float T = Expected_Thrust_Computed(*(expected_thrust));
  float T = 1;

  /**
   *
   * ϕ=X
     φ=Y
     θ=Z
   *
 M1= T−0.70701ϕ+0.70701θ+φ
 M2= T+0.70701ϕ−0.70701θ+φ
 M3= T−0.70701ϕ−0.70701θ−φ
 M4= T+0.70701ϕ+0.70701θ−φ
  */

  // float roll = euler[0];
  // float yaw = euler[2];
  // float pitch = euler[1];

  /**
   * 3        1
   *  \      /
   *   \    /
   *      X
   *   /    \
   *  /      \
   * 2       4
   */

  float M1_p = 0.9 * T - 0.707 * euler[1] + 0.707 * euler[0] - euler[2];
  float M2_p = 0.9 * T + 0.707 * euler[1] + 0.707 * euler[0] + euler[2];
  float M3_p = 0.9 * T - 0.707 * euler[1] - 0.707 * euler[0] + euler[2];
  float M4_p = 0.9 * T + 0.707 * euler[1] - 0.707 * euler[0] - euler[2];

  Diff_Pwm_Motor_Pluse(&MotorDistValue, M1_p, M2_p, M3_p, M4_p);

  Pwm_Pulse_Change(Motors[0].Mx, MotorDistValue.M1.mx);
  Pwm_Pulse_Change(Motors[1].Mx, MotorDistValue.M2.mx);
  Pwm_Pulse_Change(Motors[2].Mx, MotorDistValue.M3.mx);
  Pwm_Pulse_Change(Motors[3].Mx, MotorDistValue.M4.mx);

  free(expected_thrust);
}

void Diff_Pwm_Motor_Pluse(motor_dist_value_t *motor_pluse, float M1_p, float M2_p, float M3_p, float M4_p)
{
  Diff_And_PreRatio_Ratio(&motor_pluse->M1, motor_pluse->M1.mx, M1_p);
  Diff_And_PreRatio_Ratio(&motor_pluse->M2, motor_pluse->M2.mx, M2_p);
  Diff_And_PreRatio_Ratio(&motor_pluse->M3, motor_pluse->M3.mx, M3_p);
  Diff_And_PreRatio_Ratio(&motor_pluse->M4, motor_pluse->M4.mx, M4_p);

  // sprintf(motor_pdata, "M1:%f M2:%f M3:%f M4:%f \n", motor_pluse->M1.mx, motor_pluse->M2.mx, motor_pluse->M3.mx, motor_pluse->M4.mx);
  // Hal_Write_Buf(motor_pdata);
  // Hal_SendData();
}

void Diff_And_PreRatio_Ratio(mx_pluse_t *mx_t, float pre_mxp, float diff_mx_p)
{
  mx_t->pre_d_mx = diff_mx_p;

  float pluse_mx_p = diff_mx_p / 1000;

  if (pre_mxp + pluse_mx_p < MOTOR_K_MIN)
  {
    mx_t->mx = MOTOR_K_MIN;
  }
  else if (pre_mxp + pluse_mx_p > MOTOR_K_MAX)
  {
    mx_t->mx = MOTOR_K_MAX;
  }
  else
  {
    mx_t->mx = pre_mxp + pluse_mx_p;
  }
}