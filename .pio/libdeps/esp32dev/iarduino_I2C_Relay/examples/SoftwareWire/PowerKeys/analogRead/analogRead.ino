// ДАННЫЙ ПРИМЕР СЧИТЫВАЕТ ТЕКУЩЕЕ СОСТОЯНИЕ ШИМ НА КАНАЛАХ МОДУЛЯ: // * Строки со звёздочкой являются необязательными.
// Требуется установить библиотеку <iarduino_I2C_Software.h>        //   https://iarduino.ru/file/627.html
                                                                    //
// Модуль силовых ключей 4N-канала 10A FLASH-I2C без измерения тока://   https://iarduino.ru/shop/Expansion-payments/power-key-4n-i2c.html
// Информация о модуле и описание библиотеки:                       //   https://wiki.iarduino.ru/page/power-key-4n-i2c/
                                                                    //
// Модуль силовых ключей 4P-канала 10A FLASH-I2C без измерения тока://   https://iarduino.ru/shop/Expansion-payments/power-key-4p-i2c.html
// Информация о модуле и описание библиотеки:                       //   https://wiki.iarduino.ru/page/power-key-4p-i2c/
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
//  Устанавливаем ШИМ на каналах модуля:                            //
    pwrkey.analogWrite(1, 0x0000);                                  //   Устанавливаем ШИМ на 1 канале в значение 0x0000 = 0%.
    pwrkey.analogWrite(2, 0x0555);                                  //   Устанавливаем ШИМ на 2 канале в значение 0x0555 = 33.3%.
    pwrkey.analogWrite(3, 0x0AAA);                                  //   Устанавливаем ШИМ на 3 канале в значение 0x0AAA = 66.6%.
    pwrkey.analogWrite(4, 0x0FFF);                                  //   Устанавливаем ШИМ на 4 канале в значение 0x0FFF = 100%.
//  Проверяем состояние каналов модуля в цикле:                     //
    for(int i=1;i<=4;i++){Serial.print  ("ШИМ на канале ");         //   Проходим по всем каналам модуля.
                          Serial.print  (i);                        //   Выводим номер очередного канала.
                          Serial.print  (" имеет значение 0x");     //   Проходим по всем каналам модуля.
                          Serial.println(pwrkey.analogRead(i),HEX); //   Выводим значение которое вернула функция analogRead().
    }                                                               //
}                                                                   //
                                                                    //
void loop(){                                                        //
}                                                                   //