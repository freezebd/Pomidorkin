#include "relay6_algorithms.h"

bool Relay6Algorithms::checkTimeBasedAlgorithm() {
    bool isRightDay = false;
    switch (curDataTime.weekDay) {
        case 0: isRightDay = db[kk::t6Discr_inMonday].toInt(); break;
        case 1: isRightDay = db[kk::t6Discr_inTuesday].toInt(); break;
        case 2: isRightDay = db[kk::t6Discr_inWensday].toInt(); break;
        case 3: isRightDay = db[kk::t6Discr_inThursday].toInt(); break;
        case 4: isRightDay = db[kk::t6Discr_inFriday].toInt(); break;
        case 5: isRightDay = db[kk::t6Discr_inSaturday].toInt(); break;
        case 6: isRightDay = db[kk::t6Discr_inSunday].toInt(); break;
    }

    if (isRightDay) {
        if (db[kk::t6Discr_startTime].toInt() < db[kk::t6Discr_endTime].toInt()) {
            return (db[kk::t6Discr_startTime].toInt() <= data.secondsNow) && 
                   (data.secondsNow <= db[kk::t6Discr_endTime].toInt());
        } else {
            return !((db[kk::t6Discr_startTime].toInt() >= data.secondsNow) && 
                     (data.secondsNow >= db[kk::t6Discr_endTime].toInt()));
        }
    }
    return false;
}

bool Relay6Algorithms::checkTemperatureBasedAlgorithm() {
    int16_t threshold = db[kk::t6Discr_temp_threshold].toInt() * 10;  // Порог температуры
    int16_t hysteresis = db[kk::t6Discr_hysteresis].toInt() * 10;    // Гистерезис
    if (!data.rel6_on) {
        return data.Air1.tx10 <= threshold - hysteresis;  // Включаем при температуре ниже порога на гистерезис
    } else {
        return data.Air1.tx10 < threshold;  // Выключаем при достижении порога
    }
}

bool Relay6Algorithms::checkHumidityBasedAlgorithm() {
    int16_t threshold = db[kk::t6Discr_hum_threshold].toInt() * 10;  // Порог влажности
    int16_t hysteresis = db[kk::t6Discr_hysteresis].toInt() * 10;    // Гистерезис
    if (!data.rel6_on) {
        return data.Air1.hx10 <= threshold - hysteresis;  // Включаем при влажности ниже порога на гистерезис
    } else {
        return data.Air1.hx10 < threshold;  // Выключаем при достижении порога
    }
}

bool Relay6Algorithms::checkSoil1BasedAlgorithm() {
    int16_t threshold = db[kk::t6Discr_hum_threshold].toInt() * 10;  // Порог влажности
    int16_t hysteresis = db[kk::t6Discr_hysteresis].toInt() * 10;    // Гистерезис
    if (!data.rel6_on) {
        return data.Soil1.hx10 <= threshold - hysteresis;  // Включаем при влажности ниже порога на гистерезис
    } else {
        return data.Soil1.hx10 < threshold;  // Выключаем при достижении порога
    }
}

bool Relay6Algorithms::checkSoil2BasedAlgorithm() {
    int16_t threshold = db[kk::t6Discr_hum_threshold].toInt() * 10;  // Порог влажности
    int16_t hysteresis = db[kk::t6Discr_hysteresis].toInt() * 10;    // Гистерезис
    if (!data.rel6_on) {
        return data.Soil2.hx10 <= threshold - hysteresis;  // Включаем при влажности ниже порога на гистерезис
    } else {
        return data.Soil2.hx10 < threshold;  // Выключаем при достижении порога
    }
}

bool Relay6Algorithms::executeAlgorithm(uint8_t algorithm) {
    switch (algorithm) {
        case 0: return checkTimeBasedAlgorithm();
        case 1: return checkTemperatureBasedAlgorithm();
        case 2: return checkHumidityBasedAlgorithm();
        case 3: return checkSoil1BasedAlgorithm();
        case 4: return checkSoil2BasedAlgorithm();
        default: return false;
    }
} 