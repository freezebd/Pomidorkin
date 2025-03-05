// ДАННЫЙ ПРИМЕР СЧИТЫВАЕТ ТЕКУЩЕЕ СОСТОЯНИЕ КАНАЛОВ МОДУЛЯ:        // * Строки со звёздочкой являются необязательными.
// Требуется установить библиотеку <iarduino_I2C_Software.h>        //   https://iarduino.ru/file/627.html
                                                                    //
// Модуль силовых ключей 4N-канала 2А FLASH-I2C с измерением тока:  //   https://iarduino.ru/shop/Expansion-payments/power-key-4n-a-i2c.html
// Информация о модуле и описание библиотеки:                       //   https://wiki.iarduino.ru/page/power-key-4n-a-i2c/
                                                                    //
// Модуль силовых ключей 4P-канала 2А FLASH-I2C с измерением тока:  //   https://iarduino.ru/shop/Expansion-payments/power-key-4p-a-i2c.html
// Информация о модуле и описание библиотеки:                       //   https://wiki.iarduino.ru/page/power-key-4p-a-i2c/
                                                                    //
// Информация о подключении модулей к шине I2C:                     //   https://wiki.iarduino.ru/page/i2c_connection/
                                                                    //
#include <iarduino_I2C_Software.h>                                  //   Подключаем библиотеку для работы с программной шиной I2C, до подключения библиотеки iarduino_I2C_Relay.
SoftTwoWire sWire(3,4);                                             //   Создаём объект программной шины I2C указав выводы которым будет назначена роль линий: SDA, SCL.
                                                                    //
#include <iarduino_I2C_Relay.h>                                     //   Подключаем библиотеку для работы с реле и силовыми ключами.
iarduino_I2C_Relay pwrkey(0x09);                                    //   Объявляем объект pwrkey для работы с функциями и методами библиотеки iarduino_I2C_Relay, указывая адрес модуля на шине I2C.
                                                                    //   Если объявить объект без указания адреса (iarduino_I2C_Relay pwrkey;), то адрес будет найден автоматически.
void setup(){                                                       //
    Serial.begin(9600);                                             //
    while(!Serial){;}                                               // * Ждём завершения инициализации шины UART.
    delay(500);                                                     // * Ждём завершение переходных процессов связанных с подачей питания.
//  Готовим модуль к работе:                                        //
    pwrkey.begin(&sWire);                                           //   Инициируем работу с модулем, указав ссылку на объект для работы с шиной I2C на которой находится модуль (по умолчанию &Wire).
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
