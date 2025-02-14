#pragma once // защита от повторного включения
#include <Arduino.h>
#include <iarduino_Modbus.h>
 

extern ModbusClient modbus;   // объявление

void init_modbus();          // объявление функций
void readSensorAir();
void readSensorSoil();
void readSensorSoil2();  // Добавляем объявление

extern bool sens_alert;  // Объявление глобальной переменной