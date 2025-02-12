#pragma once // защита от повторного включения
#include <Arduino.h>
//#include <HardwareSerial.h>
#include <iarduino_Modbus.h>
 

extern ModbusClient modbus;   // объявление

void init_modbus();          // объявление функций
void readSensorAir();
void readSensorSoil();

extern bool sens_alert;  // Объявление глобальной переменной