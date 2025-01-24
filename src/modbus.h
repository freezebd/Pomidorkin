#pragma once
#include <Arduino.h>
#include <data.h>
#include <ModbusRTU.h>
#include <HardwareSerial.h>
#include "data.h"

// Адреса устройств
#define AIR_SENSOR_ADDR 2     // Датчик температуры и влажности воздуха
#define SOIL_SENSOR_ADDR 1    // Датчик температуры и влажности почвы

// Пины для подключения RS485
#define RX_PIN 16
#define TX_PIN 17

// Создаем объект ModbusRTU
ModbusRTU mb;
HardwareSerial mySerial(1);  // Используем второй аппаратный сериал

// Регистры и количество регистров
#define FIRST_REG 0x0000
#define REG_COUNT 2

// Переменные для хранения данных с датчиков
uint16_t airSensorData[REG_COUNT];
uint16_t soilSensorData[REG_COUNT];

// Callback для обработки ошибок
//bool cb(Modbus::ResultCode event, uint16_t transactionId, void* data) {
 // if (event != Modbus::EX_SUCCESS) {
  //  Serial.print("Request result: 0x");
  //  Serial.println(event, HEX);
 // }
 // return true;
//}


void init_modbus() {
  mySerial.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);  // Настройка HardwareSerial для связи с RS485
  mb.begin(&mySerial); // Инициализация библиотеки ModbusRTU
  mb.master();         // Устанавливаем режим мастер
  Serial.println("Modbus RTU Master Initialized");
}
void riadSensorAir() {
  if (!mb.slave()) {    // Проверяем, не идет ли в данный момент транзакция
    // Чтение данных с датчика воздуха (адрес 2)
    mb.readHreg(AIR_SENSOR_ADDR, FIRST_REG, airSensorData, REG_COUNT);//, cb);
    //while (mb.slave()) {  // Ожидаем завершения транзакции
      unsigned long start = millis();
   while (mb.slave()) {
      if (millis() - start > 500) 
      break;
      
      mb.task();
      data.Air1.AirTemp = (airSensorData[1] / 10.0);
      data.Air1.AirHum = (airSensorData[0] / 10.0);
      delay(10);  // Небольшая задержка, чтобы не блокировать выполнение
    }
  
  // Печать данных на Serial Monitor
    // Serial.print("Air Temperature: ");
    // Serial.print(data.Air1.AirTemp);  // Преобразуем данные для корректного отображения
    // Serial.print(" °C, Air Humidity: ");
    // Serial.println(data.Air1.AirHum); // Преобразуем данные для корректного отображения

  }
}



void riadSensorSoil() {
  if (!mb.slave()) {    // Проверяем, не идет ли в данный момент транзакция
    mb.readHreg(SOIL_SENSOR_ADDR, FIRST_REG, soilSensorData, REG_COUNT);//, cb);// Чтение данных с датчика воздуха (адрес 2)
   // while (mb.slave()) {  // Ожидаем завершения транзакции
   //   mb.task();
   unsigned long start = millis();
   while (mb.slave()) {
     if (millis() - start > 500) 
     break; // Прерываем, если время ожидания превышает 500 мс
     mb.task();
      data.Soil1.SoilTemp = (soilSensorData[1] / 10.0);
      data.Soil1.SoilHum = (soilSensorData[0] / 10.0);
      delay(10);  // Небольшая задержка, чтобы не блокировать выполнение
    } 
  }
  // Serial.print("Soil Temperature: ");
  //   Serial.print(data.Soil1.SoilTemp);  // Преобразуем данные для корректного отображения
  //   Serial.print(" °C, Soil Humidity: ");
  //   Serial.println(data.Soil1.SoilHum); // Преобразуем данные для корректного отображения
}
