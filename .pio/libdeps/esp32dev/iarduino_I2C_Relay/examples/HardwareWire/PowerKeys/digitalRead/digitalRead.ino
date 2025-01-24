// ДАННЫЙ ПРИМЕР СЧИТЫВАЕТ ТЕКУЩЕЕ СОСТОЯНИЕ КАНАЛОВ МОДУЛЯ:        // * Строки со звёздочкой являются необязательными.
                                                                    //
// Модуль силовых ключей 4N-канала 10A FLASH-I2C без измерения тока://   https://iarduino.ru/shop/Expansion-payments/power-key-4n-i2c.html
// Информация о модуле и описание библиотеки:                       //   https://wiki.iarduino.ru/page/power-key-4n-i2c/
                                                                    //
// Модуль силовых ключей 4P-канала 10A FLASH-I2C без измерения тока://   https://iarduino.ru/shop/Expansion-payments/power-key-4p-i2c.html
// Информация о модуле и описание библиотеки:                       //   https://wiki.iarduino.ru/page/power-key-4p-i2c/
                                                                    //
// Информация о подключении модулей к шине I2C:                     //   https://wiki.iarduino.ru/page/i2c_connection/
                                                                    //
#include <Wire.h>                                                   //   Подключаем библиотеку для работы с аппаратной шиной I2C, до подключения библиотеки iarduino_I2C_Relay.
#include <iarduino_I2C_Relay.h>                                     //   Подключаем библиотеку для работы с реле и силовыми ключами.
iarduino_I2C_Relay pwrkey(0x09);                                    //   Объявляем объект pwrkey для работы с функциями и методами библиотеки iarduino_I2C_Relay, указывая адрес модуля на шине I2C.
                                                                    //   Если объявить объект без указания адреса (iarduino_I2C_Relay pwrkey;), то адрес будет найден автоматически.
void setup(){                                                       //
    Serial.begin(9600);                                             //
    while(!Serial){;}                                               // * Ждём завершения инициализации шины UART.
    delay(500);                                                     // * Ждём завершение переходных процессов связанных с подачей питания.
//  Готовим модуль к работе:                                        //
    pwrkey.begin(&Wire); // &Wire1, &Wire2 ...                      //   Инициируем работу с модулем, указав ссылку на объект для работы с шиной I2C на которой находится модуль (по умолчанию &Wire).
//  Включаем и выключаем каналы модуля:                             //
    pwrkey.digitalWrite(1, LOW);                                    //   Отключаем 1 канал.
    pwrkey.digitalWrite(2,HIGH);                                    //   Включаем  2 канал.
    pwrkey.digitalWrite(3, LOW);                                    //   Отключаем 3 канал.
    pwrkey.digitalWrite(4,HIGH);                                    //   Включаем  4 канал.
//  Проверяем состояние каналов модуля в цикле:                     //
    for(int i=1; i<=4; i++){        Serial.print  ("Канал N ");     //   Проходим по всем каналам модуля.
                                    Serial.print  (i);              //   Выводим номер очередного канала.
        if( pwrkey.digitalRead(i) ){ Serial.println(" включен "); } //   Если функция digitalRead() вернула HIGH значит канал включён.
        else                      { Serial.println(" отключен"); }  //   Если функция digitalRead() вернула LOW  значит канал отключён.
    }                               Serial.println("---------");    //
}                                                                   //
                                                                    //
void loop(){                                                        //
}                                                                   //
//  ПРИМЕЧАНИЕ: состояние всех каналов можно получить одним байтом: //
//  uint8_t j = pwrkey.digitalRead(ALL_CHANNEL);                    //   4 младших бита переменной «j» соответствуют состояниям 4 каналов модуля.
