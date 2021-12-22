/*
    This file is part of esp-thermostat.

    esp-thermostat is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    esp-thermostat is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with esp-thermostat.  If not, see <http://www.gnu.org/licenses/>.

    Author: Tamas Karpati
    Created on 2020-12-21
*/

#pragma once

#include <ISettingsHandler.h>
#include <Logger.h>

#include <cstdint>

class Settings
{
public:
    static constexpr uint8_t DataVersion = 1;

    explicit Settings(ISettingsHandler& handler);

    DECLARE_SETTINGS_STRUCT(Data)
    {
    };

    Data data;

    bool load();
    bool save();

    void loadDefaults();

private:
    Logger _log{ "Settings" };
    ISettingsHandler& _handler;

    bool check();

    void dumpData() const;
};
