// ----------------------------------------------------------------------------
// DigitalController.h
//
//
// Authors:
// Peter Polidoro peter@polidoro.io
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
  typedef uint32_t Channels;
  typedef size_t Channel;
  typedef uint32_t Duration;
  typedef size_t Level;
  typedef int PwmIndex;
  typedef size_t Power;

  virtual void setup();

  void enableAll();
  void disableAll();
  bool allEnabled();

  double setPowerWhenOn(Channel channel,
    double power);
  double setPowerWhenOnToMax(Channel channel);
  void setAllPowersWhenOnToMax();
  double getPowerWhenOn(Channel channel);
  double getPower(Channel channel);

  void setChannels(Channels channels);
  void setChannelOn(Channel channel);
  void setChannelOnAtPower(Channel channel,
    double power);
  void setChannelOff(Channel channel);
  void setChannelsOn(Channels channels);
  void setChannelsOnAtPower(Channels channels,
    double power);
  void setChannelsOff(Channels channels);
  void toggleChannel(Channel channel);
  void toggleChannels(Channels channels);
  void toggleAllChannels();
  void setAllChannelsOn();
  void setAllChannelsOnAtPower(double power);
  void setAllChannelsOff();
  void setChannelOnAllOthersOff(Channel channel);
  void setChannelOffAllOthersOn(Channel channel);
  void setChannelsOnAllOthersOff(Channels channels);
  void setChannelsOffAllOthersOn(Channels channels);
  bool channelIsOn(Channel channel);
  Channels getChannelsOn();
  Channel getChannelCount();

  digital_controller::constants::PwmId addPwm(Channels channels,
    double power,
    long delay,
    long period,
    long on_duration,
    long count);
  digital_controller::constants::PwmId addPwm(Channels channels,
    double power,
    long delay,
    long period,
    long on_duration,
    long count,
    const Functor1<int> & start_pulse_functor,
    const Functor1<int> & stop_pulse_functor,
    const Functor1<int> & start_pwm_functor,
    const Functor1<int> & stop_pwm_functor);

  digital_controller::constants::PwmId startPwm(Channels channels,
    double power,
    long delay,
    long period,
    long on_duration);

  typedef Array<long,digital_controller::constants::PWM_LEVEL_COUNT_MAX> RecursivePwmValues;

  digital_controller::constants::PwmId addRecursivePwm(Channels channels,
    double power,
    long delay,
    RecursivePwmValues periods,
    RecursivePwmValues on_durations,
    long count);

  digital_controller::constants::PwmId addRecursivePwm(Channels channels,
    double power,
    long delay,
    RecursivePwmValues periods,
    RecursivePwmValues on_durations,
    long count,
    const Functor1<int> & start_pulse_functor,
    const Functor1<int> & stop_pulse_functor,
    const Functor1<int> & start_recursive_pwm_functor,
    const Functor1<int> & stop_recursive_pwm_functor);

  digital_controller::constants::PwmId startRecursivePwm(Channels channels,
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
    Duration delay,
    int arg=-1);

  bool pwmIndexHasValue(PwmIndex pwm_index);
  Channels getPwmChannels(PwmIndex pwm_index);
  double getPwmPower(PwmIndex pwm_index);

  virtual double getPowerLowerBound(Channel channel);
  virtual double getPowerUpperBound(Channel channel);
  virtual double setChannelToPower(Channel channel,
    double power);

  // Handlers
  virtual void startPulseHandler(PwmIndex pwm_index);
  virtual void stopPulseHandler(PwmIndex pwm_index);
  virtual void startPwmHandler(PwmIndex pwm_index);
  virtual void stopPwmHandler(PwmIndex pwm_index);

protected:
  EventController<digital_controller::constants::EVENT_COUNT_MAX> event_controller_;

  Functor1<int> dummy_functor_;

  typedef Array<RecursivePwmValues,digital_controller::constants::CHANNEL_COUNT_MAX> ChannelsPwmIndexes;
  ChannelsPwmIndexes getChannelsPwmIndexes();

  Channels jsonArrayToChannels(ArduinoJson::JsonArray channels_array);
  RecursivePwmValues jsonArrayToRecursivePwmValues(ArduinoJson::JsonArray array);

  void returnPwmIndexResponse(PwmIndex pwm_index);

  // Handlers
  void setChannelCountHandler();

private:
  modular_server::Property properties_[digital_controller::constants::PROPERTY_COUNT_MAX];
  modular_server::Parameter parameters_[digital_controller::constants::PARAMETER_COUNT_MAX];
  modular_server::Function functions_[digital_controller::constants::FUNCTION_COUNT_MAX];
  modular_server::Callback callbacks_[digital_controller::constants::CALLBACK_COUNT_MAX];

  bool all_enabled_;
  Channels channels_;
  double powers_when_on_[digital_controller::constants::CHANNEL_COUNT_MAX];
  double powers_[digital_controller::constants::CHANNEL_COUNT_MAX];
  long channels_pwm_indexes_[digital_controller::constants::CHANNEL_COUNT_MAX][digital_controller::constants::PWM_LEVEL_COUNT_MAX];

  IndexedContainer<digital_controller::constants::PwmInfo,
    digital_controller::constants::INDEXED_PWM_COUNT_MAX> pwm_info_container_;

  void stopPwm(PwmIndex pwm_index);

  void removeParentAndChildrenPwmInfo(PwmIndex pwm_index);

  void updateChannel(Channel channel);
  void updateAllChannels();

  void initializePwmIndexes();
  void setChannelPwmIndexesRunning(Channel channel,
    Level level,
    PwmIndex pwm_index);
  void setChannelsPwmIndexesRunning(Channels channels,
    Level level,
    PwmIndex pwm_index);
  void setChannelPwmIndexesStopped(Channel channel,
    Level level);
  void setChannelsPwmIndexesStopped(Channels channels,
    Level level);

  // Handlers
  void dummyHandler(int dummy_arg);
  void setPowerMaxHandler(Channel channel);

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

  void startPulseWrapperHandler(PwmIndex pwm_index);
  void stopPulseWrapperHandler(PwmIndex pwm_index);
  void startPwmWrapperHandler(PwmIndex pwm_index);
  void stopPwmWrapperHandler(PwmIndex pwm_index);

  void startRecursivePwmHandler(PwmIndex pwm_index);
  void stopRecursivePwmHandler(PwmIndex pwm_index);
};

#endif
