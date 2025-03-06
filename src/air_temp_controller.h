#pragma once
#include <Arduino.h>
#include "data.h"
#include "settings.h"

class AirTemperatureController {
public:
    static int16_t getTargetTemperature();
    static void setHysteresis();
}; 