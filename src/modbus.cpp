#include "modbus.h"
#include "data.h"

ModbusClient modbus(Serial1);

bool sens_alert = false;

#define SENSOR_ID_SOIL 0x01  // Адрес датчика почвы
#define SENSOR_ID_AIR  0x02  // Адрес датчика воздуха

#define REG_SOIL_TEMP  0x01  // Температура почвы
#define REG_SOIL_HUM   0x00  // Влажность почвы
#define REG_AIR_TEMP   0x01  // Температура воздуха
#define REG_AIR_HUM    0x00  // Влажность воздуха

// Пины для подключения RS485
#define RX_PIN 16
#define TX_PIN 17

void init_modbus() {// Чтение данных с датчика воздуха (адрес 2)
   Serial1.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);// Настройка HardwareSerial для связи с RS485
    modbus.setTimeout( 15 ); // Указываем жать ответ от модулей не более 15 мс.
    modbus.setDelay( 5 ); // Указываем выдерживать паузу между пакетами в 5 мс.
    Serial.println("Modbus RTU Master Initialized");
}
void readSensorAir() {
    int16_t result;
    
    // Чтение влажности
    result = modbus.holdingRegisterRead(SENSOR_ID_AIR, REG_AIR_HUM);
    if (result < 0) {
        sens_alert = true;
        data.Air1.hfloat = -80;
        Serial.println("Air humidity read error");
    } else {
        data.Air1.hx10 = result;
        data.Air1.hfloat = (data.Air1.hx10 / 10.0);
    }
    
    delay(10); // Небольшая задержка между запросами
    
    // Чтение температуры
    result = modbus.holdingRegisterRead(SENSOR_ID_AIR, REG_AIR_TEMP);
    if (result < 0) {
        sens_alert = true;
        data.Air1.tfloat = -80;
        Serial.println("Air temperature read error");
    } else {
        data.Air1.tx10 = result;
        data.Air1.tfloat = (data.Air1.tx10 / 10.0);
    }
}
void readSensorSoil() {
    int16_t result;
    
    // Чтение влажности
    result = modbus.holdingRegisterRead(SENSOR_ID_SOIL, REG_SOIL_HUM);
    if (result < 0) {
        sens_alert = true;
        data.Soil1.hfloat = -80;
        Serial.println("Soil humidity read error");
    } else {
        data.Soil1.hx10 = result;
        data.Soil1.hfloat = (data.Soil1.hx10 / 10.0);
    }
    
    delay(10); // Небольшая задержка между запросами
    
    // Чтение температуры
    result = modbus.holdingRegisterRead(SENSOR_ID_SOIL, REG_SOIL_TEMP);
    if (result < 0) {
        sens_alert = true;
        data.Soil1.tfloat = -80;
        Serial.println("Soil temperature read error");
    } else {
        data.Soil1.tx10 = result;
        data.Soil1.tfloat = (data.Soil1.tx10 / 10.0);
    }
} 

