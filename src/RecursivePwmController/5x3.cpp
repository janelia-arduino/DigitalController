// ----------------------------------------------------------------------------
// 5x3.cpp
//
//
// Authors:
// Peter Polidoro peterpolidoro@gmail.com
// ----------------------------------------------------------------------------
#include "5x3.h"


#if defined(__MK64FX512__)

namespace recursive_pwm_controller
{
namespace constants
{
const size_t enable_pin = 52;

const size_t signal_pins[CHANNEL_COUNT_MAX] =
{
  2,
  6,
  29,
  30,
  35,
  36,
  22,
  23
};

// Pins

// Units

// Properties
const long power_max_default[CHANNEL_COUNT_MAX] =
{
  100,
  100,
  100,
  100,
  100,
  100,
  100,
  100,
};

// Parameters

// Functions

// Callbacks

// Errors
}
}
#endif
