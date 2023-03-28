#include "fixed_height_mode.h"

static void SetState(filght_mode_t *ctx, state_t *state);
static void Land();
static void Filght();

fixed_h_m_t *New_Fixed_Height_Mode(filght_mode_t *ctx)
{
  static fixed_h_m_t fixed_h_mode;

  fixed_h_mode.active = ctx->state;
  fixed_h_mode.state.Filght = Filght;
  fixed_h_mode.state.SetState = SetState;
  fixed_h_mode.state.Land = Land;

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
  
}
