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
    Created on 2017-01-09
*/

#include "Settings.h"

#include <iomanip>
#include <sstream>

Settings::Settings(ISettingsHandler& handler)
    : _handler(handler)
{
    _handler.setDefaultsLoader([this](const ISettingsHandler::DefaultsLoadReason reason) {
        _log.warning("defaults load requested from settings handler: reason=%d", reason);
        loadDefaults();
    });

    _handler.registerSetting(data);

    load();
}

bool Settings::load()
{
    const auto ok = _handler.load();

    _log.info("loading settings: ok=%d", ok);

    dumpData();

    if (!check()) {
        _log.warning("loaded settings corrected");
        save();
    }

    return ok;
}

bool Settings::save()
{
    if (!check()) {
        _log.warning("settings corrected before saving");
    }

    dumpData();

    const auto ok = _handler.save();

    _log.info("saving settings: ok=%d", ok);

    return ok;
}

void Settings::loadDefaults()
{
    _log.info("loading defaults");

    data = {};

    if (!check()) {
        _log.warning("loaded defaults corrected");
    }
}

bool Settings::check()
{
    bool modified = false;



    return !modified;
}

void Settings::dumpData() const
{
    _log.debug("Settings{ }"
    );
}