#pragma once
#include <Arduino.h>
#include <GyverDBFile.h>
#include <GyverDS3231.h>
#include <SettingsGyver.h>
#include <LittleFS.h>

extern GyverDBFile db;
extern SettingsGyver sett;

DB_KEYS(
    kk,
    wifi_ssid,
    wifi_pass,
    wifi_scan,
    wifi_networks,
    wifi_selected,
    apply,
    ntp_gmt,
    ntp_gmt_set,
    secondsNow,
    secondsUptime,
    dayofweek,
    uptimeDays,
    airTempName,
    floattempair,
    airTempRele_enabled,
    airRele_startTemp,
    airRele_TempThreshold,
    airTempRele_led,
    airHumeName,
    floathumeair,
    airHumeRele_enabled,
    airRele_startHume,
    airRele_HumeTreshold,
    airHumeRele_led,
    airRele_dayTemp,
    airRele_nightTemp,
    airRele_dayStartTime,
    airRele_nightStartTime,
    airRele_tempHysteresis,
    soilTempName,
    floattempsoil,
    soilTempRele_enabled,
    soilRele_startTemp,
    soilRele_TempThreshold,
    soilTempRele_led,
    soilHumeName,
    floathumsoil,
    soilHumeRele_enabled,
    soilRele_startHume,
    soilRele_HumeTreshold,
    soilHumeRele_led,
    soil2TempName,
    floattempsoil2,
    soil2TempRele_enabled,
    soil2Rele_startTemp,
    soil2Rele_TempThreshold,
    soil2TempRele_led,
    soil2HumeName,
    floathumsoil2,
    soil2HumeRele_enabled,
    soil2Rele_startHume,
    soil2Rele_HumeTreshold,
    soil2HumeRele_led,
    t1Discr_name,
    t1Discr_led,
    t1Discr_enabled,
    t1Discr_startTime,
    t1Discr_endTime,
    t6Discr_name,
    t6Discr_led,
    t6Discr_enabled,
    t6Discr_startTime,
    t6Discr_endTime,
    t6Discr_inMonday,
    t6Discr_inTuesday,
    t6Discr_inWensday,
    t6Discr_inThursday,
    t6Discr_inFriday,
    t6Discr_inSaturday,
    t6Discr_inSunday,
    t6Discr_algorithm,
    t6Discr_temp_threshold,
    t6Discr_hum_threshold,
    t6Discr_hysteresis,
    date,
    datime,
    nowData,
    nowTime,
    btn1,
    btn2,
    old_address,
    new_address,
    relay_seach
);

extern Datime curDataTime;   // текущая дата и время
extern GyverDS3231 rtc;      // часы реального времени

void build(sets::Builder &b);          // функция для построения интерфейса
//void NewFunction(sets::Builder &b);
void update(sets::Updater &upd);       // функция для обновления интерфейса 

// Функция для измерения времени выполнения
void measureExecutionTime(const char* functionName, void (*func)());
