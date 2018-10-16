// ----------------------------------------------------------------------------
// 3x2.h
//
//
// Authors:
// Peter Polidoro peterpolidoro@gmail.com
// ----------------------------------------------------------------------------
#ifndef RECURSIVE_PWM_CONTROLLER_3X2_CONSTANTS_H
#define RECURSIVE_PWM_CONTROLLER_3X2_CONSTANTS_H
#include "Constants.h"


#if defined(__MK20DX256__)

namespace recursive_pwm_controller
{
namespace constants
{
//MAX values must be >= 1, >= created/copied count, < RAM limit
enum {CHANNEL_COUNT_MAX=2};

enum{EVENT_COUNT_MAX=36};
enum{INDEXED_PWM_COUNT_MAX=16};

enum{PWM_LEVEL_COUNT_MAX=4};

extern const size_t signal_pins[CHANNEL_COUNT_MAX];

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
