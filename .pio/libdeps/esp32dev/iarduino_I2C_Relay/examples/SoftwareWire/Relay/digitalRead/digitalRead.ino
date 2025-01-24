// ДАННЫЙ ПРИМЕР СЧИТЫВАЕТ ТЕКУЩЕЕ СОСТОЯНИЕ КАНАЛОВ МОДУЛЯ:        // * Строки со звёздочкой являются необязательными.
// Требуется установить библиотеку <iarduino_I2C_Software.h>        //   https://iarduino.ru/file/627.html
                                                                    //
// Модуль электромеханических реле на 2-канала FLASH-I2C:           //   https://iarduino.ru/shop/Expansion-payments/rele-2ch-i2c.html
// Информация о модуле и описание библиотеки:                       //   https://wiki.iarduino.ru/page/rele-2ch-i2c/
                                                                    //
// Модуль твердотельных реле на 4-канала FLASH-I2C:                 //   https://iarduino.ru/shop/Expansion-payments/solid-rele-4ch-i2c.html
// Информация о модуле и описание библиотеки:                       //   https://wiki.iarduino.ru/page/solid-rele-4ch-i2c/
                                                                    //
// Информация о подключении модулей к шине I2C:                     //   https://wiki.iarduino.ru/page/i2c_connection/
                                                                    //
#include <iarduino_I2C_Software.h>                                  //   Подключаем библиотеку для работы с программной шиной I2C, до подключения библиотеки iarduino_I2C_Relay.
SoftTwoWire sWire(3,4);                                             //   Создаём объект программной шины I2C указав выводы которым будет назначена роль линий: SDA, SCL.
                                                                    //
#include <iarduino_I2C_Relay.h>                                     //   Подключаем библиотеку для работы с реле и силовыми ключами.
iarduino_I2C_Relay relay(0x09);                                     //   Объявляем объект relay для работы с функциями и методами библиотеки iarduino_I2C_Relay, указывая адрес модуля на шине I2C.
                                                                    //   Если объявить объект без указания адреса (iarduino_I2C_Relay relay;), то адрес будет найден автоматически.
void setup(){                                                       //
    Serial.begin(9600);                                             //
    while(!Serial){;}                                               // * Ждём завершения инициализации шины UART.
    delay(500);                                                     // * Ждём завершение переходных процессов связанных с подачей питания.
//  Готовим модуль к работе:                                        //
    relay.begin(&sWire);                                            //   Инициируем работу с модулем, указав ссылку на объект для работы с шиной I2C на которой находится модуль (по умолчанию &Wire).
//  Включаем и выключаем каналы модуля:                             //
    relay.digitalWrite(1, LOW);                                     //   Отключаем 1 канал.
    relay.digitalWrite(2,HIGH);                                     //   Включаем  2 канал.
    relay.digitalWrite(3, LOW);                                     //   Отключаем 3 канал.
    relay.digitalWrite(4,HIGH);                                     //   Включаем  4 канал.
//  Проверяем состояние каналов модуля в цикле:                     //
    for(int i=1; i<=4; i++){        Serial.print  ("Канал N ");     //   Проходим по всем каналам модуля.
                                    Serial.print  (i);              //   Выводим номер очередного канала.
        if( relay.digitalRead(i) ){ Serial.println(" включен "); }  //   Если функция digitalRead() вернула HIGH значит канал включён.
        else                      { Serial.println(" отключен"); }  //   Если функция digitalRead() вернула LOW  значит канал отключён.
    }                               Serial.println("---------");    //
}                                                                   //
                                                                    //
void loop(){                                                        //
}                                                                   //
//  ПРИМЕЧАНИЕ: состояние всех каналов можно получить одним байтом: //
//  uint8_t j = relay.digitalRead(ALL_CHANNEL);                     //   4 младших бита переменной «j» соответствуют состояниям 4 каналов модуля.
