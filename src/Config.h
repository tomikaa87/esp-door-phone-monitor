#pragma once

#include <Arduino.h>
#include <cstdint>

namespace Config
{
    constexpr auto DeviceCount = 2;

    namespace Pins
    {
        static const int Adc = PIN_A0;
        static const int OneWire = 13;
        static const uint8_t Relay[] = { 5, 4, 14, 12 };
    }

    namespace ButtonAnalogThresholds
    {
        // ADC voltage range changes when the WiFi is being used
#ifdef NOT_USING_WIFI
        static const int None = 100;
        static const int Button1 = 250;
        static const int Button2 = 500;
        static const int Button3 = 750;
        static const int Button4 = 1024;
#else
        static const int None = 100;
        static const int Button1 = 300;
        static const int Button2 = 600;
        static const int Button3 = 850;
        static const int Button4 = 1024;
#endif
    }

    static const uint8_t DS18B20Resolution = 12;
    static const uint32_t TemperatureSensorReadInterval = 1000;

    static const uint32_t ButtonReadInterval = 100;
    static const uint32_t ButtonLongPressInterval = 1000;

    namespace Limits
    {
        static const size_t MaxButtonEventHandlers = 5;
        static const size_t MaxTemperatureEventHandlers = 5;
        static const size_t MaxBlynkEventHandlers = 5;
        static const size_t MaxClockEpochUpdatedEventHandlers = 5;
        static const size_t MaxLogEventHandlers = 5;
    }

    namespace Relays
    {
        static const uint32_t PulseWidth = 500;

        // Relay groups ensure that all relays in a group work mutually exclusive
        // (only one per group can be active).
        // Relays using the same number are in the same group.
        static const uint8_t Groups[] = { 0, 0, 1, 1 };
    }

    namespace Clock
    {
        static const char NtpServerAddress[] = "login-vlan194.budapest2.hpc.niif.hu";
        static const int LocalTimeOffsetMinutes = 60;
        static const int NtpUpdateIntervalMinutes = 30;
        static const int NtpUpdateRetryIntervalSeconds = 15;
    }

    namespace Automator
    {
        static const uint32_t CheckTimerInterval = 100;
    }

    namespace Persistent
    {
        static const int BaseAddress = 0;
    }

    namespace Logger
    {
        static const size_t LogLineBufferSize = 128;
    }
}

// Due to the fancy macro magic in the Blynk API,
// namespaces and integer constants cannot be used for pins.
#define CONFIG_BLYNK_PIN_ROOM_TEMPERATURE               V80
#define CONFIG_BLYNK_PIN_RELAY_1_PULSE_BUTTON           V79
#define CONFIG_BLYNK_PIN_RELAY_2_PULSE_BUTTON           V78
#define CONFIG_BLYNK_PIN_RELAY_3_PULSE_BUTTON           V77
#define CONFIG_BLYNK_PIN_RELAY_4_PULSE_BUTTON           V76
#define CONFIG_BLYNK_PIN_OPEN_TIME_INPUT                V75
#define CONFIG_BLYNK_PIN_SHUTTER_TIMER_ACTIVE_BUTTON    V74
#define CONFIG_BLYNK_PIN_VIRTUAL_TERMINAL               V64

#define DEBUG_BUTTON_HANDLER
//#define DEBUG_TEMPERATURE_SENSOR
#define DEBUG_BLYNK_HANDLER