#include "DoorPhoneMonitor.h"
#include "Utils.h"

#include <cstdio>
#include <sstream>

namespace RingingSensor
{
    constexpr unsigned long CheckInvervalMs = 500;
    constexpr unsigned long ActiveStateTimeoutMs = 3000;
    constexpr auto CountThreshold = 100u;
}

namespace Pins
{
    constexpr auto Led = LED_BUILTIN;
    constexpr auto RingingSense = D2;
    constexpr auto RingerMute = D1;
}

DoorPhoneMonitor::DoorPhoneMonitor(const ApplicationConfig& appConfig)
    : _appConfig(appConfig)
    , _coreApplication(_appConfig)
    , _mqtt(_coreApplication.mqttClient())
{
    Logger::setup(_appConfig, _coreApplication.systemClock());

    setupMqtt();
    _coreApplication.setMqttUpdateHandler([this]{ updateMqtt(); });

    // Setup ringing sensor input pin
    pinMode(Pins::RingingSense, INPUT);
    attachInterruptArg(Pins::RingingSense, ringingSenseIsr, this, FALLING);

    // Setup speaker mute control pin, unmute by default
    pinMode(Pins::RingerMute, OUTPUT);
    setMute(Mute::Off);

    // LED pin for testing
    pinMode(Pins::Led, OUTPUT);
    setLed(Led::Off);
}

void DoorPhoneMonitor::task()
{
    _coreApplication.task();

    if (millis() - _ringingSensorCheckTime >= RingingSensor::CheckInvervalMs) {
        // Reset 'Sensor Check' timer
        _ringingSensorCheckTime = millis();

        if (_ringingSensorTriggerCount >= RingingSensor::CountThreshold) {
            _log.info_P(PSTR("ringing detected: triggerCount=%u"), _ringingSensorTriggerCount);

            // Reset 'Active State' timer
            _ringingStateCheckTime = millis();
            _mqtt.ringing = true;

            setLed(Led::On);
        }

        _ringingSensorTriggerCount = 0;
    }

    if (_mqtt.ringing && millis() - _ringingStateCheckTime >= RingingSensor::ActiveStateTimeoutMs) {
        _mqtt.ringing = false;

        setLed(Led::Off);

        _log.info_P(PSTR("ringing ended"));
    }
}

void DoorPhoneMonitor::setupMqtt()
{
    _mqtt.muted.setChangedHandler([this](const bool& muted) {
        _log.info_P(PSTR("mute state changed via MQTT: muted=%d"), muted ? 1 : 0);
        setMute(muted ? Mute::On : Mute::Off);
    });

    {
        std::stringstream config;

        config << '{';
        config << Utils::pgmToStdString(PSTR(R"("icon":"mdi:phone-ring")"));
        config << Utils::pgmToStdString(PSTR(R"(,"name":"Door Phone Ringing")"));
        config << Utils::pgmToStdString(PSTR(R"(,"object_id":"door_phone_monitor_ringing")"));
        config << Utils::pgmToStdString(PSTR(R"(,"unique_id":"door_phone_monitor_ringing")"));
        config << Utils::pgmToStdString(PSTR(R"(,"state_topic":"doorphone/ringing")"));
        config << '}';

        _coreApplication.mqttClient().publish(
            PSTR("homeassistant/sensor/door_phone_monitor_ringing/config"),
            config.str(),
            false
        );
    }

    {
        std::stringstream config;

        config << '{';
        config << Utils::pgmToStdString(PSTR(R"("icon":"mdi:volume-off")"));
        config << Utils::pgmToStdString(PSTR(R"(,"name":"Door Phone Mute")"));
        config << Utils::pgmToStdString(PSTR(R"(,"object_id":"door_phone_monitor_mute")"));
        config << Utils::pgmToStdString(PSTR(R"(,"unique_id":"door_phone_monitor_mute")"));
        config << Utils::pgmToStdString(PSTR(R"(,"command_topic":"doorphone/mute")"));
        config << Utils::pgmToStdString(PSTR(R"(,"state_topic":"doorphone/muted")"));
        config << Utils::pgmToStdString(PSTR(R"(,"payload_on":"1")"));
        config << Utils::pgmToStdString(PSTR(R"(,"payload_off":"0")"));
        config << Utils::pgmToStdString(PSTR(R"(,"state_on":"1")"));
        config << Utils::pgmToStdString(PSTR(R"(,"state_off":"0")"));
        config << '}';

        _coreApplication.mqttClient().publish(
            PSTR("homeassistant/switch/door_phone_monitor_mute/config"),
            config.str(),
            false
        );
    }

    updateMqtt();
}

void DoorPhoneMonitor::updateMqtt()
{
}

void DoorPhoneMonitor::setLed(const Led state)
{
    digitalWrite(Pins::Led, state == Led::On ? 0 : 1);
}

void DoorPhoneMonitor::setMute(const Mute state)
{
    digitalWrite(Pins::RingerMute, state == Mute::On ? 1 : 0);
}

void ICACHE_RAM_ATTR DoorPhoneMonitor::ringingSenseIsr(void* const arg)
{
    auto* self = static_cast<DoorPhoneMonitor*>(arg);
    ++self->_ringingSensorTriggerCount;
}