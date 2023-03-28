#include "fixed_height_mode.h"

static void SetState(filght_mode_t *ctx, state_t *state);
static void Land(filght_mode_t *ctx);
static void Filght(filght_mode_t *ctx);

char pdata[64];

pid_t fixed_height_pid;
volatile pid_c_t pid_value = {
    .D = 0.05,
    .I = 0.01,
    .P = 0.001,
};

fixed_h_m_t *New_Fixed_Height_Mode(filght_mode_t *ctx)
{
  static fixed_h_m_t fixed_h_mode;

  fixed_h_mode.active = ctx->state;
  fixed_h_mode.state->Filght = Filght;
  fixed_h_mode.state->SetState = SetState;
  fixed_h_mode.state->Land = Land;

  return &fixed_h_mode;
}

static void SetState(filght_mode_t *ctx, state_t *state)
{
  if (ctx->state != state)
  {
    ctx->state = state;
  }
}

static void Land(filght_mode_t *ctx)
{
}

static void Filght(filght_mode_t *ctx)
{
  const int32_t mix_diff = ctx->fiexd_height * 0.05;

  if (ctx->curr_height >= ctx->init_height - mix_diff)
  {
    ctx->state->SetState(ctx, ctx->state);
    return;
  }

  fixed_height_pid.target_value = ctx->init_height + ctx->fiexd_height;
  fixed_height_pid.watch_v = ctx->curr_height;

  const float pid_v = Pid_Incremental(&fixed_height_pid, &pid_value);

  sprintf(pdata, "pid_v:%f\n", pid_v);
  Hal_Write_Buf(pdata);
  Hal_SendData();

  float pwm = (1 / pid_v) + MOTOR_K_MIN;

  Diff_Pwm_Motor_Pluse(&MotorDistValue, pwm, pwm, pwm, pwm);
}
