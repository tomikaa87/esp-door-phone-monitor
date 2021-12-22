#include "DoorPhoneMonitor.h"

#include <cstdio>

namespace RingingSensor
{
    constexpr unsigned long CheckInvervalMs = 500;
    constexpr unsigned long ActiveStateTimeoutMs = 3000;
    constexpr auto CountThreshold = 100u;
    volatile auto triggerCount = 0u;
}

ICACHE_RAM_ATTR void ringingSensorIsr()
{
    RingingSensor::triggerCount += 1;
    digitalWrite(LED_BUILTIN, RingingSensor::triggerCount & 1 == 1);
}

DoorPhoneMonitor::DoorPhoneMonitor(const ApplicationConfig& appConfig)
    : _appConfig(appConfig)
    , _coreApplication(_appConfig)
    , _settings(_coreApplication.settings())
    , _mqtt(_coreApplication.mqttClient())
{
    Logger::setup(_appConfig, _coreApplication.systemClock());

    setupMqtt();
    _coreApplication.setMqttUpdateHandler([this]{ updateMqtt(); });

    // Setup ringing sensor input pin
    pinMode(D7, INPUT);
    attachInterrupt(D7, ringingSensorIsr, FALLING);

    // LED pin for testing
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, 1);
}

void DoorPhoneMonitor::task()
{
    _coreApplication.task();

    if (millis() - _ringingSensorCheckTime >= RingingSensor::CheckInvervalMs) {
        // Reset 'Sensor Check' timer
        _ringingSensorCheckTime = millis();

        if (RingingSensor::triggerCount >= RingingSensor::CountThreshold) {
            _log.info("ringing detected: triggerCount=%u", RingingSensor::triggerCount);

            RingingSensor::triggerCount = 0;

            // Reset 'Active State' timer
            _ringingStateCheckTime = millis();
            _mqtt.ringing = true;
        }

        RingingSensor::triggerCount = 0;
    }

    if (_mqtt.ringing && millis() - _ringingStateCheckTime >= RingingSensor::ActiveStateTimeoutMs) {
        _mqtt.ringing = false;
        digitalWrite(LED_BUILTIN, 1);
        _log.info("ringing ended");
    }
}

void DoorPhoneMonitor::setupMqtt()
{
    updateMqtt();
}

void DoorPhoneMonitor::updateMqtt()
{
}