#ifndef _HAL_STATE_H_
#define _HAL_STATE_H_

#ifdef __cplusplus
extern "C"
{
#endif
#include "filght_mode.h"
#include "config.h"

  typedef struct State
  {
    void (*SetState)(filght_mode_t *ctx, state_t *state);
    void (*Land)(filght_mode_t *ctx);
    void (*Filght)(filght_mode_t *ctx);
  } state_t;

#ifdef __cplusplus
}
#endif
#endif