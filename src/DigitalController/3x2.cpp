// ----------------------------------------------------------------------------
// 3x2.cpp
//
//
// Authors:
// Peter Polidoro peterpolidoro@gmail.com
// ----------------------------------------------------------------------------
#include "3x2.h"


#if defined(__MK20DX256__)

namespace recursive_pwm_controller
{
namespace constants
{
const size_t enable_pin = 2;

const size_t signal_pins[CHANNEL_COUNT_MAX] =
{
  3,
  4,
};

// Pins

// Units

// Properties
const long power_max_default[CHANNEL_COUNT_MAX] =
{
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
