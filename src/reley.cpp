//#pragma once
#include <Arduino.h>

#include <Wire.h>                                                   //   Подключаем библиотеку для работы с аппаратной шиной I2C, до подключения библиотеки iarduino_I2C_Relay.
#include <iarduino_I2C_Relay.h>  
#include "reley.h"                                                 
#include "data.h"
#include "settings.h"  // Добавляем для доступа к sett



iarduino_I2C_Relay rele1(0x09);      //   Объявляем объект rele для работы с функциями и методами библиотеки iarduino_I2C_Relay, указывая адрес модуля на шине I2C.
iarduino_I2C_Relay rele2(0x08);      //   Объявляем объект rele для работы с функциями и методами библиотеки iarduino_I2C_Relay, указывая адрес модуля на шине I2C.
iarduino_I2C_Relay rele3(0x0A);      //   Объявляем объект rele для работы с функциями и методами библиотеки iarduino_I2C_Relay, указывая адрес модуля на шине I2C.

// Кэшируем состояния реле
static uint8_t rele1_state = 0; // Свет / Влажность воздуха
static uint8_t rele2_state = 0; // 
static uint8_t rele3_state = 0; // 

void init_reley(){                                                       
    rele1.begin(&Wire); // &Wire1, &Wire2 ...                       //   Инициируем работу с модулем, указав ссылку на объект для работы с шиной I2C на которой находится модуль (по умолчанию &Wire).
    rele1.digitalWrite(ALL_CHANNEL,LOW);                            // * Выключаем все каналы модуля.
    rele2.begin(&Wire); // &Wire1, &Wire2 ...                      //   Инициируем работу с модулем, указав ссылку на объект для работы с шиной I2C на которой находится модуль (по умолчанию &Wire).
    rele2.digitalWrite(ALL_CHANNEL,LOW);                           // * Выключаем все каналы модуля.
    rele3.begin(&Wire); // &Wire1, &Wire2 ...                      //   Инициируем работу с модулем, указав ссылку на объект для работы с шиной I2C на которой находится модуль (по умолчанию &Wire).
    rele3.digitalWrite(ALL_CHANNEL,LOW);                           // * Выключаем все каналы модуля.
}                                                                   

 void reley_Soil_on(){        // Включить  реле температура почвы                                              
   if(!(rele1_state & 0x01)) {
       rele1.digitalWrite(1, HIGH);
       rele1_state |= 0x01;
   }
 }       
 void reley_Soil_off(){       // Выключить  реле температура почвы  
   if(rele1_state & 0x01) {
       rele1.digitalWrite(1, LOW); 
       rele1_state &= ~0x01;
   }
 }  
 void reley_Soil2_on(){       // Включить  реле температура почвы 2                                                      
   if(!(rele2_state & 0x01)) {
       rele2.digitalWrite(1, HIGH);
       rele2_state |= 0x01;
   }
 }       
 void reley_Soil2_off(){ // Выключить  реле температура почвы 2
   if(rele2_state & 0x01) {
       rele2.digitalWrite(1,LOW);
       rele2_state &= ~0x01;
   }
 }  
 void reley_Air_Temt_on(){ // Включить  реле температура воздуха
   if(!(rele1_state & 0x02)) {
       rele1.digitalWrite(2, HIGH);
       rele1_state |= 0x02;
   }
 } 
 void reley_Air_Temp_off(){
   if(rele1_state & 0x02) {
       rele1.digitalWrite(2,LOW);
       rele1_state &= ~0x02;
   }
 }
 void reley_1_on(){ 
   if(!(rele2_state & 0x01)) {
       rele2.digitalWrite(1, HIGH);
       rele2_state |= 0x01;
   }
 }    
 void reley_1_off(){ 
   if(rele2_state & 0x01) {
       rele2.digitalWrite(1,LOW);
       rele2_state &= ~0x01;
   }
 }
 void reley_2_on(){ 
   if(!(rele2_state & 0x02)) {
       rele2.digitalWrite(2, HIGH);
       rele2_state |= 0x02;
   }
 }    
 void reley_2_off(){ 
   if(rele2_state & 0x02) {
       rele2.digitalWrite(2,LOW);
       rele2_state &= ~0x02;
   }
 } 
 void reley_3_on(){ // Включить  реле 3
   if(!(rele3_state & 0x01)) {
       rele3.digitalWrite(1, HIGH);
       rele3_state |= 0x01;
   }
 }    
 void reley_3_off(){ // Выключить  реле 3
   if(rele3_state & 0x01) {
       rele3.digitalWrite(1,LOW);
       rele3_state &= ~0x01;
   }
 } 

bool change_relay_address_alert = false;
bool change_relay_address_notice = false;
bool relay_address_error = false;
bool module_address_error = false;

void change_relay_address() {    // функция для изменения адреса реле
    iarduino_I2C_Relay temp_relay(data.old_address);
    
    if(temp_relay.begin()) {
        if(data.new_address > 0 && data.new_address <= 127) {
            if(temp_relay.changeAddress(data.new_address)) {
              change_relay_address_notice = true;
            } else {
                change_relay_address_alert = true;
            }
        } else {
            relay_address_error = true;
        }
    } else {
        module_address_error = true;
    }
}

uint8_t scan_relays(RelayInfo* relays) { // функция для сканирования реле
    uint8_t count = 0;
    
    // Сканируем адреса от 0x01 до 0x7F
    for (uint8_t addr = 0x01; addr < 0x80 && count < MAX_RELAYS; addr++) {
        iarduino_I2C_Relay temp_relay(addr);
        if (temp_relay.begin()) {
            relays[count].address = addr;
            relays[count].isPresent = true;
            count++;
        }
    }
    return count;
}

