// ----------------------------------------------------------------------------
// Constants.cpp
//
//
// Authors:
// Peter Polidoro peterpolidoro@gmail.com
// ----------------------------------------------------------------------------
#include "Constants.h"


namespace digital_controller
{
namespace constants
{
CONSTANT_STRING(device_name,"digital_controller");

CONSTANT_STRING(firmware_name,"DigitalController");
// Use semantic versioning http://semver.org/
const modular_server::FirmwareInfo firmware_info =
{
  .name_ptr=&firmware_name,
  .version_major=2,
  .version_minor=0,
  .version_patch=1,
};

CONSTANT_STRING(running_string,"running");
CONSTANT_STRING(level_string,"level");
CONSTANT_STRING(top_level_string,"top_level");
CONSTANT_STRING(child_index_string,"child_index");

const double power_lower_bound_default = 0.1;
const double power_upper_bound_default = 99.9;
CONSTANT_STRING(power_lower_bound_string,"power_lower_bound");
CONSTANT_STRING(power_upper_bound_string,"power_upper_bound");

// Pins

// Units
CONSTANT_STRING(ms_units,"ms");
CONSTANT_STRING(percent_units,"%");

// Properties
CONSTANT_STRING(channel_count_property_name,"channelCount");
const long channel_count_min = 1;

CONSTANT_STRING(power_max_property_name,"powerMax");
const double power_min = 0.0;
const double power_max = 100.0;
const double power_mid = power_max / 2;

// Parameters
CONSTANT_STRING(channel_parameter_name,"channel");
const long channel_min = 0;

CONSTANT_STRING(channels_parameter_name,"channels");

CONSTANT_STRING(power_parameter_name,"power");

CONSTANT_STRING(powers_parameter_name,"powers");

CONSTANT_STRING(delay_parameter_name,"delay");
const long delay_min = 0;
const long delay_max = 2000000000;

CONSTANT_STRING(period_parameter_name,"period");
const long period_min = 2;
const long period_max = 2000000000;

CONSTANT_STRING(on_duration_parameter_name,"on_duration");
const long on_duration_min = 1;
const long on_duration_max = 2000000000;

CONSTANT_STRING(count_parameter_name,"count");
const long count_min = 1;
const long count_max = 2000000000;

CONSTANT_STRING(pwm_index_parameter_name,"pwm_index");
const long pwm_index_min = 0;

CONSTANT_STRING(delays_parameter_name,"delays");

CONSTANT_STRING(periods_parameter_name,"periods");

CONSTANT_STRING(on_durations_parameter_name,"on_durations");

// Functions
CONSTANT_STRING(all_enabled_function_name,"allEnabled");
CONSTANT_STRING(set_power_when_on_function_name,"setPowerWhenOn");
CONSTANT_STRING(set_powers_when_on_function_name,"setPowersWhenOn");
CONSTANT_STRING(set_all_powers_when_on_function_name,"setAllPowersWhenOn");
CONSTANT_STRING(set_all_powers_when_on_to_max_function_name,"setAllPowersWhenOnToMax");
CONSTANT_STRING(get_powers_when_on_function_name,"getPowersWhenOn");
CONSTANT_STRING(get_powers_function_name,"getPowers");
CONSTANT_STRING(set_channel_on_function_name,"setChannelOn");
CONSTANT_STRING(set_channel_on_at_power_function_name,"setChannelOnAtPower");
CONSTANT_STRING(set_channel_off_function_name,"setChannelOff");
CONSTANT_STRING(set_channels_on_function_name,"setChannelsOn");
CONSTANT_STRING(set_channels_on_at_power_function_name,"setChannelsOnAtPower");
CONSTANT_STRING(set_channels_off_function_name,"setChannelsOff");
CONSTANT_STRING(toggle_channel_function_name,"toggleChannel");
CONSTANT_STRING(toggle_channels_function_name,"toggleChannels");
CONSTANT_STRING(set_all_channels_on_at_power_function_name,"setAllChannelsOnAtPower");
CONSTANT_STRING(set_channel_on_all_others_off_function_name,"setChannelOnAllOthersOff");
CONSTANT_STRING(set_channel_off_all_others_on_function_name,"setChannelOffAllOthersOn");
CONSTANT_STRING(set_channels_on_all_others_off_function_name,"setChannelsOnAllOthersOff");
CONSTANT_STRING(set_channels_off_all_others_on_function_name,"setChannelsOffAllOthersOn");
CONSTANT_STRING(channel_is_on_function_name,"channelIsOn");
CONSTANT_STRING(get_channels_on_function_name,"getChannelsOn");
CONSTANT_STRING(get_channels_off_function_name,"getChannelsOff");
CONSTANT_STRING(get_channel_count_function_name,"getChannelCount");
CONSTANT_STRING(add_pwm_function_name,"addPwm");
CONSTANT_STRING(start_pwm_function_name,"startPwm");
CONSTANT_STRING(add_recursive_pwm_function_name,"addRecursivePwm");
CONSTANT_STRING(start_recursive_pwm_function_name,"startRecursivePwm");
CONSTANT_STRING(stop_pwm_function_name,"stopPwm");
CONSTANT_STRING(stop_all_pwm_function_name,"stopAllPwm");
CONSTANT_STRING(get_channels_pwm_indexes_function_name,"getChannelsPwmIndexes");
CONSTANT_STRING(get_pwm_info_function_name,"getPwmInfo");
CONSTANT_STRING(get_power_bounds_function_name,"getPowerBounds");

// Callbacks
CONSTANT_STRING(enable_all_callback_name,"enableAll");
CONSTANT_STRING(disable_all_callback_name,"disableAll");
CONSTANT_STRING(toggle_all_channels_callback_name,"toggleAllChannels");
CONSTANT_STRING(set_all_channels_on_callback_name,"setAllChannelsOn");
CONSTANT_STRING(set_all_channels_off_callback_name,"setAllChannelsOff");

// Errors
CONSTANT_STRING(no_pwm_available_error,"Maximum number of PWM already set, must stop one to add another.");
CONSTANT_STRING(pwm_array_lengths_not_equal_error,"PWM parameter arrays must be equal length.");
CONSTANT_STRING(pwm_array_lengths_are_zero_error,"PWM parameter arrays must have length greater than 0.");
}
}
