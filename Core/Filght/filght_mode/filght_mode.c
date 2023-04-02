#include "filght_mode.h"

static void Filght_Mode_Init(filght_mode_t *mode, state_t *init_state);
static void Land(filght_mode_t *filght_mode);
static void Filght(filght_mode_t *filght_mode);

fixed_h_m_t *fixed_h_mode;

filght_mode_t *State_Machine()
{
  static filght_mode_t filght_mode;

  filght_mode.fiexd_height = Poweron_Drive_Height;
  filght_mode.curr_height = 0;
  filght_mode.max_up = MPC_Z_VEL_MAX_UP;
  filght_mode.min_dn = MPC_Z_VEL_MAX_DN;

  filght_mode.Filght_Mode_Init = Filght_Mode_Init;
  filght_mode.Land = Land;
  filght_mode.Filght = Filght;

  fixed_h_mode = New_Fixed_Height_Mode(&filght_mode);
  // 默认状态
  filght_mode.state = fixed_h_mode->state;

  return &filght_mode;
}

void Filght_Mode_Init(filght_mode_t *mode, state_t *init_state)
{
  if (mode->state != init_state)
  {
    mode->state = init_state;
  }
}

void Land(filght_mode_t *filght_mode)
{
}

void Filght(filght_mode_t *filght_mode)
{
  filght_mode->state->Filght(filght_mode);
}
