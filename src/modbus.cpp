#include "modbus.h"
#include "data.h"

// uint8_t nowAddress = 3;                     // Текущий адрес блока климатических датчиков ( 1 - 247 ).
// uint8_t newAddress = 4;                     // Новый адрес блока климатических датчиков ( 1 - 247 ).
uint8_t pinDE      = 18;                     // Вывод DE конвертера UART-RS485.
ModbusClient modbus(Serial1, pinDE);
iarduino_MB_HTL sensor(modbus);

bool sens_alert = false;

#define SENSOR_ID_SOIL 0x01  // Адрес датчика почвы
//#define SENSOR_ID_AIR  0x02  // Адрес датчика воздуха
#define SENSOR_ID_SOIL2 0x03  // Адрес датчика почвы 2

#define REG_SOIL_TEMP  0x01  // Температура почвы
#define REG_SOIL_HUM   0x00  // Влажность почвы
// #define REG_AIR_TEMP   0x01  // Температура воздуха
// #define REG_AIR_HUM    0x00  // Влажность воздуха
#define REG_SOIL2_TEMP  0x01  // Температура почвы 2
#define REG_SOIL2_HUM   0x00  // Влажность почвы 2

// Пины для подключения RS485
#define RX_PIN 16
#define TX_PIN 17

void init_modbus() {// Инициализация Modbus
   Serial1.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);// Настройка HardwareSerial для связи с RS485
    modbus.begin();           //   Инициируем работу по протоколу Modbus.
    modbus.setTimeout( 25 ); // Указываем жать ответ от модулей не более 25 мс.
    modbus.setDelay( 10 ); // Указываем выдерживать паузу между пакетами в 10 мс.
    Serial.println("Modbus RTU Master Initialized"); 
}
void init_modbus_air() {// Инициализация Modbus для датчика воздуха
Serial.print("Инициализация "); 
     if (sensor.begin(4)){
        Serial.println("Датчик воздуха готов к работе");      
     Serial.print("Текущий адрес ");        //
     Serial.println(sensor.getID());        // Выводим текущий адрес блока климатических датчиков.
     Serial.print("Версия прошивки ");      //
     Serial.println(sensor.getVersion());
     }
     else {
        Serial.println("Датчик воздуха не найден");
     }
}

void readSensorAir() {
    int16_t result;
    
    // Чтение влажности
     result = sensor.getHUM() / 0.1;
     if (result < 0) {
         sens_alert = true;
         data.Air1.hfloat = -80;
     } else {
         data.Air1.hx10 = result;
         data.Air1.hfloat = (data.Air1.hx10 / 10.0);
     }
    
    delay(10); // Небольшая задержка между запросами
    
    // Чтение температуры
    result = sensor.getTEM() / 0.1;//modbus.holdingRegisterRead(SENSOR_ID_AIR, REG_AIR_TEMP);
    if (result < 0) {
        sens_alert = true;
        data.Air1.tfloat = -80;
        } else {
        data.Air1.tx10 = result;
        data.Air1.tfloat = (data.Air1.tx10 / 10.0);
        Serial.println(data.Air1.tx10);
    }
}
void readSensorSoil() {
    int16_t result;
    
    // Чтение влажности
    result = modbus.holdingRegisterRead(SENSOR_ID_SOIL, REG_SOIL_HUM);
    if (result < 0) {
        sens_alert = true;
        data.Soil1.hfloat = -80;
        // Serial.println("Soil humidity read error");
        // ESP_LOGE("Modbus", "Soil humidity read error");
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
        // Serial.println("Soil temperature read error");
        // ESP_LOGE("Modbus", "Soil temperature read error");
    } else {
        data.Soil1.tx10 = result;
        data.Soil1.tfloat = (data.Soil1.tx10 / 10.0);
    }
}
void readSensorSoil2() {
    int16_t result;
    
    // Чтение температуры
    result = modbus.holdingRegisterRead(SENSOR_ID_SOIL2, REG_SOIL2_TEMP);
    if (result < 0) {
        sens_alert = true;
        data.Soil2.tfloat = -80;
        // Serial.println("Soil2 temperature read error");
        // ESP_LOGE("Modbus", "Soil2 temperature read error");
    } else {
        data.Soil2.tx10 = result;
        data.Soil2.tfloat = (data.Soil2.tx10 / 10.0);
    }
    
    delay(10); // Небольшая задержка между запросами
    
    // Чтение влажности
    result = modbus.holdingRegisterRead(SENSOR_ID_SOIL2, REG_SOIL2_HUM);
    if (result < 0) {
        sens_alert = true;
        data.Soil2.hfloat = -80;
        // Serial.println("Soil2 humidity read error");
        // ESP_LOGE("Modbus", "Soil2 humidity read error");
    } else {
        data.Soil2.hx10 = result;
        data.Soil2.hfloat = (data.Soil2.hx10 / 10.0);
    }
} 
