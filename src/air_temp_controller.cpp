#include "air_temp_controller.h"

int16_t AirTemperatureController::getTargetTemperature() {
    uint32_t dayStart = db[kk::airRele_dayStartTime].toInt();
    uint32_t nightStart = db[kk::airRele_nightStartTime].toInt();
    
    if (dayStart < nightStart) {
        // День: между dayStart и nightStart
        if (data.secondsNow >= dayStart && data.secondsNow < nightStart) {
            return db[kk::airRele_dayTemp].toInt() * 10;
        } else {
            return db[kk::airRele_nightTemp].toInt() * 10;
        }
    } else {
        // День: между dayStart и полночью, или между полночью и nightStart
        if (data.secondsNow >= dayStart || data.secondsNow < nightStart) {
            return db[kk::airRele_dayTemp].toInt() * 10;
        } else {
            return db[kk::airRele_nightTemp].toInt() * 10;
        }
    }
}

void AirTemperatureController::setHysteresis() {
    switch (db[kk::airRele_tempHysteresis].toInt()) {
        case 0: data.Air1.tTresholdx10 = 5; break;
        case 1: data.Air1.tTresholdx10 = 10; break;
        case 2: data.Air1.tTresholdx10 = 20; break;
        case 3: data.Air1.tTresholdx10 = 30; break;
    }
} 