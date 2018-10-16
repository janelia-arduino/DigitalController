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
  DigitalController();
  ~DigitalController();
  virtual void setup();

  void enableAll();
  void disableAll();
  bool enabled();

  long setPowerWhenOn(size_t channel,
    long power);
  long getPowerWhenOn(size_t channel);
  long getPower(size_t channel);

  void setChannels(uint32_t channels);
  void setChannelOn(size_t channel);
  void setChannelOnAtPower(size_t channel,
    long power);
  void setChannelOff(size_t channel);
  void setChannelsOn(uint32_t channels);
  void setChannelsOnAtPower(uint32_t channels,
    long power);
  void setChannelsOff(uint32_t channels);
  void toggleChannel(size_t channel);
  void toggleChannels(uint32_t channels);
  void toggleAllChannels();
  void setAllChannelsOn();
  void setAllChannelsOff();
  void setChannelOnAllOthersOff(size_t channel);
  void setChannelOffAllOthersOn(size_t channel);
  void setChannelsOnAllOthersOff(uint32_t channels);
  void setChannelsOffAllOthersOn(uint32_t channels);
  bool channelIsOn(size_t channel);
  uint32_t getChannelsOn();
  size_t getChannelCount();

  int addPwm(uint32_t channels,
    long delay,
    long period,
    long on_duration,
    long count);
  int startPwm(uint32_t channels,
    long delay,
    long period,
    long on_duration);

  typedef Array<long,digital_controller::constants::PWM_LEVEL_COUNT_MAX> RecursivePwmValues;

  int addRecursivePwm(uint32_t channels,
    long delay,
    RecursivePwmValues periods,
    RecursivePwmValues on_durations,
    long count);
  int startRecursivePwm(uint32_t channels,
    long delay,
    RecursivePwmValues periods,
    RecursivePwmValues on_durations);

  void addCountCompletedFunctor(int pwm_index,
    const Functor1<int> & functor,
    int arg=-1);

  void stopPwm(int pwm_index);
  void stopAllPwm();

  void addEventUsingDelay(const Functor1<int> & functor,
    uint32_t delay,
    int arg=-1);

  typedef Array<RecursivePwmValues,digital_controller::constants::CHANNEL_COUNT_MAX> ChannelsPwmIndexes;
  ChannelsPwmIndexes getChannelsPwmIndexes();

  uint32_t arrayToChannels(ArduinoJson::JsonArray & channels_array);
  RecursivePwmValues arrayToRecursivePwmValues(ArduinoJson::JsonArray & array);

  // Handlers
  virtual void startPwmHandler(int pwm_index);
  virtual void stopPwmHandler(int pwm_index);

protected:

  // Handlers
  void setChannelCountHandler();

private:
  modular_server::Property properties_[digital_controller::constants::PROPERTY_COUNT_MAX];
  modular_server::Parameter parameters_[digital_controller::constants::PARAMETER_COUNT_MAX];
  modular_server::Function functions_[digital_controller::constants::FUNCTION_COUNT_MAX];
  modular_server::Callback callbacks_[digital_controller::constants::CALLBACK_COUNT_MAX];

  bool enabled_;
  uint32_t channels_;
  long powers_when_on_[digital_controller::constants::CHANNEL_COUNT_MAX];
  long powers_[digital_controller::constants::CHANNEL_COUNT_MAX];
  long channels_pwm_indexes_[digital_controller::constants::CHANNEL_COUNT_MAX][digital_controller::constants::PWM_LEVEL_COUNT_MAX];

  EventController<digital_controller::constants::EVENT_COUNT_MAX> event_controller_;

  IndexedContainer<digital_controller::constants::PwmInfo,
    digital_controller::constants::INDEXED_PWM_COUNT_MAX> indexed_pwm_;

  const Functor1<int> functor_dummy_;

  void removeParentAndChildrenPwmInfo(int pwm_index);

  long powerToAnalogWriteValue(long power);
  void setPowersToMax();

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

  void returnPwmIndexResponse(int pwm_index);

  // Handlers
  void setPowerMaxHandler(size_t channel);
  void enableAllHandler();
  void disableAllHandler();
  void enabledHandler();
  void setPowerWhenOnHandler();
  void setPowersWhenOnHandler();
  void setAllPowersWhenOnHandler();
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
  void toggleAllChannelsHandler();
  void setAllChannelsOnHandler();
  void setAllChannelsOffHandler();
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
  void setChannelsOnHandler(int pwm_index);
  void setChannelsOffHandler(int pwm_index);
  void startRecursivePwmHandler(int pwm_index);
  void stopRecursivePwmHandler(int pwm_index);

};

#endif