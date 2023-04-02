#include "mitter.h"

mitt_t Mitter_mitt;

void Mitt_Init()
{
  Mitter_mitt = Mitter_Bus(5);
}

mitt_t Mitt_Get()
{
  return Mitter_mitt;
}