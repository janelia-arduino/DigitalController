// ----------------------------------------------------------------------------
// DigitalController.cpp
//
//
// Authors:
// Peter Polidoro peter@polidoro.io
// ----------------------------------------------------------------------------
#include "../DigitalController.h"


using namespace digital_controller;

void DigitalController::setup()
{
  // Parent Setup
  ModularDeviceBase::setup();

  // Reset Watchdog
  resetWatchdog();

  // Variable Setup
  channels_ = 0;
  initializePwmIndexes();
  dummy_functor_ = makeFunctor((Functor1<int> *)0,*this,&DigitalController::dummyHandler);

  // Event Controller Setup
  event_controller_.setup();

  // Set Device ID
  modular_server_.setDeviceName(constants::device_name);

  // Pin Setup

  // Pins
  modular_server::Pin & btn_a_pin = modular_server_.pin(modular_device_base::constants::btn_a_pin_name);

  modular_server::Pin & btn_b_pin = modular_server_.pin(modular_device_base::constants::btn_b_pin_name);

  // Add Hardware

  // Add Firmware
  modular_server_.addFirmware(constants::firmware_info,
    properties_,
    parameters_,
    functions_,
    callbacks_);

  // Properties
  modular_server::Property & channel_count_property = modular_server_.createProperty(constants::channel_count_property_name,constants::channel_count_default);
  channel_count_property.setRange(constants::channel_count_min,constants::CHANNEL_COUNT_MAX);
  channel_count_property.attachPostSetValueFunctor(makeFunctor((Functor0 *)0,*this,&DigitalController::setChannelCountHandler));

  modular_server::Property & power_max_property = modular_server_.createProperty(constants::power_max_property_name,constants::power_max_default);
  power_max_property.setRange(constants::power_min,constants::power_max);
  power_max_property.setUnits(constants::percent_units);
  power_max_property.attachPostSetElementValueFunctor(makeFunctor((Functor1<size_t> *)0,*this,&DigitalController::setPowerMaxHandler));

  // Parameters
  modular_server::Parameter & channel_parameter = modular_server_.createParameter(constants::channel_parameter_name);

  modular_server::Parameter & channels_parameter = modular_server_.createParameter(constants::channels_parameter_name);

  modular_server::Parameter & power_parameter = modular_server_.createParameter(constants::power_parameter_name);
  power_parameter.setRange(constants::power_min,constants::power_max);
  power_parameter.setUnits(constants::percent_units);

  modular_server::Parameter & powers_parameter = modular_server_.createParameter(constants::powers_parameter_name);
  powers_parameter.setRange(constants::power_min,constants::power_max);
  powers_parameter.setUnits(constants::percent_units);

  modular_server::Parameter & delay_parameter = modular_server_.createParameter(constants::delay_parameter_name);
  delay_parameter.setRange(constants::delay_min,constants::delay_max);
  delay_parameter.setUnits(constants::ms_units);

  modular_server::Parameter & period_parameter = modular_server_.createParameter(constants::period_parameter_name);
  period_parameter.setRange(constants::period_min,constants::period_max);
  period_parameter.setUnits(constants::ms_units);

  modular_server::Parameter & on_duration_parameter = modular_server_.createParameter(constants::on_duration_parameter_name);
  on_duration_parameter.setRange(constants::on_duration_min,constants::on_duration_max);
  on_duration_parameter.setUnits(constants::ms_units);

  modular_server::Parameter & count_parameter = modular_server_.createParameter(constants::count_parameter_name);
  count_parameter.setRange(constants::count_min,constants::count_max);

  modular_server::Parameter & pwm_index_parameter = modular_server_.createParameter(constants::pwm_index_parameter_name);
  pwm_index_parameter.setRange(constants::pwm_index_min,(long)constants::INDEXED_PWM_COUNT_MAX-1);

  // modular_server::Parameter & delays_parameter = modular_server_.createParameter(constants::delays_parameter_name);
  // delays_parameter.setRange(constants::delay_min,constants::delay_max);
  // delays_parameter.setArrayLengthRange(1,constants::PWM_LEVEL_COUNT_MAX);
  // delays_parameter.setUnits(constants::ms_units);

  modular_server::Parameter & periods_parameter = modular_server_.createParameter(constants::periods_parameter_name);
  periods_parameter.setRange(constants::period_min,constants::period_max);
  periods_parameter.setArrayLengthRange(1,constants::PWM_LEVEL_COUNT_MAX);
  periods_parameter.setUnits(constants::ms_units);

  modular_server::Parameter & on_durations_parameter = modular_server_.createParameter(constants::on_durations_parameter_name);
  on_durations_parameter.setRange(constants::on_duration_min,constants::on_duration_max);
  on_durations_parameter.setArrayLengthRange(1,constants::PWM_LEVEL_COUNT_MAX);
  on_durations_parameter.setUnits(constants::ms_units);

  setChannelCountHandler();

  // Functions
  modular_server::Function & all_enabled_function = modular_server_.createFunction(constants::all_enabled_function_name);
  all_enabled_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&DigitalController::allEnabledHandler));
  all_enabled_function.setResultTypeBool();

  modular_server::Function & set_power_when_on_function = modular_server_.createFunction(constants::set_power_when_on_function_name);
  set_power_when_on_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&DigitalController::setPowerWhenOnHandler));
  set_power_when_on_function.addParameter(channel_parameter);
  set_power_when_on_function.addParameter(power_parameter);
  set_power_when_on_function.setResultTypeDouble();

  modular_server::Function & set_powers_when_on_function = modular_server_.createFunction(constants::set_powers_when_on_function_name);
  set_powers_when_on_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&DigitalController::setPowersWhenOnHandler));
  set_powers_when_on_function.addParameter(powers_parameter);
  set_powers_when_on_function.setResultTypeArray();
  set_powers_when_on_function.setResultTypeDouble();

  modular_server::Function & set_all_powers_when_on_function = modular_server_.createFunction(constants::set_all_powers_when_on_function_name);
  set_all_powers_when_on_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&DigitalController::setAllPowersWhenOnHandler));
  set_all_powers_when_on_function.addParameter(power_parameter);
  set_all_powers_when_on_function.setResultTypeArray();
  set_all_powers_when_on_function.setResultTypeDouble();

  modular_server::Function & set_all_powers_when_on_to_max_function = modular_server_.createFunction(constants::set_all_powers_when_on_to_max_function_name);
  set_all_powers_when_on_to_max_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&DigitalController::setAllPowersWhenOnToMaxHandler));
  set_all_powers_when_on_to_max_function.setResultTypeArray();
  set_all_powers_when_on_to_max_function.setResultTypeDouble();

  modular_server::Function & get_powers_when_on_function = modular_server_.createFunction(constants::get_powers_when_on_function_name);
  get_powers_when_on_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&DigitalController::getPowersWhenOnHandler));
  get_powers_when_on_function.setResultTypeArray();
  get_powers_when_on_function.setResultTypeDouble();

  modular_server::Function & get_powers_function = modular_server_.createFunction(constants::get_powers_function_name);
  get_powers_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&DigitalController::getPowersHandler));
  get_powers_function.setResultTypeArray();
  get_powers_function.setResultTypeDouble();

  modular_server::Function & set_channel_on_function = modular_server_.createFunction(constants::set_channel_on_function_name);
  set_channel_on_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&DigitalController::setChannelOnHandler));
  set_channel_on_function.addParameter(channel_parameter);

  modular_server::Function & set_channel_on_at_power_function = modular_server_.createFunction(constants::set_channel_on_at_power_function_name);
  set_channel_on_at_power_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&DigitalController::setChannelOnAtPowerHandler));
  set_channel_on_at_power_function.addParameter(channel_parameter);
  set_channel_on_at_power_function.addParameter(power_parameter);

  modular_server::Function & set_channel_off_function = modular_server_.createFunction(constants::set_channel_off_function_name);
  set_channel_off_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&DigitalController::setChannelOffHandler));
  set_channel_off_function.addParameter(channel_parameter);

  modular_server::Function & set_channels_on_function = modular_server_.createFunction(constants::set_channels_on_function_name);
  set_channels_on_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&DigitalController::setChannelsOnHandler));
  set_channels_on_function.addParameter(channels_parameter);

  modular_server::Function & set_channels_on_at_power_function = modular_server_.createFunction(constants::set_channels_on_at_power_function_name);
  set_channels_on_at_power_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&DigitalController::setChannelsOnAtPowerHandler));
  set_channels_on_at_power_function.addParameter(channels_parameter);
  set_channels_on_at_power_function.addParameter(power_parameter);

  modular_server::Function & set_channels_off_function = modular_server_.createFunction(constants::set_channels_off_function_name);
  set_channels_off_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&DigitalController::setChannelsOffHandler));
  set_channels_off_function.addParameter(channels_parameter);

  modular_server::Function & toggle_channel_function = modular_server_.createFunction(constants::toggle_channel_function_name);
  toggle_channel_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&DigitalController::toggleChannelHandler));
  toggle_channel_function.addParameter(channel_parameter);

  modular_server::Function & toggle_channels_function = modular_server_.createFunction(constants::toggle_channels_function_name);
  toggle_channels_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&DigitalController::toggleChannelsHandler));
  toggle_channels_function.addParameter(channels_parameter);

  modular_server::Function & set_all_channels_on_at_power_function = modular_server_.createFunction(constants::set_all_channels_on_at_power_function_name);
  set_all_channels_on_at_power_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&DigitalController::setAllChannelsOnAtPowerHandler));
  set_all_channels_on_at_power_function.addParameter(power_parameter);

  modular_server::Function & set_channel_on_all_others_off_function = modular_server_.createFunction(constants::set_channel_on_all_others_off_function_name);
  set_channel_on_all_others_off_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&DigitalController::setChannelOnAllOthersOffHandler));
  set_channel_on_all_others_off_function.addParameter(channel_parameter);

  modular_server::Function & set_channel_off_all_others_on_function = modular_server_.createFunction(constants::set_channel_off_all_others_on_function_name);
  set_channel_off_all_others_on_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&DigitalController::setChannelOffAllOthersOnHandler));
  set_channel_off_all_others_on_function.addParameter(channel_parameter);

  modular_server::Function & set_channels_on_all_others_off_function = modular_server_.createFunction(constants::set_channels_on_all_others_off_function_name);
  set_channels_on_all_others_off_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&DigitalController::setChannelsOnAllOthersOffHandler));
  set_channels_on_all_others_off_function.addParameter(channels_parameter);

  modular_server::Function & set_channels_off_all_others_on_function = modular_server_.createFunction(constants::set_channels_off_all_others_on_function_name);
  set_channels_off_all_others_on_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&DigitalController::setChannelsOffAllOthersOnHandler));
  set_channels_off_all_others_on_function.addParameter(channels_parameter);

  modular_server::Function & channel_is_on_function = modular_server_.createFunction(constants::channel_is_on_function_name);
  channel_is_on_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&DigitalController::channelIsOnHandler));
  channel_is_on_function.addParameter(channel_parameter);
  channel_is_on_function.setResultTypeBool();

  modular_server::Function & get_channels_on_function = modular_server_.createFunction(constants::get_channels_on_function_name);
  get_channels_on_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&DigitalController::getChannelsOnHandler));
  get_channels_on_function.setResultTypeArray();
  get_channels_on_function.setResultTypeLong();

  modular_server::Function & get_channels_off_function = modular_server_.createFunction(constants::get_channels_off_function_name);
  get_channels_off_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&DigitalController::getChannelsOffHandler));
  get_channels_off_function.setResultTypeArray();
  get_channels_off_function.setResultTypeLong();

  modular_server::Function & get_channel_count_function = modular_server_.createFunction(constants::get_channel_count_function_name);
  get_channel_count_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&DigitalController::getChannelCountHandler));

  modular_server::Function & add_pwm_function = modular_server_.createFunction(constants::add_pwm_function_name);
  add_pwm_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&DigitalController::addPwmHandler));
  add_pwm_function.addParameter(channels_parameter);
  add_pwm_function.addParameter(power_parameter);
  add_pwm_function.addParameter(delay_parameter);
  add_pwm_function.addParameter(period_parameter);
  add_pwm_function.addParameter(on_duration_parameter);
  add_pwm_function.addParameter(count_parameter);
  add_pwm_function.setResultTypeLong();

  modular_server::Function & start_pwm_function = modular_server_.createFunction(constants::start_pwm_function_name);
  start_pwm_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&DigitalController::startPwmHandler));
  start_pwm_function.addParameter(channels_parameter);
  start_pwm_function.addParameter(power_parameter);
  start_pwm_function.addParameter(delay_parameter);
  start_pwm_function.addParameter(period_parameter);
  start_pwm_function.addParameter(on_duration_parameter);
  start_pwm_function.setResultTypeLong();

  modular_server::Function & add_recursive_pwm_function = modular_server_.createFunction(constants::add_recursive_pwm_function_name);
  add_recursive_pwm_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&DigitalController::addRecursivePwmHandler));
  add_recursive_pwm_function.addParameter(channels_parameter);
  add_recursive_pwm_function.addParameter(power_parameter);
  add_recursive_pwm_function.addParameter(delay_parameter);
  add_recursive_pwm_function.addParameter(periods_parameter);
  add_recursive_pwm_function.addParameter(on_durations_parameter);
  add_recursive_pwm_function.addParameter(count_parameter);
  add_recursive_pwm_function.setResultTypeLong();

  modular_server::Function & start_recursive_pwm_function = modular_server_.createFunction(constants::start_recursive_pwm_function_name);
  start_recursive_pwm_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&DigitalController::startRecursivePwmHandler));
  start_recursive_pwm_function.addParameter(channels_parameter);
  start_recursive_pwm_function.addParameter(power_parameter);
  start_recursive_pwm_function.addParameter(delay_parameter);
  start_recursive_pwm_function.addParameter(periods_parameter);
  start_recursive_pwm_function.addParameter(on_durations_parameter);
  start_recursive_pwm_function.setResultTypeLong();

  modular_server::Function & stop_pwm_function = modular_server_.createFunction(constants::stop_pwm_function_name);
  stop_pwm_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&DigitalController::stopPwmHandler));
  stop_pwm_function.addParameter(pwm_index_parameter);

  modular_server::Function & stop_all_pwm_function = modular_server_.createFunction(constants::stop_all_pwm_function_name);
  stop_all_pwm_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&DigitalController::stopAllPwmHandler));

  modular_server::Function & get_channels_pwm_indexes_function = modular_server_.createFunction(constants::get_channels_pwm_indexes_function_name);
  get_channels_pwm_indexes_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&DigitalController::getChannelsPwmIndexesHandler));
  get_channels_pwm_indexes_function.setResultTypeArray();
  get_channels_pwm_indexes_function.setResultTypeLong();

  modular_server::Function & get_pwm_info_function = modular_server_.createFunction(constants::get_pwm_info_function_name);
  get_pwm_info_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&DigitalController::getPwmInfoHandler));
  get_pwm_info_function.setResultTypeArray();
  get_pwm_info_function.setResultTypeObject();

  modular_server::Function & get_power_bounds_function = modular_server_.createFunction(constants::get_power_bounds_function_name);
  get_power_bounds_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&DigitalController::getPowerBoundsHandler));
  get_power_bounds_function.setResultTypeArray();
  get_power_bounds_function.setResultTypeObject();

  // Callbacks
  modular_server::Callback & enable_all_callback = modular_server_.createCallback(constants::enable_all_callback_name);
  enable_all_callback.attachFunctor(makeFunctor((Functor1<modular_server::Pin *> *)0,*this,&DigitalController::enableAllHandler));

  modular_server::Callback & disable_all_callback = modular_server_.createCallback(constants::disable_all_callback_name);
  disable_all_callback.attachFunctor(makeFunctor((Functor1<modular_server::Pin *> *)0,*this,&DigitalController::disableAllHandler));

  modular_server::Callback & toggle_all_channels_callback = modular_server_.createCallback(constants::toggle_all_channels_callback_name);
  toggle_all_channels_callback.attachFunctor(makeFunctor((Functor1<modular_server::Pin *> *)0,*this,&DigitalController::toggleAllChannelsHandler));

  modular_server::Callback & set_all_channels_on_callback = modular_server_.createCallback(constants::set_all_channels_on_callback_name);
  set_all_channels_on_callback.attachFunctor(makeFunctor((Functor1<modular_server::Pin *> *)0,*this,&DigitalController::setAllChannelsOnHandler));
  set_all_channels_on_callback.attachTo(btn_a_pin,modular_server::constants::pin_mode_interrupt_falling);

  modular_server::Callback & set_all_channels_off_callback = modular_server_.createCallback(constants::set_all_channels_off_callback_name);
  set_all_channels_off_callback.attachFunctor(makeFunctor((Functor1<modular_server::Pin *> *)0,*this,&DigitalController::setAllChannelsOffHandler));
  set_all_channels_off_callback.attachTo(btn_b_pin,modular_server::constants::pin_mode_interrupt_falling);


}

void DigitalController::enableAll()
{
  all_enabled_ = true;
}

void DigitalController::disableAll()
{
  all_enabled_ = false;
}

bool DigitalController::allEnabled()
{
  return all_enabled_;
}

double DigitalController::setPowerWhenOn(Channel channel,
  double power)
{
  double power_to_set = 0;
  if (channel < getChannelCount())
  {
    power_to_set = power;
    if (power_to_set < constants::power_min)
    {
      power_to_set = constants::power_min;
    }
    else
    {
      modular_server::Property & power_max_property = modular_server_.property(constants::power_max_property_name);
      double power_max;
      power_max_property.getElementValue(channel,power_max);
      if (power_to_set > power_max)
      {
        power_to_set = power_max;
      }
    }
    noInterrupts();
    powers_when_on_[channel] = power_to_set;
    interrupts();
    updateChannel(channel);
  }
  return power_to_set;
}

double DigitalController::setPowerWhenOnToMax(Channel channel)
{
  double power_to_set = 0;
  if (channel < getChannelCount())
  {
    modular_server::Property & power_max_property = modular_server_.property(constants::power_max_property_name);
    double power_max;
    power_max_property.getElementValue(channel,power_max);

    power_to_set = setPowerWhenOn(channel,power_max);
  }
  return power_to_set;
}

void DigitalController::setAllPowersWhenOnToMax()
{
  modular_server::Property & power_max_property = modular_server_.property(constants::power_max_property_name);
  double power_max;
  for (Channel channel=0; channel<getChannelCount(); ++channel)
  {
    power_max_property.getElementValue(channel,power_max);
    noInterrupts();
    powers_when_on_[channel] = power_max;
    interrupts();
  }
}

double DigitalController::getPowerWhenOn(Channel channel)
{
  double power = constants::power_min;
  if (channel < getChannelCount())
  {
    noInterrupts();
    power = powers_when_on_[channel];
    interrupts();
  }
  return power;
}

double DigitalController::getPower(Channel channel)
{
  double power = constants::power_min;
  if (channel < getChannelCount())
  {
    noInterrupts();
    power = powers_[channel];
    interrupts();
  }
  return power;
}

void DigitalController::setChannels(Channels channels)
{
  Channels bit = 1;
  for (Channel channel=0; channel<getChannelCount(); ++channel)
  {
    if ((bit << channel) & channels)
    {
      setChannelOn(channel);
    }
    else
    {
      setChannelOff(channel);
    }
  }
}

void DigitalController::setChannelOn(Channel channel)
{
  if (channel < getChannelCount())
  {
    noInterrupts();
    double power = powers_when_on_[channel];
    interrupts();

    setChannelOnAtPower(channel,power);
  }
}

void DigitalController::setChannelOnAtPower(Channel channel,
  double power)
{
  if (channel < getChannelCount())
  {
    Channels bit = 1;
    bit = bit << channel;

    if (power < getPowerLowerBound(channel))
    {
      setChannelOff(channel);
      return;
    }

    noInterrupts();
    channels_ |= bit;
    power = setChannelToPower(channel,power);
    powers_[channel] = power;
    interrupts();
  }
}

void DigitalController::setChannelOff(Channel channel)
{
  if (channel < getChannelCount())
  {
    Channels bit = 1;
    bit = bit << channel;

    double power = constants::power_min;
    noInterrupts();
    channels_ &= ~bit;
    power = setChannelToPower(channel,power);
    powers_[channel] = power;
    interrupts();
  }
}

void DigitalController::setChannelsOn(Channels channels)
{
  Channels bit = 1;
  for (Channel channel=0; channel<getChannelCount(); ++channel)
  {
    if ((bit << channel) & channels)
    {
      setChannelOn(channel);
    }
  }
}

void DigitalController::setChannelsOnAtPower(Channels channels,
  double power)
{
  Channels bit = 1;
  for (Channel channel=0; channel<getChannelCount(); ++channel)
  {
    if ((bit << channel) & channels)
    {
      setChannelOnAtPower(channel,power);
    }
  }
}

void DigitalController::setChannelsOff(Channels channels)
{
  Channels bit = 1;
  for (Channel channel=0; channel<getChannelCount(); ++channel)
  {
    if ((bit << channel) & channels)
    {
      setChannelOff(channel);
    }
  }
}

void DigitalController::toggleChannel(Channel channel)
{
  if (channel < getChannelCount())
  {
    Channels bit = 1;
    bit = bit << channel;
    noInterrupts();
    Channels channels = channels_;
    interrupts();
    channels ^= bit;
    if (bit & channels)
    {
      setChannelOn(channel);
    }
    else
    {
      setChannelOff(channel);
    }
  }
}

void DigitalController::toggleChannels(Channels channels)
{
  noInterrupts();
  channels_ ^= channels;
  interrupts();
  setChannels(channels_);
}

void DigitalController::toggleAllChannels()
{
  noInterrupts();
  channels_ = ~channels_;
  interrupts();
  setChannels(channels_);
}

void DigitalController::setAllChannelsOn()
{
  for (Channel channel=0; channel<getChannelCount(); ++channel)
  {
    setChannelOn(channel);
  }
}

void DigitalController::setAllChannelsOnAtPower(double power)
{
  for (Channel channel=0; channel<getChannelCount(); ++channel)
  {
    setChannelOnAtPower(channel,power);
  }
}

void DigitalController::setAllChannelsOff()
{
  for (Channel channel=0; channel<getChannelCount(); ++channel)
  {
    setChannelOff(channel);
  }
}

void DigitalController::setChannelOnAllOthersOff(Channel channel)
{
  if (channel < getChannelCount())
  {
    Channels bit = 1;
    bit = bit << channel;
    noInterrupts();
    channels_ = bit;
    interrupts();
    setChannels(channels_);
  }
}

void DigitalController::setChannelOffAllOthersOn(Channel channel)
{
  if (channel < getChannelCount())
  {
    Channels bit = 1;
    bit = bit << channel;
    noInterrupts();
    channels_ = ~bit;
    interrupts();
    setChannels(channels_);
  }
}

void DigitalController::setChannelsOnAllOthersOff(Channels channels)
{
  noInterrupts();
  channels_ = channels;
  interrupts();
  setChannels(channels_);
}

void DigitalController::setChannelsOffAllOthersOn(Channels channels)
{
  noInterrupts();
  channels_ = ~channels;
  interrupts();
  setChannels(channels_);
}

bool DigitalController::channelIsOn(Channel channel)
{
  bool channel_is_on = false;
  if (channel < getChannelCount())
  {
    noInterrupts();
    Channels channels = channels_;
    interrupts();
    Channels bit = 1;
    if ((bit << channel) & channels)
    {
      channel_is_on = true;
    }
  }
  return channel_is_on;
}

DigitalController::Channels DigitalController::getChannelsOn()
{
  return channels_;
}

DigitalController::Channel DigitalController::getChannelCount()
{
  long channel_count;
  modular_server_.property(constants::channel_count_property_name).getValue(channel_count);

  // channel_count may contain bad values
  if (channel_count < constants::channel_count_min)
  {
    channel_count = constants::channel_count_min;
  }
  else if (channel_count > constants::CHANNEL_COUNT_MAX)
  {
    channel_count = constants::CHANNEL_COUNT_MAX;
  }

  return channel_count;
}

digital_controller::constants::PwmId DigitalController::addPwm(Channels channels,
  double power,
  long delay,
  long period,
  long on_duration,
  long count)
{
  return addPwm(channels,
    power,
    delay,
    period,
    on_duration,
    count,
    makeFunctor((Functor1<int> *)0,*this,&DigitalController::startPulseHandler),
    makeFunctor((Functor1<int> *)0,*this,&DigitalController::stopPulseHandler),
    makeFunctor((Functor1<int> *)0,*this,&DigitalController::startPwmHandler),
    makeFunctor((Functor1<int> *)0,*this,&DigitalController::stopPwmHandler));
}

digital_controller::constants::PwmId DigitalController::addPwm(Channels channels,
  double power,
  long delay,
  long period,
  long on_duration,
  long count,
  const Functor1<int> & start_pulse_functor,
  const Functor1<int> & stop_pulse_functor,
  const Functor1<int> & start_pwm_functor,
  const Functor1<int> & stop_pwm_functor)
{
  constants::PwmId pwm_id;
  if (pwm_info_container_.full() || (event_controller_.eventsAvailable() < 2))
  {
    pwm_id.index = constants::NO_PWM_AVAILABLE_INDEX;
    return pwm_id;
  }
  digital_controller::constants::PwmInfo pwm_info;
  pwm_info.channels = channels;
  pwm_info.running = false;
  pwm_info.level = 0;
  pwm_info.top_level = true;
  pwm_info.child_index = constants::NO_CHILD_PWM_INDEX;
  pwm_info.power = power;
  pwm_info.delay = delay;
  pwm_info.period = period;
  pwm_info.on_duration = on_duration;
  pwm_info.count = count;
  pwm_info.stopped_before_count_completed = false;
  pwm_info.count_completed_functor = dummy_functor_;
  pwm_info.functor_arg = -1;
  pwm_info.start_pulse_functor = start_pulse_functor;
  pwm_info.stop_pulse_functor = stop_pulse_functor;
  pwm_info.start_pwm_functor = start_pwm_functor;
  pwm_info.stop_pwm_functor = stop_pwm_functor;
  pwm_id.index = pwm_info_container_.add(pwm_info);
  if (pwm_id.index < 0)
  {
    return pwm_id;
  }
  pwm_id.event_id_pair = event_controller_.addPwmUsingDelay(
    makeFunctor((Functor1<int> *)0,*this,&DigitalController::startPulseWrapperHandler),
    makeFunctor((Functor1<int> *)0,*this,&DigitalController::stopPulseWrapperHandler),
    delay,
    period,
    on_duration,
    count,
    pwm_id.index);
  event_controller_.addStartFunctor(pwm_id.event_id_pair,
    makeFunctor((Functor1<int> *)0,*this,&DigitalController::startPwmWrapperHandler));
  event_controller_.addStopFunctor(pwm_id.event_id_pair,
    makeFunctor((Functor1<int> *)0,*this,&DigitalController::stopPwmWrapperHandler));
  pwm_info_container_[pwm_id.index].event_id_pair = pwm_id.event_id_pair;
  event_controller_.enable(pwm_id.event_id_pair);
  return pwm_id;
}

digital_controller::constants::PwmId DigitalController::startPwm(Channels channels,
  double power,
  long delay,
  long period,
  long on_duration)
{
  return addPwm(channels,power,delay,period,on_duration,-1);
}

digital_controller::constants::PwmId DigitalController::addRecursivePwm(Channels channels,
  double power,
  long delay,
  RecursivePwmValues periods,
  RecursivePwmValues on_durations,
  long count)
{
  return addRecursivePwm(channels,
    power,
    delay,
    periods,
    on_durations,
    count,
    makeFunctor((Functor1<int> *)0,*this,&DigitalController::startPulseHandler),
    makeFunctor((Functor1<int> *)0,*this,&DigitalController::stopPulseHandler),
    dummy_functor_,
    dummy_functor_);
}

digital_controller::constants::PwmId DigitalController::addRecursivePwm(Channels channels,
  double power,
  long delay,
  RecursivePwmValues periods,
  RecursivePwmValues on_durations,
  long count,
  const Functor1<int> & start_pulse_functor,
  const Functor1<int> & stop_pulse_functor,
  const Functor1<int> & start_recursive_pwm_functor,
  const Functor1<int> & stop_recursive_pwm_functor)
{
  constants::PwmId pwm_id;
  if (pwm_info_container_.full() || (event_controller_.eventsAvailable() < 2))
  {
    pwm_id.index = constants::NO_PWM_AVAILABLE_INDEX;
    return pwm_id;
  }

  Level level_count = periods.size();
  if (on_durations.size() != level_count)
  {
    pwm_id.index = constants::PWM_ARRAY_LENGTHS_NOT_EQUAL_INDEX;
    return pwm_id;
  }

  if (level_count == 0)
  {
    pwm_id.index = constants::PWM_ARRAY_LENGTHS_ARE_ZERO_INDEX;
    return pwm_id;
  }

  pwm_id.index = constants::NO_CHILD_PWM_INDEX;

  constants::PwmInfo pwm_info;
  for (Level level=0; level < level_count; ++level)
  {
    pwm_info.channels = channels;
    pwm_info.running = false;
    pwm_info.level = level;
    pwm_info.child_index = pwm_id.index;
    pwm_info.power = power;
    pwm_info.period = periods[level];
    pwm_info.on_duration = on_durations[level];
    pwm_info.start_pulse_functor = dummy_functor_;
    pwm_info.stop_pulse_functor = dummy_functor_;
    pwm_info.start_pwm_functor = dummy_functor_;
    pwm_info.stop_pwm_functor = dummy_functor_;
    pwm_info.top_level = false;
    pwm_info.delay = 0;
    pwm_info.count = -1;
    if (level == 0)
    {
      pwm_info.start_pulse_functor = start_pulse_functor;
      pwm_info.stop_pulse_functor = stop_pulse_functor;
    }
    else if (level == (level_count - 1))
    {
      pwm_info.top_level = true;
      pwm_info.delay = delay;
      pwm_info.count = count;
      pwm_info.start_pwm_functor = start_recursive_pwm_functor;
      pwm_info.stop_pwm_functor = stop_recursive_pwm_functor;
    }
    pwm_info.stopped_before_count_completed = false;
    pwm_info.count_completed_functor = dummy_functor_;
    pwm_info.functor_arg = -1;
    pwm_id.index = pwm_info_container_.add(pwm_info);
  }

  if (pwm_id.index >= 0)
  {
    EventIdPair event_id_pair = event_controller_.addPwmUsingDelay(makeFunctor((Functor1<int> *)0,*this,&DigitalController::startRecursivePwmHandler),
      makeFunctor((Functor1<int> *)0,*this,&DigitalController::stopRecursivePwmHandler),
      delay,
      pwm_info.period,
      pwm_info.on_duration,
      count,
      pwm_id.index);
    event_controller_.addStartFunctor(event_id_pair,makeFunctor((Functor1<int> *)0,*this,&DigitalController::startPwmWrapperHandler));
    event_controller_.addStopFunctor(event_id_pair,makeFunctor((Functor1<int> *)0,*this,&DigitalController::stopPwmWrapperHandler));
    pwm_info_container_[pwm_id.index].event_id_pair = event_id_pair;
    event_controller_.enable(event_id_pair);
  }

  return pwm_id;
}

digital_controller::constants::PwmId DigitalController::startRecursivePwm(Channels channels,
  double power,
  long delay,
  RecursivePwmValues periods,
  RecursivePwmValues on_durations)
{
  return addRecursivePwm(channels,power,delay,periods,on_durations,-1);
}

void DigitalController::addCountCompletedFunctor(digital_controller::constants::PwmId pwm_id,
  const Functor1<int> & functor,
  int arg)
{
  if (!pwmIndexHasValue(pwm_id.index))
  {
    return;
  }
  constants::PwmInfo & pwm_info = pwm_info_container_[pwm_id.index];
  pwm_info.count_completed_functor = functor;
  pwm_info.functor_arg = arg;
}

void DigitalController::stopPwm(digital_controller::constants::PwmId pwm_id)
{
  if (!pwmIndexHasValue(pwm_id.index))
  {
    return;
  }
  constants::PwmInfo & pwm_info = pwm_info_container_[pwm_id.index];
  if  (pwm_id.event_id_pair != pwm_info.event_id_pair)
  {
    return;
  }
  if (pwm_info.child_index >= 0)
  {
    stopPwm(pwm_info.child_index);
  }
  pwm_info.stopped_before_count_completed = true;
  event_controller_.remove(pwm_info.event_id_pair);
}

void DigitalController::stopAllPwm()
{
  for (size_t i=0; i<constants::INDEXED_PWM_COUNT_MAX; ++i)
  {
    stopPwm(i);
  }
  pwm_info_container_.clear();
  event_controller_.clearAllEvents();
}

void DigitalController::addEventUsingDelay(const Functor1<int> & functor,
  Duration delay,
  int arg)
{
  if (event_controller_.eventsAvailable() == 0)
  {
    return;
  }
  EventId event_id = event_controller_.addEventUsingDelay(functor,delay,arg);
  event_controller_.enable(event_id);
}

bool DigitalController::pwmIndexHasValue(PwmIndex pwm_index)
{
  return pwm_info_container_.indexHasValue(pwm_index);
}

DigitalController::Channels DigitalController::getPwmChannels(PwmIndex pwm_index)
{
  Channels channels = 0;
  if (pwmIndexHasValue(pwm_index))
  {
    channels = pwm_info_container_[pwm_index].channels;
  }
  return channels;
}

double DigitalController::getPwmPower(PwmIndex pwm_index)
{
  double power = 0.0;
  if (pwmIndexHasValue(pwm_index))
  {
    power = pwm_info_container_[pwm_index].power;
  }
  return power;
}

double DigitalController::getPowerLowerBound(Channel channel)
{
  return constants::power_lower_bound_default;
}

double DigitalController::getPowerUpperBound(Channel channel)
{
  return constants::power_upper_bound_default;
}

double DigitalController::setChannelToPower(Channel channel,
  double power)
{
  return constants::power_min;
}

void DigitalController::startPulseHandler(PwmIndex pwm_index)
{
  Channels channels = getPwmChannels(pwm_index);
  double power = getPwmPower(pwm_index);
  setChannelsOnAtPower(channels,power);
}

void DigitalController::stopPulseHandler(PwmIndex pwm_index)
{
  Channels channels = getPwmChannels(pwm_index);
  setChannelsOff(channels);
}

void DigitalController::startPwmHandler(PwmIndex pwm_index)
{
}

void DigitalController::stopPwmHandler(PwmIndex pwm_index)
{
  Channels channels = getPwmChannels(pwm_index);
  setChannelsOff(channels);
}

DigitalController::ChannelsPwmIndexes DigitalController::getChannelsPwmIndexes()
{
  ChannelsPwmIndexes channels_pwm_indexes;
  noInterrupts();
  for (Channel channel=0; channel<getChannelCount(); ++channel)
  {
    RecursivePwmValues channel_pwm_indexes(channels_pwm_indexes_[channel]);
    channels_pwm_indexes.push_back(channel_pwm_indexes);
  }
  interrupts();
  return channels_pwm_indexes;
}

DigitalController::Channels DigitalController::jsonArrayToChannels(ArduinoJson::JsonArray channels_array)
{
  Channels channels = 0;
  Channels bit = 1;
  for (long channel : channels_array)
  {
    channels |= bit << channel;
  }
  return channels;
}

DigitalController::RecursivePwmValues DigitalController::jsonArrayToRecursivePwmValues(ArduinoJson::JsonArray array)
{
  RecursivePwmValues pwm_values;
  for (long value : array)
  {
    pwm_values.push_back(value);
  }
  return pwm_values;
}

void DigitalController::returnPwmIndexResponse(PwmIndex pwm_index)
{
  if (pwm_index >= 0)
  {
    modular_server_.response().returnResult(pwm_index);
  }
  else if (pwm_index == constants::NO_PWM_AVAILABLE_INDEX)
  {
    modular_server_.response().returnError(constants::no_pwm_available_error);
  }
  else if (pwm_index == constants::PWM_ARRAY_LENGTHS_NOT_EQUAL_INDEX)
  {
    modular_server_.response().returnError(constants::pwm_array_lengths_not_equal_error);
  }
  else if (pwm_index == constants::PWM_ARRAY_LENGTHS_ARE_ZERO_INDEX)
  {
    modular_server_.response().returnError(constants::pwm_array_lengths_are_zero_error);
  }
}

void DigitalController::stopPwm(PwmIndex pwm_index)
{
  if (!pwmIndexHasValue(pwm_index))
  {
    return;
  }
  constants::PwmInfo & pwm_info = pwm_info_container_[pwm_index];
  if (pwm_info.child_index >= 0)
  {
    stopPwm(pwm_info.child_index);
  }
  pwm_info.stopped_before_count_completed = true;
  event_controller_.remove(pwm_info.event_id_pair);
}

void DigitalController::removeParentAndChildrenPwmInfo(PwmIndex pwm_index)
{
  if (pwmIndexHasValue(pwm_index))
  {
    removeParentAndChildrenPwmInfo(pwm_info_container_[pwm_index].child_index);
    pwm_info_container_.remove(pwm_index);
  }
}

void DigitalController::updateChannel(Channel channel)
{
  Channels bit = 1;
  bit = bit << channel;
  noInterrupts();
  Channels channels = channels_;
  interrupts();
  if (bit & channels)
  {
    setChannelOn(channel);
  }
}

void DigitalController::updateAllChannels()
{
  noInterrupts();
  Channels channels = channels_;
  interrupts();
  setChannels(channels);
}

void DigitalController::initializePwmIndexes()
{
  noInterrupts();
  for (Channel channel=0; channel<getChannelCount(); ++channel)
  {
    for (Level level=0; level<constants::PWM_LEVEL_COUNT_MAX; ++level)
    {
      channels_pwm_indexes_[channel][level] = constants::PWM_NOT_RUNNING_INDEX;
    }
  }
  interrupts();
}

void DigitalController::setChannelPwmIndexesRunning(Channel channel,
  Level level,
  int pwm_index)
{
  if ((channel < getChannelCount()) && (level < constants::PWM_LEVEL_COUNT_MAX))
  {
    noInterrupts();
    channels_pwm_indexes_[channel][level] = pwm_index;
    interrupts();
  }
}

void DigitalController::setChannelsPwmIndexesRunning(Channels channels,
  Level level,
  PwmIndex pwm_index)
{
  if (level < constants::PWM_LEVEL_COUNT_MAX)
  {
    Channels bit = 1;
    noInterrupts();
    for (Channel channel=0; channel<getChannelCount(); ++channel)
    {
      if ((bit << channel) & channels)
      {
        channels_pwm_indexes_[channel][level] = pwm_index;
      }
    }
    interrupts();
  }
}

void DigitalController::setChannelPwmIndexesStopped(Channel channel,
  Level level)
{
  if ((channel < getChannelCount()) && (level < constants::PWM_LEVEL_COUNT_MAX))
  {
    noInterrupts();
    channels_pwm_indexes_[channel][level] = constants::PWM_NOT_RUNNING_INDEX;
    interrupts();
  }
}

void DigitalController::setChannelsPwmIndexesStopped(Channels channels,
  Level level)
{
  if (level < constants::PWM_LEVEL_COUNT_MAX)
  {
    Channels bit = 1;
    noInterrupts();
    for (Channel channel=0; channel<getChannelCount(); ++channel)
    {
      if ((bit << channel) & channels)
      {
        channels_pwm_indexes_[channel][level] = constants::PWM_NOT_RUNNING_INDEX;
      }
    }
    interrupts();
  }
}

void DigitalController::dummyHandler(int dummy_arg)
{}

void DigitalController::setChannelCountHandler()
{
  long channel_count = getChannelCount();
  long channel_max = channel_count - 1;

  modular_server::Property & power_max_property = modular_server_.property(constants::power_max_property_name);
  power_max_property.setArrayLengthRange(channel_count,channel_count);

  modular_server::Parameter & channel_parameter = modular_server_.parameter(constants::channel_parameter_name);
  channel_parameter.setRange(constants::channel_min,channel_max);

  modular_server::Parameter & channels_parameter = modular_server_.parameter(constants::channels_parameter_name);
  channels_parameter.setRange(constants::channel_min,channel_max);
  channels_parameter.setArrayLengthRange(constants::channel_count_min,channel_count);

  modular_server::Parameter & powers_parameter = modular_server_.parameter(constants::powers_parameter_name);
  powers_parameter.setArrayLengthRange(channel_count,channel_count);

  setAllPowersWhenOnToMax();
}

void DigitalController::setPowerMaxHandler(Channel channel)
{
  setPowerWhenOnToMax(channel);

  updateChannel(channel);
}

void DigitalController::allEnabledHandler()
{
  bool all_enabled = allEnabled();
  modular_server_.response().returnResult(all_enabled);
}

void DigitalController::setPowerWhenOnHandler()
{
  Channel channel;
  modular_server_.parameter(constants::channel_parameter_name).getValue(channel);
  Power power;
  modular_server_.parameter(constants::power_parameter_name).getValue(power);
  power = setPowerWhenOn(channel,power);
  modular_server_.response().returnResult(power);
}

void DigitalController::setPowersWhenOnHandler()
{
  ArduinoJson::JsonArray powers_array;
  modular_server_.parameter(constants::powers_parameter_name).getValue(powers_array);

  modular_server_.response().writeResultKey();
  modular_server_.response().beginArray();

  Channel channel = 0;
  for (double power : powers_array)
  {
    power = setPowerWhenOn(channel,power);
    modular_server_.response().write(power);
    ++channel;
  }

  modular_server_.response().endArray();
}

void DigitalController::setAllPowersWhenOnHandler()
{
  Power power_to_set;
  modular_server_.parameter(constants::power_parameter_name).getValue(power_to_set);

  modular_server_.response().writeResultKey();
  modular_server_.response().beginArray();

  for (Channel channel=0; channel<getChannelCount(); ++channel)
  {
    double power = setPowerWhenOn(channel,power_to_set);
    modular_server_.response().write(power);
  }

  modular_server_.response().endArray();
}

void DigitalController::setAllPowersWhenOnToMaxHandler()
{
  modular_server_.response().writeResultKey();
  modular_server_.response().beginArray();

  for (Channel channel=0; channel<getChannelCount(); ++channel)
  {
    double power = setPowerWhenOnToMax(channel);
    modular_server_.response().write(power);
  }

  modular_server_.response().endArray();
}

void DigitalController::getPowersWhenOnHandler()
{
  modular_server_.response().writeResultKey();
  modular_server_.response().beginArray();
  double power;
  for (Channel channel=0; channel<getChannelCount(); ++channel)
  {
    power = getPowerWhenOn(channel);
    modular_server_.response().write(power);
  }
  modular_server_.response().endArray();
}

void DigitalController::getPowersHandler()
{
  modular_server_.response().writeResultKey();
  modular_server_.response().beginArray();
  double power;
  for (Channel channel=0; channel<getChannelCount(); ++channel)
  {
    power = getPower(channel);
    modular_server_.response().write(power);
  }
  modular_server_.response().endArray();
}

void DigitalController::setChannelOnHandler()
{
  Channel channel;
  modular_server_.parameter(constants::channel_parameter_name).getValue(channel);
  setChannelOn(channel);
}

void DigitalController::setChannelOnAtPowerHandler()
{
  Channel channel;
  modular_server_.parameter(constants::channel_parameter_name).getValue(channel);
  Power power;
  modular_server_.parameter(constants::power_parameter_name).getValue(power);
  setChannelOnAtPower(channel,power);
}

void DigitalController::setChannelOffHandler()
{
  Channel channel;
  modular_server_.parameter(constants::channel_parameter_name).getValue(channel);
  setChannelOff(channel);
}

void DigitalController::setChannelsOnHandler()
{
  ArduinoJson::JsonArray channels_array;
  modular_server_.parameter(constants::channels_parameter_name).getValue(channels_array);
  const Channels channels = jsonArrayToChannels(channels_array);
  setChannelsOn(channels);
}

void DigitalController::setChannelsOnAtPowerHandler()
{
  ArduinoJson::JsonArray channels_array;
  modular_server_.parameter(constants::channels_parameter_name).getValue(channels_array);
  const Channels channels = jsonArrayToChannels(channels_array);
  Power power;
  modular_server_.parameter(constants::power_parameter_name).getValue(power);
  setChannelsOnAtPower(channels,power);
}

void DigitalController::setChannelsOffHandler()
{
  ArduinoJson::JsonArray channels_array;
  modular_server_.parameter(constants::channels_parameter_name).getValue(channels_array);
  const Channels channels = jsonArrayToChannels(channels_array);
  setChannelsOff(channels);
}

void DigitalController::toggleChannelHandler()
{
  Channel channel;
  modular_server_.parameter(constants::channel_parameter_name).getValue(channel);
  toggleChannel(channel);
}

void DigitalController::toggleChannelsHandler()
{
  ArduinoJson::JsonArray channels_array;
  modular_server_.parameter(constants::channels_parameter_name).getValue(channels_array);
  const Channels channels = jsonArrayToChannels(channels_array);
  toggleChannels(channels);
}

void DigitalController::setAllChannelsOnAtPowerHandler()
{
  Power power;
  modular_server_.parameter(constants::power_parameter_name).getValue(power);

  setAllChannelsOnAtPower(power);
}

void DigitalController::setChannelOnAllOthersOffHandler()
{
  Channel channel;
  modular_server_.parameter(constants::channel_parameter_name).getValue(channel);
  setChannelOnAllOthersOff(channel);
}

void DigitalController::setChannelOffAllOthersOnHandler()
{
  Channel channel;
  modular_server_.parameter(constants::channel_parameter_name).getValue(channel);
  setChannelOffAllOthersOn(channel);
}

void DigitalController::setChannelsOnAllOthersOffHandler()
{
  ArduinoJson::JsonArray channels_array;
  modular_server_.parameter(constants::channels_parameter_name).getValue(channels_array);
  const Channels channels = jsonArrayToChannels(channels_array);
  setChannelsOnAllOthersOff(channels);
}

void DigitalController::setChannelsOffAllOthersOnHandler()
{
  ArduinoJson::JsonArray channels_array;
  modular_server_.parameter(constants::channels_parameter_name).getValue(channels_array);
  const Channels channels = jsonArrayToChannels(channels_array);
  setChannelsOffAllOthersOn(channels);
}

void DigitalController::channelIsOnHandler()
{
  Channel channel;
  modular_server_.parameter(constants::channel_parameter_name).getValue(channel);
  bool channel_is_on = channelIsOn(channel);
  modular_server_.response().returnResult(channel_is_on);
}

void DigitalController::getChannelsOnHandler()
{
  Channels channels_on = getChannelsOn();
  Channels bit = 1;
  modular_server_.response().writeResultKey();
  modular_server_.response().beginArray();
  for (Channel channel=0; channel<getChannelCount(); ++channel)
  {
    if (channels_on & (bit << channel))
    {
      modular_server_.response().write(channel);
    }
  }
  modular_server_.response().endArray();
}

void DigitalController::getChannelsOffHandler()
{
  Channels channels_on = getChannelsOn();
  Channels channels_off = ~channels_on;
  Channels bit = 1;
  modular_server_.response().writeResultKey();
  modular_server_.response().beginArray();
  for (Channel channel=0; channel<getChannelCount(); ++channel)
  {
    if (channels_off & (bit << channel))
    {
      modular_server_.response().write(channel);
    }
  }
  modular_server_.response().endArray();
}

void DigitalController::getChannelCountHandler()
{
  Channel channel_count = getChannelCount();
  modular_server_.response().returnResult(channel_count);
}

void DigitalController::addPwmHandler()
{
  ArduinoJson::JsonArray channels_array;
  modular_server_.parameter(constants::channels_parameter_name).getValue(channels_array);
  double power;
  modular_server_.parameter(constants::power_parameter_name).getValue(power);
  long delay;
  modular_server_.parameter(constants::delay_parameter_name).getValue(delay);
  long period;
  modular_server_.parameter(constants::period_parameter_name).getValue(period);
  long on_duration;
  modular_server_.parameter(constants::on_duration_parameter_name).getValue(on_duration);
  long count;
  modular_server_.parameter(constants::count_parameter_name).getValue(count);
  const Channels channels = jsonArrayToChannels(channels_array);
  constants::PwmId pwm_id = addPwm(channels,power,delay,period,on_duration,count);
  returnPwmIndexResponse(pwm_id.index);
}

void DigitalController::startPwmHandler()
{
  ArduinoJson::JsonArray channels_array;
  modular_server_.parameter(constants::channels_parameter_name).getValue(channels_array);
  double power;
  modular_server_.parameter(constants::power_parameter_name).getValue(power);
  long delay;
  modular_server_.parameter(constants::delay_parameter_name).getValue(delay);
  long period;
  modular_server_.parameter(constants::period_parameter_name).getValue(period);
  long on_duration;
  modular_server_.parameter(constants::on_duration_parameter_name).getValue(on_duration);
  const Channels channels = jsonArrayToChannels(channels_array);
  constants::PwmId pwm_id = startPwm(channels,power,delay,period,on_duration);
  returnPwmIndexResponse(pwm_id.index);
}

void DigitalController::addRecursivePwmHandler()
{
  ArduinoJson::JsonArray channels_array;
  modular_server_.parameter(constants::channels_parameter_name).getValue(channels_array);
  double power;
  modular_server_.parameter(constants::power_parameter_name).getValue(power);
  long delay;
  modular_server_.parameter(constants::delay_parameter_name).getValue(delay);
  ArduinoJson::JsonArray periods_array;
  modular_server_.parameter(constants::periods_parameter_name).getValue(periods_array);
  ArduinoJson::JsonArray on_durations_array;
  modular_server_.parameter(constants::on_durations_parameter_name).getValue(on_durations_array);
  long count;
  modular_server_.parameter(constants::count_parameter_name).getValue(count);
  const Channels channels = jsonArrayToChannels(channels_array);
  RecursivePwmValues periods = jsonArrayToRecursivePwmValues(periods_array);
  RecursivePwmValues on_durations = jsonArrayToRecursivePwmValues(on_durations_array);
  constants::PwmId pwm_id = addRecursivePwm(channels,power,delay,periods,on_durations,count);
  returnPwmIndexResponse(pwm_id.index);
}

void DigitalController::startRecursivePwmHandler()
{
  ArduinoJson::JsonArray channels_array;
  modular_server_.parameter(constants::channels_parameter_name).getValue(channels_array);
  double power;
  modular_server_.parameter(constants::power_parameter_name).getValue(power);
  long delay;
  modular_server_.parameter(constants::delay_parameter_name).getValue(delay);
  ArduinoJson::JsonArray periods_array;
  modular_server_.parameter(constants::periods_parameter_name).getValue(periods_array);
  ArduinoJson::JsonArray on_durations_array;
  modular_server_.parameter(constants::on_durations_parameter_name).getValue(on_durations_array);
  const Channels channels = jsonArrayToChannels(channels_array);
  RecursivePwmValues periods = jsonArrayToRecursivePwmValues(periods_array);
  RecursivePwmValues on_durations = jsonArrayToRecursivePwmValues(on_durations_array);
  constants::PwmId pwm_id = startRecursivePwm(channels,power,delay,periods,on_durations);
  returnPwmIndexResponse(pwm_id.index);
}

void DigitalController::stopPwmHandler()
{
  PwmIndex pwm_index;
  modular_server_.parameter(constants::pwm_index_parameter_name).getValue(pwm_index);
  stopPwm(pwm_index);
}

void DigitalController::stopAllPwmHandler()
{
  stopAllPwm();
}

void DigitalController::getChannelsPwmIndexesHandler()
{
  modular_server_.response().writeResultKey();
  modular_server_.response().beginArray();

  ChannelsPwmIndexes channels_pwm_indexes = getChannelsPwmIndexes();
  for (Channel channel=0; channel<getChannelCount(); ++channel)
  {
    modular_server_.response().beginArray();

    for (Level level=0; level<constants::PWM_LEVEL_COUNT_MAX; ++level)
    {
      modular_server_.response().write(channels_pwm_indexes[channel][level]);
    }

    modular_server_.response().endArray();
  }

  modular_server_.response().endArray();

}

void DigitalController::getPwmInfoHandler()
{
  noInterrupts();
  IndexedContainer<constants::PwmInfo,
    constants::INDEXED_PWM_COUNT_MAX> indexed_pwm = pwm_info_container_;
  interrupts();

  Channels bit = 1;

  modular_server_.response().writeResultKey();
  modular_server_.response().beginArray();

  for (size_t i=0; i<constants::INDEXED_PWM_COUNT_MAX; ++i)
  {
    if (indexed_pwm.indexHasValue(i))
    {
      modular_server_.response().beginObject();
      constants::PwmInfo & pwm_info = indexed_pwm[i];
      modular_server_.response().write(constants::pwm_index_parameter_name,i);
      modular_server_.response().writeKey(constants::channels_parameter_name);
      modular_server_.response().beginArray();
      for (Channel channel=0; channel<getChannelCount(); ++channel)
      {
        if ((bit << channel) & pwm_info.channels)
        {
          modular_server_.response().write(channel);
        }
      }
      modular_server_.response().endArray();
      modular_server_.response().write(constants::running_string,pwm_info.running);
      modular_server_.response().write(constants::level_string,pwm_info.level);
      modular_server_.response().write(constants::top_level_string,pwm_info.top_level);
      modular_server_.response().write(constants::child_index_string,pwm_info.child_index);
      modular_server_.response().write(constants::power_parameter_name,pwm_info.power);
      modular_server_.response().write(constants::delay_parameter_name,pwm_info.delay);
      modular_server_.response().write(constants::period_parameter_name,pwm_info.period);
      modular_server_.response().write(constants::on_duration_parameter_name,pwm_info.on_duration);
      modular_server_.response().write(constants::count_parameter_name,pwm_info.count);
      modular_server_.response().endObject();
    }
  }

  modular_server_.response().endArray();

}

void DigitalController::getPowerBoundsHandler()
{
  modular_server_.response().writeResultKey();
  modular_server_.response().beginArray();

  for (Channel channel=0; channel<getChannelCount(); ++channel)
  {
    modular_server_.response().beginObject();

    double power_lower_bound = getPowerLowerBound(channel);
    modular_server_.response().write(constants::power_lower_bound_string,power_lower_bound);

    double power_upper_bound = getPowerUpperBound(channel);
    modular_server_.response().write(constants::power_upper_bound_string,power_upper_bound);

    modular_server_.response().endObject();
  }

  modular_server_.response().endArray();

}

void DigitalController::enableAllHandler(modular_server::Pin * pin_ptr)
{
  enableAll();
}

void DigitalController::disableAllHandler(modular_server::Pin * pin_ptr)
{
  disableAll();
}

void DigitalController::toggleAllChannelsHandler(modular_server::Pin * pin_ptr)
{
  toggleAllChannels();
}

void DigitalController::setAllChannelsOnHandler(modular_server::Pin * pin_ptr)
{
  setAllChannelsOn();
}

void DigitalController::setAllChannelsOffHandler(modular_server::Pin * pin_ptr)
{
  setAllChannelsOff();
}

void DigitalController::startPulseWrapperHandler(PwmIndex pwm_index)
{
  if (!pwmIndexHasValue(pwm_index))
  {
    return;
  }

  pwm_info_container_[pwm_index].start_pulse_functor(pwm_index);
}

void DigitalController::stopPulseWrapperHandler(PwmIndex pwm_index)
{
  if (!pwmIndexHasValue(pwm_index))
  {
    return;
  }

  pwm_info_container_[pwm_index].stop_pulse_functor(pwm_index);
}

void DigitalController::startPwmWrapperHandler(PwmIndex pwm_index)
{
  if (!pwmIndexHasValue(pwm_index))
  {
    return;
  }
  Channels & channels = pwm_info_container_[pwm_index].channels;
  uint8_t & level = pwm_info_container_[pwm_index].level;

  setChannelsPwmIndexesRunning(channels,level,pwm_index);
  pwm_info_container_[pwm_index].running = true;

  pwm_info_container_[pwm_index].start_pwm_functor(pwm_index);
}

void DigitalController::stopPwmWrapperHandler(PwmIndex pwm_index)
{
  if (!pwmIndexHasValue(pwm_index))
  {
    return;
  }
  constants::PwmInfo pwm_info = pwm_info_container_[pwm_index];
  Channels & channels = pwm_info.channels;
  uint8_t & level = pwm_info.level;
  bool stopped_before_count_completed = pwm_info.stopped_before_count_completed;
  Functor1<int> count_completed_functor = pwm_info.count_completed_functor;
  int functor_arg = pwm_info.functor_arg;
  pwm_info_container_[pwm_index].stop_pwm_functor(pwm_index);
  setChannelsPwmIndexesStopped(channels,level);
  pwm_info_container_[pwm_index].running = false;
  if (pwm_info.top_level)
  {
    removeParentAndChildrenPwmInfo(pwm_index);
  }
  if (!stopped_before_count_completed && count_completed_functor)
  {
    count_completed_functor(functor_arg);
  }
}

void DigitalController::startRecursivePwmHandler(PwmIndex pwm_index)
{
  if (!pwmIndexHasValue(pwm_index))
  {
    return;
  }
  constants::PwmInfo pwm_info = pwm_info_container_[pwm_index];
  PwmIndex child_index = pwm_info.child_index;
  if (pwm_info.level == 0)
  {
    startPulseWrapperHandler(pwm_index);
  }
  else
  {
    long delay = pwm_info_container_[child_index].delay;
    long period = pwm_info_container_[child_index].period;
    long on_duration = pwm_info_container_[child_index].on_duration;
    EventIdPair event_id_pair = event_controller_.addInfinitePwmUsingDelay(makeFunctor((Functor1<int> *)0,*this,&DigitalController::startRecursivePwmHandler),
      makeFunctor((Functor1<int> *)0,*this,&DigitalController::stopRecursivePwmHandler),
      delay,
      period,
      on_duration,
      child_index);
    event_controller_.addStartFunctor(event_id_pair,makeFunctor((Functor1<int> *)0,*this,&DigitalController::startPwmWrapperHandler));
    event_controller_.addStopFunctor(event_id_pair,makeFunctor((Functor1<int> *)0,*this,&DigitalController::stopPwmWrapperHandler));
    pwm_info_container_[child_index].event_id_pair = event_id_pair;
    event_controller_.enable(event_id_pair);
  }
}

void DigitalController::stopRecursivePwmHandler(PwmIndex pwm_index)
{
  if (!pwmIndexHasValue(pwm_index))
  {
    return;
  }
  constants::PwmInfo pwm_info = pwm_info_container_[pwm_index];
  if (pwm_info.level == 0)
  {
    stopPulseWrapperHandler(pwm_index);
  }
  else
  {
    PwmIndex child_index = pwm_info.child_index;
    if (child_index >= 0)
    {
      event_controller_.remove(pwm_info_container_[child_index].event_id_pair);
      stopRecursivePwmHandler(child_index);
    }
  }
}
