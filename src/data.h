#pragma once
#include <Arduino.h>

struct Air_sensor
{
    float tfloat = 0.0;      // температура воздуха float
    int16_t tx10 = 0;       // температура воздуха integer
    int16_t tTrigx10 = 0;   // температура воздуха срабатывания реле
    int16_t tTresholdx10 = 0;  // температура воздуха гистерезиса отключения реле
    bool TempRele_on = false;    // флаг включения реле температуры воздкха
    byte StateAir = 0;         // автомат работы реле температуры воздуха

    float hfloat = 0.0;      // влажность воздуха float
    int16_t hx10 = 0;       // влажность воздуха integer
    int16_t hTrigx10 = 0;      // влажность воздуха сработки реле
    int16_t hTresholdx10 = 0;  // Влажность воздуха гистерезиса отключения реле
    bool HumeRele_on = false;    // флаг включения реле влажности воздкха
    byte StateHume = 0;         // автомат работы реле влажности воздуха
};

struct Soil_sensor
{
    float tfloat = 0.0;      // температура почва float
    int16_t tx10 = 0;       //температура почва integer
    int16_t tTrigx10 = 0;      //температура почва сработки реле
    int16_t tTresholdx10 = 0;  //температура почва гистерезиса отключения реле
    bool TempRele_on = false;    // флаг включения реле температуры почвы
    byte StateTemp = 0;         // автомат работы реле температуры почвы

    float hfloat = 0.0;      // влажность почва float
    int16_t hx10 = 0;       //влажность почва integer
    int16_t hTrigx10 = 0;      //влажность почва сработки реле
    int16_t hTresholdx10 = 0;  //влажность почва гистерезиса отключения реле
    bool HumeRele_on = false;    // флаг включения реле влажности почвы
    byte StateHume = 0;         // автомат работы реле влажности почвы
};

struct Soil2_sensor
{
    float tfloat = 0.0;      // температура почва float
    int16_t tx10 = 0;       //температура почва integer
    int16_t tTrigx10 = 0;      //температура почва сработки реле
    int16_t tTresholdx10 = 0;  //температура почва гистерезиса отключения реле
    bool TempRele_on = false;    // флаг включения реле температуры почвы
    byte StateTemp = 0;         // автомат работы реле температуры почвы

    float hfloat = 0.0;      // влажность почва float
    int16_t hx10 = 0;       //влажность почва integer   
    int16_t hTrigx10 = 0;      //влажность почва сработки реле
    int16_t hTresholdx10 = 0;  //влажность почва гистерезиса отключения реле
    bool HumeRele_on = false;    // флаг включения реле влажности почвы
    byte StateHume = 0;         // автомат работы реле влажности почвы

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

// Структура для хранения информации о найденных реле
struct RelayInfo {
    uint8_t address;
    bool isPresent;
};

struct Data {  // обьявляем класс структуры

    uint32_t secondsNow = 44000ul;
    uint32_t datime = 1738875600ul;
    uint32_t secondsUptime = 1;
    byte uptime_Days = 0;
    uint8_t old_address = 0;  // старый адрес реле
    uint8_t new_address = 0;  // новый адрес реле
    RelayInfo relays[8];         // массив для хранения информации о найденных реле
    uint8_t relay_count = 0;     // количество найденных реле


    struct Air_sensor Air1;
    struct Soil_sensor Soil1;
    struct Soil_sensor Soil2;
 
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
extern Soil_sensor Soil2;


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
