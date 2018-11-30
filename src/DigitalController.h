// ----------------------------------------------------------------------------
// DigitalController.h
//
//
// Authors:
// Peter Polidoro peterpolidoro@gmail.com
// ----------------------------------------------------------------------------
#ifndef DIGITAL_CONTROLLER_H
#define DIGITAL_CONTROLLER_H
#include <ArduinoJson.h>
#include <JsonStream.h>
#include <Array.h>
#include <Vector.h>
#include <ConstantVariable.h>
#include <Functor.h>

#include <IndexedContainer.h>
#include <EventController.h>

#include <ModularServer.h>
#include <ModularDeviceBase.h>

#include "DigitalController/Constants.h"


class DigitalController : public ModularDeviceBase
{
public:
  virtual void setup();

  void enableAll();
  void disableAll();
  bool allEnabled();

  double setPowerWhenOn(size_t channel,
    double power);
  double setPowerWhenOnToMax(size_t channel);
  void setAllPowersWhenOnToMax();
  double getPowerWhenOn(size_t channel);
  double getPower(size_t channel);

  void setChannels(uint32_t channels);
  void setChannelOn(size_t channel);
  void setChannelOnAtPower(size_t channel,
    double power);
  void setChannelOff(size_t channel);
  void setChannelsOn(uint32_t channels);
  void setChannelsOnAtPower(uint32_t channels,
    double power);
  void setChannelsOff(uint32_t channels);
  void toggleChannel(size_t channel);
  void toggleChannels(uint32_t channels);
  void toggleAllChannels();
  void setAllChannelsOn();
  void setAllChannelsOnAtPower(double power);
  void setAllChannelsOff();
  void setChannelOnAllOthersOff(size_t channel);
  void setChannelOffAllOthersOn(size_t channel);
  void setChannelsOnAllOthersOff(uint32_t channels);
  void setChannelsOffAllOthersOn(uint32_t channels);
  bool channelIsOn(size_t channel);
  uint32_t getChannelsOn();
  size_t getChannelCount();

  digital_controller::constants::PwmId addPwm(uint32_t channels,
    double power,
    long delay,
    long period,
    long on_duration,
    long count);
  digital_controller::constants::PwmId addPwm(uint32_t channels,
    double power,
    long delay,
    long period,
    long on_duration,
    long count,
    const Functor1<int> & start_pulse_functor,
    const Functor1<int> & stop_pulse_functor,
    const Functor1<int> & start_pwm_functor,
    const Functor1<int> & stop_pwm_functor);

  digital_controller::constants::PwmId startPwm(uint32_t channels,
    double power,
    long delay,
    long period,
    long on_duration);

  typedef Array<long,digital_controller::constants::PWM_LEVEL_COUNT_MAX> RecursivePwmValues;

  digital_controller::constants::PwmId addRecursivePwm(uint32_t channels,
    double power,
    long delay,
    RecursivePwmValues periods,
    RecursivePwmValues on_durations,
    long count);

  digital_controller::constants::PwmId addRecursivePwm(uint32_t channels,
    double power,
    long delay,
    RecursivePwmValues periods,
    RecursivePwmValues on_durations,
    long count,
    const Functor1<int> & start_pulse_functor,
    const Functor1<int> & stop_pulse_functor,
    const Functor1<int> & start_recursive_pwm_functor,
    const Functor1<int> & stop_recursive_pwm_functor);

  digital_controller::constants::PwmId startRecursivePwm(uint32_t channels,
    double power,
    long delay,
    RecursivePwmValues periods,
    RecursivePwmValues on_durations);

  void addCountCompletedFunctor(digital_controller::constants::PwmId pwm_id,
    const Functor1<int> & functor,
    int arg=-1);

  void stopPwm(digital_controller::constants::PwmId pwm_id);
  void stopAllPwm();

  void addEventUsingDelay(const Functor1<int> & functor,
    uint32_t delay,
    int arg=-1);

protected:
  EventController<digital_controller::constants::EVENT_COUNT_MAX> event_controller_;

  Functor1<int> dummy_functor_;

  virtual double getPowerLowerBound(size_t channel);
  virtual double getPowerUpperBound(size_t channel);
  virtual double setChannelToPower(size_t channel,
    double power);

  typedef Array<RecursivePwmValues,digital_controller::constants::CHANNEL_COUNT_MAX> ChannelsPwmIndexes;
  ChannelsPwmIndexes getChannelsPwmIndexes();

  uint32_t jsonArrayToChannels(ArduinoJson::JsonArray & channels_array);
  RecursivePwmValues jsonArrayToRecursivePwmValues(ArduinoJson::JsonArray & array);

  void returnPwmIndexResponse(int pwm_index);

  // Handlers
  void setChannelCountHandler();

private:
  modular_server::Property properties_[digital_controller::constants::PROPERTY_COUNT_MAX];
  modular_server::Parameter parameters_[digital_controller::constants::PARAMETER_COUNT_MAX];
  modular_server::Function functions_[digital_controller::constants::FUNCTION_COUNT_MAX];
  modular_server::Callback callbacks_[digital_controller::constants::CALLBACK_COUNT_MAX];

  bool all_enabled_;
  uint32_t channels_;
  double powers_when_on_[digital_controller::constants::CHANNEL_COUNT_MAX];
  double powers_[digital_controller::constants::CHANNEL_COUNT_MAX];
  long channels_pwm_indexes_[digital_controller::constants::CHANNEL_COUNT_MAX][digital_controller::constants::PWM_LEVEL_COUNT_MAX];

  IndexedContainer<digital_controller::constants::PwmInfo,
    digital_controller::constants::INDEXED_PWM_COUNT_MAX> pwm_info_container_;

  void stopPwm(int pwm_index);

  void removeParentAndChildrenPwmInfo(int pwm_index);

  void updateChannel(size_t channel);
  void updateAllChannels();

  void initializePwmIndexes();
  void setChannelPwmIndexesRunning(size_t channel,
    size_t level,
    int pwm_index);
  void setChannelsPwmIndexesRunning(uint32_t channels,
    size_t level,
    int pwm_index);
  void setChannelPwmIndexesStopped(size_t channel,
    size_t level);
  void setChannelsPwmIndexesStopped(uint32_t channels,
    size_t level);

  // Handlers
  void dummyHandler(int dummy_arg);
  void setPowerMaxHandler(size_t channel);

  void allEnabledHandler();
  void setPowerWhenOnHandler();
  void setPowersWhenOnHandler();
  void setAllPowersWhenOnHandler();
  void setAllPowersWhenOnToMaxHandler();
  void getPowersWhenOnHandler();
  void getPowersHandler();
  void setChannelOnHandler();
  void setChannelOnAtPowerHandler();
  void setChannelOffHandler();
  void setChannelsOnHandler();
  void setChannelsOnAtPowerHandler();
  void setChannelsOffHandler();
  void toggleChannelHandler();
  void toggleChannelsHandler();
  void setAllChannelsOnAtPowerHandler();
  void setChannelOnAllOthersOffHandler();
  void setChannelOffAllOthersOnHandler();
  void setChannelsOnAllOthersOffHandler();
  void setChannelsOffAllOthersOnHandler();
  void channelIsOnHandler();
  void getChannelsOnHandler();
  void getChannelsOffHandler();
  void getChannelCountHandler();
  void addPwmHandler();
  void startPwmHandler();
  void addRecursivePwmHandler();
  void startRecursivePwmHandler();
  void stopPwmHandler();
  void stopAllPwmHandler();
  void getChannelsPwmIndexesHandler();
  void getPwmInfoHandler();
  void getPowerBoundsHandler();

  void enableAllHandler(modular_server::Pin * pin_ptr);
  void disableAllHandler(modular_server::Pin * pin_ptr);
  void toggleAllChannelsHandler(modular_server::Pin * pin_ptr);
  void setAllChannelsOnHandler(modular_server::Pin * pin_ptr);
  void setAllChannelsOffHandler(modular_server::Pin * pin_ptr);

  void startPulseHandler(int pwm_index);
  void stopPulseHandler(int pwm_index);
  void startPwmHandler(int pwm_index);
  void stopPwmHandler(int pwm_index);

  void startPulseWrapperHandler(int pwm_index);
  void stopPulseWrapperHandler(int pwm_index);
  void startPwmWrapperHandler(int pwm_index);
  void stopPwmWrapperHandler(int pwm_index);

  void startRecursivePwmHandler(int pwm_index);
  void stopRecursivePwmHandler(int pwm_index);
};

#endif
