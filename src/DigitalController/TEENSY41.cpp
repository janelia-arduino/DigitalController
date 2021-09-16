// ----------------------------------------------------------------------------
// TEENSY41.cpp
//
//
// Authors:
// Peter Polidoro peter@polidoro.io
// ----------------------------------------------------------------------------
#include "TEENSY41.h"


#if defined(__IMXRT1062__) && defined(ARDUINO_TEENSY41)
namespace digitial_controller
{
namespace constants
{
// Pins

// Units

// Properties
const long channel_count_default = CHANNEL_COUNT_MAX;

const double power_max_default[CHANNEL_COUNT_MAX] =
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
