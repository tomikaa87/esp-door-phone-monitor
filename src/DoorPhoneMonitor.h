#pragma once

#include "Settings.h"

#include <CoreApplication.h>
#include <Logger.h>
#include <network/MQTT/MqttVariable.h>

#include <array>

class MqttClient;

class DoorPhoneMonitor
{
public:
    explicit DoorPhoneMonitor(const ApplicationConfig& appConfig);

    void task();

private:
    const ApplicationConfig& _appConfig;
    CoreApplication _coreApplication;
    const Logger _log{ "DoorPhoneMonitor" };
    Settings _settings;

    unsigned long _ringingSensorCheckTime = 0;
    unsigned long _ringingStateCheckTime = 0;

    void onBlynkUpdateNeeded();

    struct Mqtt {
        Mqtt(MqttClient& mqttClient)
            : ringing{ PSTR("doorphone/ringing"), mqttClient }
            , muted{ PSTR("doorphone/muted"), mqttClient }
        {}

        MqttVariable<bool> ringing;
        MqttVariable<bool> muted;
    } _mqtt;

    void setupMqtt();
    void updateMqtt();
};