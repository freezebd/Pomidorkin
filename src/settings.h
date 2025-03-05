#pragma once
#include <Arduino.h>
#include <GyverDBFile.h>
#include <GyverDS3231.h>
#include <SettingsGyver.h>


DB_KEYS(  // ключи по которым идет привязка к базе и к виджетам в web
    kk,
    wifi_ssid,               // имя wifi
    wifi_pass,               // пароль wifi
    wifi_scan,              // кнопка сканирования сетей
    wifi_networks,          // список найденных сетей
    wifi_selected,          // выбранная сеть
    apply,                   // кнопка применить
    ntp_gmt,                 // часовой пояс
    ntp_gmt_set,             // кнопка установить часовой пояс
    secondsNow,              // секунды с начала дня
    secondsUptime,           // секунды с начала запуска
    dayofweek,               // день недели
    uptimeDays,              // дни работы

    airTempName,             // имя датчика воздуха
    floattempair,            // температура воздуха float
    airTempRele_enabled,     // включение реле температуры воздуха
    airRele_startTemp,       // температура включения реле температуры воздуха
    airRele_TempThreshold,   // порог отключения реле температуры воздуха
    airTempRele_led,         // светодиод реле температуры воздуха
    
    airHumeName,             // имя датчика влажности воздуха
    floathumeair,            // влажность воздуха float
    airHumeRele_enabled,     // включение реле влажности воздуха
    airRele_startHume,       // влажность включения реле влажности воздуха
    airRele_HumeTreshold,    // порог отключения реле влажности воздуха
    airHumeRele_led,         // светодиод реле влажности воздуха

    soilTempName,            // имя датчика температуры почвы
    floattempsoil,           // температура почвы float
    soilTempRele_enabled,    // включение реле температуры почвы
    soilRele_startTemp,      // включения реле температуры почвы
    soilRele_TempThreshold,  // порог отключения реле температуры почвы
    soilTempRele_led,        // светодиод реле температуры почвы

    soilHumeName,            // имя датчика влажности почвы
    floathumsoil,           // влажность почвы float
    soilHumeRele_enabled,    // включение реле влажности почвы
    soilRele_startHume,      // включения реле влажности почвы
    soilRele_HumeTreshold,  // порог отключения реле влажности почвы
    soilHumeRele_led,        // светодиод реле влажности почвы

    soil2TempName,           // имя датчика температуры почвы 2
    floattempsoil2,          // температура почвы 2 float
    soil2TempRele_enabled,   // включение реле температуры почвы 2
    soil2Rele_startTemp,     // включения реле температуры почвы 2
    soil2Rele_TempThreshold, // порог отключения реле температуры почвы 2
    soil2TempRele_led,       // светодиод реле температуры почвы 2

    soil2HumeName,           // имя датчика влажности почвы 2
    floathumsoil2,           // влажность почвы 2 float
    soil2HumeRele_enabled,   // включение реле влажности почвы 2
    soil2Rele_startHume,     // включения реле влажности почвы 2
    soil2Rele_HumeTreshold,  // порог отключения реле влажности почвы 2
    soil2HumeRele_led,       // светодиод реле влажности почвы 2

    t1Discr_name,            // имя дискриминатора 1
    t1Discr_led,             // светодиод дискриминатора 1
    t1Discr_enabled,         // включение дискриминатора 1
    t1Discr_startTime,       // время включения дискриминатора 1
    t1Discr_endTime,         // время отключения дискриминатора 1

    t6Discr_name,            // имя дискриминатора 6
    t6Discr_led,             // светодиод дискриминатора 6
    t6Discr_enabled,         // включение дискриминатора 6
    t6Discr_startTime,       // время включения дискриминатора 6
    t6Discr_endTime,         // время отключения дискриминатора 6
    t6Discr_inMonday,        // включение дискриминатора 6 в понедельник
    t6Discr_inTuesday,       // включение дискриминатора 6 во вторник
    t6Discr_inWensday,       // включение дискриминатора 6 в среду
    t6Discr_inThursday,      // включение дискриминатора 6 в четверг
    t6Discr_inFriday,        // включение дискриминатора 6 в пятницу    
    t6Discr_inSaturday,      // включение дискриминатора 6 в субботу
    t6Discr_inSunday,        // включение дискриминатора 6 в воскресенье
    t6Discr_algorithm,       // алгоритм работы реле 6 (0-время, 1-темп.воздуха, 2-влаж.воздуха, 3-влаж.почвы1, 4-влаж.почвы2)
    t6Discr_temp_threshold,  // порог температуры для реле 6
    t6Discr_hum_threshold,   // порог влажности для реле 6
    t6Discr_hysteresis,      // гистерезис для реле 6

    date,                    // дата
    datime,                  // дата и время
    nowData,                 // текущая дата
    nowTime,                 // текущее время
    btn1,                    // кнопка 1
    btn2,                    // кнопка 2
    old_address,             // текущий адрес реле
    new_address,             // новый адрес реле
    relay_count,             // количество реле
    relay_seach              // найденные реле
);
    

extern GyverDBFile db;       // база данных для хранения настроек будет автоматически записываться в файл при изменениях 
extern SettingsGyver sett;   // настройки для web интерфейса

extern Datime curDataTime;   // текущая дата и время
extern GyverDS3231 rtc;      // часы реального времени

void build(sets::Builder &b);          // функция для построения интерфейса
//void NewFunction(sets::Builder &b);
void update(sets::Updater &upd);       // функция для обновления интерфейса 

// Функция для измерения времени выполнения
void measureExecutionTime(const char* functionName, void (*func)());
