// ----------------------------------------------------------------------------
// 3x2.cpp
//
//
// Authors:
// Peter Polidoro peterpolidoro@gmail.com
// ----------------------------------------------------------------------------
#include "3x2.h"


#if defined(__MK20DX256__)

namespace digital_controller
{
namespace constants
{
const size_t enable_pin = 2;

const size_t signal_pins[CHANNEL_COUNT_MAX] =
{
  3,
  4,
  5,
  6,
  14,
  15,
  16,
  17,
  18,
  19,
  20,
  21,
  22,
  23,
  24,
  25,
};

// Pins

// Units

// Properties
const long channel_count_default = CHANNEL_COUNT_MAX;

const long power_max_default[CHANNEL_COUNT_MAX] =
{
  power_max,
  power_max,
  power_max,
  power_max,
  power_max,
  power_max,
  power_max,
  power_max,
  power_max,
  power_max,
  power_max,
  power_max,
  power_max,
  power_max,
  power_max,
  power_max,
};

// Parameters

// Functions

// Callbacks

// Errors
}
}
#endif
