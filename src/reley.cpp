//#pragma once
#include <Arduino.h>

#include <Wire.h>                                                   //   Подключаем библиотеку для работы с аппаратной шиной I2C, до подключения библиотеки iarduino_I2C_Relay.
#include <iarduino_I2C_Relay.h>  
#include "reley.h"                                                 




iarduino_I2C_Relay rele1(0x09);                                     //   Объявляем объект rele для работы с функциями и методами библиотеки iarduino_I2C_Relay, указывая адрес модуля на шине I2C.
iarduino_I2C_Relay rele2(0x08);                                    //   Объявляем объект rele для работы с функциями и методами библиотеки iarduino_I2C_Relay, указывая адрес модуля на шине I2C.

void init_reley(){                                                       
    rele1.begin(&Wire); // &Wire1, &Wire2 ...                       //   Инициируем работу с модулем, указав ссылку на объект для работы с шиной I2C на которой находится модуль (по умолчанию &Wire).
    rele1.digitalWrite(ALL_CHANNEL,LOW);                            // * Выключаем все каналы модуля.
    rele2.begin(&Wire); // &Wire1, &Wire2 ...                      //   Инициируем работу с модулем, указав ссылку на объект для работы с шиной I2C на которой находится модуль (по умолчанию &Wire).
    rele2.digitalWrite(ALL_CHANNEL,LOW);                           // * Выключаем все каналы модуля.
}                                                                   

 void reley_Soil_on(){                                                      
   rele1.digitalWrite(1,HIGH); 
 }       
 void reley_Soil_off(){
   rele1.digitalWrite(1,LOW);
 }  
 void reley_Air_on(){
   rele1.digitalWrite(2,HIGH);
 } 
 void reley_Air_off(){
   rele1.digitalWrite(2,LOW);
 }
 void reley_1_on(){
   rele2.digitalWrite(1,HIGH);
 }    
 void reley_1_off(){
   rele2.digitalWrite(1,LOW);
 }
 void reley_2_on(){
   rele2.digitalWrite(2,HIGH);
 }    
 void reley_2_off(){
   rele2.digitalWrite(2,LOW);
 } 
                 

