#pragma once
#include <Arduino.h>
#include <iarduino_I2C_Relay.h>  


 

extern iarduino_I2C_Relay pwrkey;                                    //   Объявляем объект pwrkey для работы с функциями и методами библиотеки iarduino_I2C_Relay, указывая адрес модуля на шине I2C.
extern iarduino_I2C_Relay pwrkey1;                                    //   Объявляем объект pwrkey для работы с функциями и методами библиотеки iarduino_I2C_Relay, указывая адрес модуля на шине I2C.
void init_reley();
void reley_on();
void reley_off();
