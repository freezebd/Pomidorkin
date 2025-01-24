//#pragma once
#include <Arduino.h>

#include <Wire.h>                                                   //   Подключаем библиотеку для работы с аппаратной шиной I2C, до подключения библиотеки iarduino_I2C_Relay.
#include <iarduino_I2C_Relay.h>  
#include "reley.h"                                   //   Подключаем библиотеку для работы с реле и силовыми ключами.



iarduino_I2C_Relay pwrkey(0x09);                                    //   Объявляем объект pwrkey для работы с функциями и методами библиотеки iarduino_I2C_Relay, указывая адрес модуля на шине I2C.
iarduino_I2C_Relay pwrkey1(0x08);                                    //   Объявляем объект pwrkey для работы с функциями и методами библиотеки iarduino_I2C_Relay, указывая адрес модуля на шине I2C.
                                                                    //   Если объявить объект без указания адреса (iarduino_I2C_Relay pwrkey;), то адрес будет найден автоматически.
void init_reley(){                                                       //
                                                        // * Ждём завершение переходных процессов связанных с подачей питания.
//  Готовим модуль к работе:                                        //
    pwrkey.begin(&Wire); // &Wire1, &Wire2 ...                      //   Инициируем работу с модулем, указав ссылку на объект для работы с шиной I2C на которой находится модуль (по умолчанию &Wire).
    pwrkey.digitalWrite(ALL_CHANNEL,LOW);                           // * Выключаем все каналы модуля.
    pwrkey1.begin(&Wire); // &Wire1, &Wire2 ...                      //   Инициируем работу с модулем, указав ссылку на объект для работы с шиной I2C на которой находится модуль (по умолчанию &Wire).
    pwrkey1.digitalWrite(ALL_CHANNEL,LOW);                           // * Выключаем все каналы модуля.

}                                                                   //
                                                                    //
 void reley_on(){                                                        //
// //  Включаем и выключаем каналы модуля:                             //
     pwrkey.digitalWrite(1,HIGH); 
     //pwrkey1.digitalWrite(2,LOW);        //   Включаем 1 канал и выключаем 4.
//     delay(500);                                                     //   Ждём   500 мс.
//     pwrkey.digitalWrite(2,HIGH); pwrkey.digitalWrite(1,LOW);        //   Включаем 2 канал и выключаем 1.
//     delay(500);                                                     //   Ждём   500 мс.
//     pwrkey1.digitalWrite(2,HIGH); pwrkey.digitalWrite(2,LOW);        //   Включаем 3 канал и выключаем 2.
//     delay(500);                                                     //   Ждём   500 мс.
//     pwrkey.digitalWrite(4,HIGH); pwrkey.digitalWrite(3,LOW);        //   Включаем 4 канал и выключаем 3.
//     delay(500);                                                     //   Ждём   500 мс.
 }       
 void reley_off(){
    pwrkey.digitalWrite(1,LOW);

 }                     