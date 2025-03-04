#include "userTimers.h"

#include <Arduino.h>
#include <GyverDBFile.h>
#include <GyverDS3231.h>
#include <SettingsGyver.h>
#include <iarduino_I2C_Relay.h>

#include "data.h"
#include "led.h"
#include "modbus.h"
#include "nastroyki.h"
#include "relay6_algorithms.h"
#include "reley.h"
#include "settings.h"
#include "timer.h"

static bool t6_rightDay = 0;  // День недели для таймера 6
static uint8_t lastWeekDay = 0;

// Объявления функций
static bool checkTimeBasedAlgorithm();
static bool checkTemperatureBasedAlgorithm();
static bool checkHumidityBasedAlgorithm();
static bool checkSoil1BasedAlgorithm();
static bool checkSoil2BasedAlgorithm();

void userRelays() {  // реле

    switch (data.Air1.StateAir) {  // Реле AirTemp для нагрев  воздуха
        case 0:
            if (db[kk::airTempRele_enabled].toInt() != 0) {
                data.Air1.StateAir = 5;
            } else if (data.Air1.TempRele_on) {
                data.Air1.StateAir = 20;  // выключим по перемещению ползунка в OFF
            }
            break;
        case 5:
            if (data.Air1.tx10 <= data.Air1.tTrigx10 - data.Air1.tTresholdx10) {  // data.dhtOne.tx10 >= data.dhtOne.tTrigx10) { // ожидание понижение температуры
                data.Air1.StateAir = 10;
            }
            break;
        case 10:  // включаем нагрев
            reley_1_1_on();
            data.Air1.TempRele_on = true;
            data.Air1.StateAir = 15;
            break;
        case 15:  // ожидаем повышение температуры - трешхолд
            if (data.Air1.tx10 >= data.Air1.tTrigx10) {
                data.Air1.StateAir = 20;
            }
            break;
        case 20:  // используется при переключении ползунка в морде
            reley_1_1_off();
            data.Air1.TempRele_on = false;
            data.Air1.StateAir = 0;
            break;
    }  // switch (data.Air1.StateAir)

    switch (data.Air1.StateHume) {  // реле AirHume для увлажнение воздуха
        case 0:
            if (db[kk::airHumeRele_enabled].toInt() != 0) {
                data.Air1.StateHume = 5;
            } else if (data.Air1.HumeRele_on) {
                data.Air1.StateHume = 20;  // выключим по перемещению ползунка в OFF
            }
            break;
        case 5:
            if (data.Air1.hx10 <= data.Air1.hTrigx10 - data.Air1.hTresholdx10) {
                data.Air1.StateHume = 10;
            }
            break;
        case 10:
            reley_1_2_on();
            data.Air1.HumeRele_on = true;
            data.Air1.StateHume = 15;
            break;
        case 15:  // ожидаем повышение влажности
            if (data.Air1.hx10 >= data.Air1.hTrigx10) {
                data.Air1.StateHume = 20;
            }
            break;
        case 20:  // используется при переключении ползунка в морде
            // выключаем полив
            reley_1_2_off();
            data.Air1.HumeRele_on = false;
            data.Air1.StateHume = 0;
            break;
    }  // switch (data.Air1.StateHume)

    switch (data.Soil1.StateHume) {  // Реле почвы 1
        // инициализация
        //  ползунок включен - отрабатываем
        // выключен и включено реле - уйдем на выключение
        case 0:
            if (db[kk::soilHumeRele_enabled].toInt() != 0) {
                data.Soil1.StateHume = 5;
            } else if (data.Soil1.HumeRele_on) {
                data.Soil1.StateHume = 20;  // выключим по перемещению ползунка в OFF
            }
            break;
        case 5:  // ожидание понижения влажности
            if (data.Soil1.hx10 <= data.Soil1.hTrigx10) {
                data.Soil1.StateHume = 10;
            }
            break;
        case 10:  // включаем полив
            reley_2_1_on();
            data.Soil1.HumeRele_on = true;
            data.Soil1.StateHume = 15;
            break;
        case 15:                                                                     // ожидаем повышения влажности + трешхолд
            if (data.Soil1.hx10 >= data.Soil1.hTrigx10 + data.Soil1.hTresholdx10) {  //(data.dhtTwo.hum >= data.dhtTwo.hTrig + data.dhtTwo.hTreshold) {
                data.Soil1.StateHume = 20;
            }
            break;
        case 20:  // используется при переключении ползунка в морде
            // выключаем полив
            reley_2_1_off();
            data.Soil1.HumeRele_on = false;
            data.Soil1.StateHume = 0;
            break;
    }  // switch (Реле почвы 1)

    switch (data.Soil2.StateHume) {  // реле Датчика почвы 2 для
        // инициализация
        //  ползунок включен - отрабатываем
        // выключен и включено реле - уйдем на выключение
        case 0:
            if (db[kk::soil2HumeRele_enabled].toInt() != 0) {
                data.Soil2.StateHume = 5;
            } else if (data.Soil2.HumeRele_on) {
                data.Soil2.StateHume = 20;  // выключим по перемещению ползунка в OFF
            }
            break;
        case 5:  // ожидание понижения влажности
            if (data.Soil2.hx10 <= data.Soil2.hTrigx10) {
                data.Soil2.StateHume = 10;
            }
            break;
        case 10:  // включаем полив
            reley_2_2_on();
            data.Soil2.HumeRele_on = true;
            data.Soil2.StateHume = 15;
            break;
        case 15:                                                                     // ожидаем повышения влажности + трешхолд
            if (data.Soil2.hx10 >= data.Soil2.hTrigx10 + data.Soil2.hTresholdx10) {  //(data.dhtTwo.hum >= data.dhtTwo.hTrig + data.dhtTwo.hTreshold) {
                data.Soil2.StateHume = 20;
            }
            break;
        case 20:  // используется при переключении ползунка в морде
            // выключаем полив
            reley_2_2_off();
            data.Soil2.HumeRele_on = false;
            data.Soil2.StateHume = 0;
            break;
    }  // switch (Реле почвы 2)
}  // userRelays()

// Реализации функций
static bool checkTimeBasedAlgorithm() {
    bool isRightDay = false;
    switch (curDataTime.weekDay) {
        case 1: isRightDay = db[kk::t6Discr_inMonday].toInt(); break;
        case 2: isRightDay = db[kk::t6Discr_inTuesday].toInt(); break;
        case 3: isRightDay = db[kk::t6Discr_inWensday].toInt(); break;
        case 4: isRightDay = db[kk::t6Discr_inThursday].toInt(); break;
        case 5: isRightDay = db[kk::t6Discr_inFriday].toInt(); break;
        case 6: isRightDay = db[kk::t6Discr_inSaturday].toInt(); break;
        case 7: isRightDay = db[kk::t6Discr_inSunday].toInt(); break;
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

static bool checkTemperatureBasedAlgorithm() {
    int16_t threshold = db[kk::t6Discr_temp_threshold].toInt() * 10;
    int16_t hysteresis = db[kk::t6Discr_hysteresis].toInt() * 10;
    if (!data.rel6_on) {
        return data.Air1.tx10 <= threshold;
    } else {
        return data.Air1.tx10 < threshold + hysteresis;
    }
}

static bool checkHumidityBasedAlgorithm() {
    int16_t threshold = db[kk::t6Discr_hum_threshold].toInt() * 10;
    int16_t hysteresis = db[kk::t6Discr_hysteresis].toInt() * 10;
    if (!data.rel6_on) {
        return data.Air1.hx10 <= threshold;
    } else {
        return data.Air1.hx10 < threshold + hysteresis;
    }
}

static bool checkSoil1BasedAlgorithm() {
    int16_t threshold = db[kk::t6Discr_hum_threshold].toInt() * 10;
    int16_t hysteresis = db[kk::t6Discr_hysteresis].toInt() * 10;
    if (!data.rel6_on) {
        return data.Soil1.hx10 <= threshold;
    } else {
        return data.Soil1.hx10 < threshold + hysteresis;
    }
}

static bool checkSoil2BasedAlgorithm() {
    int16_t threshold = db[kk::t6Discr_hum_threshold].toInt() * 10;
    int16_t hysteresis = db[kk::t6Discr_hysteresis].toInt() * 10;
    if (!data.rel6_on) {
        return data.Soil2.hx10 <= threshold;
    } else {
        return data.Soil2.hx10 < threshold + hysteresis;
    }
}

void userSixTimers() {  // Таймеры с1 - по 6 ===

    // === таймер Реле 1
    // if (db[kk::t1Discr_enabled].toBool()) {
    if (data.t1discr_enbl) {
        if (db[kk::t1Discr_startTime].toInt() < db[kk::t1Discr_endTime].toInt())  // если нет перехода через полночь
        {
            if ((db[kk::t1Discr_startTime].toInt() <= data.secondsNow) && (data.secondsNow <= db[kk::t1Discr_endTime].toInt())) {
                if (!data.rel1_on)  // avoid extra digWrite
                {
                    reley_3_1_on();
                    data.rel1_on = 1;
                }
            } else {
                if (data.rel1_on)  // avoid extra digWrite
                {
                    reley_3_1_off();
                    data.rel1_on = 0;
                }
            }
        } else if (db[kk::t1Discr_startTime].toInt() > db[kk::t1Discr_endTime].toInt())  // если есть переход через полночь
        {
            if ((db[kk::t1Discr_startTime].toInt() >= data.secondsNow) && (data.secondsNow >= db[kk::t1Discr_endTime].toInt())) {
                if (data.rel1_on)  // avoid extra digWrite
                {
                    reley_3_1_off();
                    data.rel1_on = 0;
                }
            } else {
                if (!data.rel1_on)  // avoid extra digWrite
                {
                    reley_3_1_on();
                    data.rel1_on = 1;
                }
            }
        }
    } else {
        if (data.rel1_on)  // если было включено, выключим
        {
            reley_3_1_off();
            data.rel1_on = 0;
        }
    }

    //=== таймер Реле 6 ===
    if (data.t6discr_enbl) {
        uint8_t algorithm = db[kk::t6Discr_algorithm].toInt();
        bool shouldBeOn = Relay6Algorithms::executeAlgorithm(algorithm);

        if (shouldBeOn != data.rel6_on) {
            if (shouldBeOn) {
                reley_3_2_on();
                data.rel6_on = 1;
            } else {
                reley_3_2_off();
                data.rel6_on = 0;
            }
        }
    } else {
        if (data.rel6_on) {
            reley_3_2_off();
            data.rel6_on = 0;
        }
    }

}  // userSixTimers()