// ДАННЫЙ ПРИМЕР ПОСТОЯННО МЕНЯЕТ СИГНАЛ ШИМ НА 3 КАНАЛЕ МОДУЛЯ:    // * Строки со звёздочкой являются необязательными.
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
double val = 2;                                                     //   Определяем начальное аналоговое значение.
bool   flg = 1;                                                     //   Определяем флаг приращения аналогового значения (0-убывает, 1-растёт).
                                                                    //
void setup(){                                                       //
//  Готовим модуль к работе:                                        //
    pwrkey.begin(&Wire); // &Wire1, &Wire2 ...                      //   Инициируем работу с модулем, указав ссылку на объект для работы с шиной I2C на которой находится модуль (по умолчанию &Wire).
    pwrkey.analogWrite(ALL_CHANNEL, 0);                             // * Отключаем все каналы.
}                                                                   //
                                                                    //
void loop(){                                                        //
    delay(10);                                                      //   Чем выше задержка, тем плавнее меняется аналоговый уровень.
    if(flg      ){val*=1.05;}else{val/=1.05;}                       //   Увеличиваем или уменьшаем аналоговое значение.
    if(val<=2   ){val = 2;    flg=1;}                               //   Меняем спад аналогового уровня на рост.
    if(val>=4095){val = 4095; flg=0;}                               //   Меняем рост аналогового уровня на спад.
                                                                    //
    pwrkey.analogWrite(3, val);                                     //   Устанавливаем на 3 канале модуля ШИМ с уровнем «val».
}                                                                   //   Допустимые значения ШИМ - от 0 до 4095.