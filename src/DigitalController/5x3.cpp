// ----------------------------------------------------------------------------
// 5x3.cpp
//
//
// Authors:
// Peter Polidoro peterpolidoro@gmail.com
// ----------------------------------------------------------------------------
#include "5x3.h"


#if defined(__MK64FX512__)

namespace digital_controller
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
  23,
  14,
  15,
  16,
  17,
  18,
  19,
  20,
  21,
  40,
  41,
  42,
  50,
  49,
  48,
  47,
  43,
  44,
  45,
  46,
  33,
  34,
  37,
  38,
  53,
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
