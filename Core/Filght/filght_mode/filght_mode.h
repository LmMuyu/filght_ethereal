#ifndef _HAL_FILGHT_MODE_
#define _HAL_FILGHT_MODE_

#ifdef __cplusplus
extern "C"
{
#endif

#include "FlightConfig.h"
#include "state.h"
#include "config.h"

#include "fixed_height_mode.h"

  typedef struct Filght_Mode
  {
    state_t *state;
    float init_height;
    float curr_height;
    float fiexd_height;
    float max_up;
    float min_dn;

    void (*Filght_Mode_Init)(filght_mode_t *mode, state_t *init_state);
    void (*Land)(filght_mode_t *filght_mode);
    void (*Filght)(filght_mode_t *filght_mode);
  } filght_mode_t;

  extern fixed_h_m_t *fixed_h_mode;

  filght_mode_t *State_Machine();

#ifdef __cplusplus
}
#endif
#endif
