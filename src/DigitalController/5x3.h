// ----------------------------------------------------------------------------
// 5x3.h
//
//
// Authors:
// Peter Polidoro peterpolidoro@gmail.com
// ----------------------------------------------------------------------------
#ifndef DIGITAL_CONTROLLER_5X3_CONSTANTS_H
#define DIGITAL_CONTROLLER_5X3_CONSTANTS_H
#include "Constants.h"


#if defined(__MK64FX512__)

namespace digital_controller
{
namespace constants
{
//MAX values must be >= 1, >= created/copied count, < RAM limit
enum {CHANNEL_COUNT_MAX=32};

enum{EVENT_COUNT_MAX=36};
enum{INDEXED_PWM_COUNT_MAX=16};

enum{PWM_LEVEL_COUNT_MAX=4};

extern const size_t signal_pins[CHANNEL_COUNT_MAX];

// Pins

// Units

// Properties
// Property values must be long, double, bool, long[], double[], bool[], char[], ConstantString *, (ConstantString *)[]
extern const long power_max_default[CHANNEL_COUNT_MAX];

// Parameters

// Functions

// Callbacks

// Errors
}
}
#endif
#endif
