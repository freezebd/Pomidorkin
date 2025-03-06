#include "air_temp_control.h"
#include "reley.h"

// Функция для определения целевой температуры
static int16_t getTargetTemperature() {
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

// Функция для установки гистерезиса
static void setHysteresis() {
    switch (db[kk::airRele_tempHysteresis].toInt()) {
        case 0: data.Air1.tTresholdx10 = 5; break;
        case 1: data.Air1.tTresholdx10 = 10; break;
        case 2: data.Air1.tTresholdx10 = 20; break;
        case 3: data.Air1.tTresholdx10 = 30; break;
    }
}

// Функция для управления температурой воздуха
void controlAirTemperature() {
    switch (data.Air1.StateAir) {
        case 0:
            if (db[kk::airTempRele_enabled].toInt() != 0) {
                data.Air1.StateAir = 5;
            } else if (data.Air1.TempRele_on) {
                data.Air1.StateAir = 20;  // выключим по перемещению ползунка в OFF
            }
            break;
        case 5:
            // Устанавливаем гистерезис
            setHysteresis();
            
            // Определяем целевую температуру
            data.Air1.tTrigx10 = getTargetTemperature();
            
            if (data.Air1.tx10 <= data.Air1.tTrigx10 - data.Air1.tTresholdx10) {
                data.Air1.StateAir = 10;
            }
            break;
        case 10:  // включаем нагрев
            reley_1_1_on();
            data.Air1.TempRele_on = true;
            data.Air1.StateAir = 15;
            break;
        case 15:  // ожидаем повышение температуры - трешхолд
            // Проверяем, не стала ли целевая температура ниже текущей
            data.Air1.tTrigx10 = getTargetTemperature();
            if (data.Air1.tx10 >= data.Air1.tTrigx10 || data.Air1.tx10 > data.Air1.tTrigx10 + data.Air1.tTresholdx10) {
                data.Air1.StateAir = 20;
            }
            break;
        case 20:  // используется при переключении ползунка в морде
            reley_1_1_off();
            data.Air1.TempRele_on = false;
            data.Air1.StateAir = 0;
            break;
        case 25:
            if (data.Air1.tfloat == -80) { // принудительно выключаем свитч и  выключаем реле
                db[kk::airTempRele_enabled] = 0;
                data.Air1.StateAir = 20;  
            }
            break;
    }
} 