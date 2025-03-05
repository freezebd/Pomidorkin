// ДАННЫЙ ПРИМЕР ПООЧЕРЁДНО ВКЛЮЧАЕТ И ВЫКЛЮЧАЕТ КАНАЛЫ МОДУЛЯ:     // * Строки со звёздочкой являются необязательными.
                                                                    //
// Модуль силовых ключей 4N-канала 2А FLASH-I2C с измерением тока:  //   https://iarduino.ru/shop/Expansion-payments/power-key-4n-a-i2c.html
// Информация о модуле и описание библиотеки:                       //   https://wiki.iarduino.ru/page/power-key-4n-a-i2c/
                                                                    //
// Модуль силовых ключей 4P-канала 2А FLASH-I2C с измерением тока:  //   https://iarduino.ru/shop/Expansion-payments/power-key-4p-a-i2c.html
// Информация о модуле и описание библиотеки:                       //   https://wiki.iarduino.ru/page/power-key-4p-a-i2c/
                                                                    //
// Информация о подключении модулей к шине I2C:                     //   https://wiki.iarduino.ru/page/i2c_connection/
                                                                    //
#include <Wire.h>                                                   //   Подключаем библиотеку для работы с аппаратной шиной I2C, до подключения библиотеки iarduino_I2C_Relay.
#include <iarduino_I2C_Relay.h>                                     //   Подключаем библиотеку для работы с реле и силовыми ключами.
iarduino_I2C_Relay pwrkey(0x09);                                    //   Объявляем объект pwrkey для работы с функциями и методами библиотеки iarduino_I2C_Relay, указывая адрес модуля на шине I2C.
                                                                    //   Если объявить объект без указания адреса (iarduino_I2C_Relay pwrkey;), то адрес будет найден автоматически.
void setup(){                                                       //
    delay(500);                                                     // * Ждём завершение переходных процессов связанных с подачей питания.
//  Готовим модуль к работе:                                        //
    pwrkey.begin(&Wire); // &Wire1, &Wire2 ...                      //   Инициируем работу с модулем, указав ссылку на объект для работы с шиной I2C на которой находится модуль (по умолчанию &Wire).
    pwrkey.digitalWrite(ALL_CHANNEL,LOW);                           // * Выключаем все каналы модуля.
}                                                                   //
                                                                    //
void loop(){                                                        //
//  Включаем и выключаем каналы модуля:                             //
    pwrkey.digitalWrite(1,HIGH); pwrkey.digitalWrite(4,LOW);        //   Включаем 1 канал и выключаем 4.
    delay(500);                                                     //   Ждём   500 мс.
    pwrkey.digitalWrite(2,HIGH); pwrkey.digitalWrite(1,LOW);        //   Включаем 2 канал и выключаем 1.
    delay(500);                                                     //   Ждём   500 мс.
    pwrkey.digitalWrite(3,HIGH); pwrkey.digitalWrite(2,LOW);        //   Включаем 3 канал и выключаем 2.
    delay(500);                                                     //   Ждём   500 мс.
    pwrkey.digitalWrite(4,HIGH); pwrkey.digitalWrite(3,LOW);        //   Включаем 4 канал и выключаем 3.
    delay(500);                                                     //   Ждём   500 мс.
}                                                                   //