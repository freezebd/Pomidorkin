#pragma once
#include <Arduino.h>
#include <iarduino_I2C_Relay.h>  
#include "data.h"

 

extern iarduino_I2C_Relay rele1;     //   Объявляем объект pwrkey для работы с функциями и методами библиотеки iarduino_I2C_Relay, указывая адрес модуля на шине I2C.
extern iarduino_I2C_Relay rele2;     //   Объявляем объект pwrkey для работы с функциями и методами библиотеки iarduino_I2C_Relay, указывая адрес модуля на шине I2C.
extern iarduino_I2C_Relay rele3;     //   Объявляем объект pwrkey для работы с функциями и методами библиотеки iarduino_I2C_Relay, указывая адрес модуля на шине I2C.
void init_reley();

void reley_1_1_on();
void reley_1_1_off();
void reley_1_2_on();
void reley_1_2_off();

void reley_2_1_on();
void reley_2_1_off();
void reley_2_2_on();
void reley_2_2_off();

void reley_3_1_on();
void reley_3_1_off();
void reley_3_2_on();
void reley_3_2_off();

// Добавляем объявление новой функции
void change_relay_address();        //  функция для изменения адреса реле
// Максимальное количество реле для сканирования
#define MAX_RELAYS 8

// Функция сканирования реле
uint8_t scan_relays(RelayInfo* relays);  // объявление функции сканирования реле    
extern bool change_relay_address_alert;  // объявление глобальной переменной для хранения состояния ошибки изменения адреса реле
extern bool change_relay_address_notice;  // объявление глобальной переменной для хранения состояния уведомления об изменении адреса реле
extern bool relay_address_error;  // объявление глобальной переменной для хранения состояния ошибки адреса реле
extern bool module_address_error;  // объявление глобальной переменной для хранения состояния ошибки адреса модуля реле
