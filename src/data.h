#pragma once
#include <Arduino.h>

struct Air_sensor
{
    float tfloat = -100.0;
    float hfloat = -100.0;
    int16_t tx10 = 0;       // температура переведенная из флоат в интежер * 10
    int16_t hx10 = 0;       // влажность int
    int16_t tTrigx10 = 0;   // температура срабатывания реле
    int16_t hTrig = 0;      //влажность сработки реле
    int16_t hTreshold = 0;
    int16_t tTreshold = 0;
    bool Rel_on = false;    // флаг включения реле
    byte State = 0;         // автомат работы реле
};
struct Soil_sensor
{
    float SoilTemp = -100.0;
    float SoilHum = -100.0;
    int16_t hTrig = 0; //влажность сработки реле
    int16_t hTreshold = 0;
    bool Rel_on = false;  // флаг включения реле
    byte State = 0;  // автомат работы реле
};


// подструктруа структуры Data
// struct Ds18b20_sensor {
//     int16_t tx10 = 0;       // температура переведенная из флоат в интежер * 10
//     int16_t tTrigx10 = 0;   // температура срабатывания реле
//     int16_t tTreshold = 0;  // температура гистерезиса отключения реле
//     float tfloat = -80.0;
//     bool rel_on = false;  // флаг включения реле
//     byte State = 0;       // автомат состояний работы реле
// };

struct Data {  // обьявляем класс структуры

    uint32_t secondsNow = 44000ul;
    uint32_t secondsUptime = 1;
    byte uptime_Days = 0;

    struct Air_sensor Air1;
    struct Soil_sensor Soil1;
 
    // чтобы не проверять булевы значения ползунков в интерфейсе из базы, пишем их сюда сначала
    bool t1discr_enbl = 0;
    bool rel1_on = 0;
    bool t2discr_enbl = 0;
    bool rel2_on = 0;
    bool t3discr_enbl = 0;
    bool rel3_on = 0;
    bool t4discr_enbl = 0;
    bool rel4_on = 0;
    bool t5discr_enbl = 0;
    bool rel5_on = 0;
    bool t6discr_enbl = 0;
    bool rel6_on = 0;
    
};

// for cpp
extern Data data;  // объявляем что у нас будет переменная data класса Data
extern Air_sensor Air1; 
extern Soil_sensor Soil1;


// constexpr size_t BRIGHT_SIZE = 100;// размер массива
// extern uint16_t brightn[BRIGHT_SIZE];

/*
byte red, green, blue;
unsigned long rgb = 0x6f56a3;

red = rgb >> 16
green = (rgb & 0x00ff00) >> 8;
blue = (rgb & 0x0000ff);

или



String hexstring = "B787B7";
    // long number = (long) strtol( &hexstring[1], NULL, 16);
    long number = (long) strtol( &hexstring[0], NULL, 16);
    int r = number >> 16;
    int g = number >> 8 & 0xFF;
    int b = number & 0xFF;
*/

// bool    t1Discr_enabled,
// uint32_t    t1Discr_startTime,
// uint32_t    t1Discr_endTime,

// bool    t2Discr_enabled,
// uint32_t    t2Discr_startTime,
// uint32_t    t2Discr_endTime,
// bool    t3Discr_enabled,
// uint32_t    t3Discr_startTime,
// uint32_t    t3Discr_endTime,

// bool    t4Discr_enabled,
// uint32_t    t4Discr_startTime,
// uint32_t    t4Discr_endTime,

// bool    t5Discr_enabled,
// uint32_t    t5Discr_startTime,
// uint32_t    t5Discr_endTime,
