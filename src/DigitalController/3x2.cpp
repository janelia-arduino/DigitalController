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

const long switching_frequency_max_default[CHANNEL_COUNT_MAX] =
{
  switching_frequency_max,
  switching_frequency_max,
  switching_frequency_max,
  switching_frequency_max,
  switching_frequency_max,
  switching_frequency_max,
  switching_frequency_max,
  switching_frequency_max,
  switching_frequency_max,
  switching_frequency_max,
  switching_frequency_max,
  switching_frequency_max,
  switching_frequency_max,
  switching_frequency_max,
  switching_frequency_max,
  switching_frequency_max,
};

// Parameters

// Functions

// Callbacks

// Errors
}
}
#endif
