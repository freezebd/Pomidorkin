#pragma once // защита от повторного включения
#include <Arduino.h>
#include <iarduino_Modbus.h>
#include <iarduino_MB_HTL.h>
 

extern ModbusClient modbus;   // объявление 
extern iarduino_MB_HTL sensor; // объявление датчика климата
 

void init_modbus();          // объявление функций
void readSensorAir();
void readSensorSoil();
void readSensorSoil2();  // Добавляем объявление
void init_modbus_air();

extern bool sens_alert;  // Объявление глобальной переменной