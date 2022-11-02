#include "DoorPhoneMonitor.h"
#include "PrivateConfig.h"

#include <Arduino.h>

#include <memory>

static std::unique_ptr<DoorPhoneMonitor> doorPhoneMonitor;

void setup()
{
    static ApplicationConfig appConfig;

    appConfig.firmwareVersion = VersionNumber{ 1, 2, 0 };

    appConfig.logging.syslog.enabled = true;
    appConfig.logging.syslog.hostName = Config::Logging::SyslogHostName;
    appConfig.logging.syslog.serverHostName = Config::Logging::SyslogServerHost;
    appConfig.logging.syslog.serverPort = Config::Logging::SyslogServerPort;

    appConfig.mqtt.brokerIp = Config::Mqtt::BrokerIp;
    appConfig.mqtt.brokerPort = Config::Mqtt::BrokerPort;
    appConfig.mqtt.enabled = true;
    appConfig.mqtt.id = Config::Mqtt::Id;
    appConfig.mqtt.user = Config::Mqtt::User;
    appConfig.mqtt.password = Config::Mqtt::Password;

    appConfig.persistence.Size = 8;

#ifdef IOT_ENABLE_HTTP_OTA_UPDATE
    appConfig.otaUpdate.updateCheckIntervalMs = 60000;
    appConfig.otaUpdate.updateUrl = Config::OtaUpdate::FirmwareUpdateUrl;
#endif
    appConfig.otaUpdate.arduinoOtaPasswordHash = Config::OtaUpdate::ArduinoOtaPasswordHash;

    appConfig.wifi.password = Config::WiFi::Password;
    appConfig.wifi.ssid = Config::WiFi::SSID;

    appConfig.hostName = Config::HostName;

    doorPhoneMonitor.reset(new DoorPhoneMonitor(appConfig));
}

void loop()
{
    doorPhoneMonitor->task();
}