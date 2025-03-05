// ПРИМЕР СМЕНЫ АДРЕСА МОДУЛЯ:                                                                                 // * Строки со звёздочкой являются необязательными.
                                                                                                               //
// Модуль силовых ключей 4N-канала 10A FLASH-I2C без измерения тока:                                           //   https://iarduino.ru/shop/Expansion-payments/power-key-4n-i2c.html
// Информация о модуле и описание библиотеки:                                                                  //   https://wiki.iarduino.ru/page/power-key-4n-i2c/
                                                                                                               //
// Модуль силовых ключей 4P-канала 10A FLASH-I2C без измерения тока:                                           //   https://iarduino.ru/shop/Expansion-payments/power-key-4p-i2c.html
// Информация о модуле и описание библиотеки:                                                                  //   https://wiki.iarduino.ru/page/power-key-4p-i2c/
                                                                                                               //
// Информация о подключении модулей к шине I2C:                                                                //   https://wiki.iarduino.ru/page/i2c_connection/
                                                                                                               //
// Данный скетч демонстрирует не только возможность смены адреса на указанный в переменной i,                  //
// но и обнаружение, и вывод текущего адреса модуля на шине I2C.                                               //
                                                                                                               //
// Библиотека <iarduino_I2C_Relay.h> предназначена для работы с реле и силовыми ключами I2C-flash              //
// и меняет только их адреса. Но, в разделе Файл/Примеры/.../findSortDevices находятся примеры смены адресов и //
// сортировки адресов любых модулей iarduino серии Flash I2C.                                                  //
                                                                                                               //
uint8_t i = 0x09;                                                                                              //   Назначаемый модулю новый адрес (0x07 < адрес < 0x7F).
                                                                                                               //
#include <Wire.h>                                                                                              //   Подключаем библиотеку для работы с аппаратной шиной I2C, до подключения библиотеки iarduino_I2C_Relay.
#include <iarduino_I2C_Relay.h>                                                                                //   Подключаем библиотеку для работы с реле и силовыми ключами.
iarduino_I2C_Relay pwrkey;                                                                                     //   Объявляем объект pwrkey для работы с функциями и методами библиотеки iarduino_I2C_Relay.
                                                                                                               //   Если при объявлении объекта указать адрес, например, pwrkey(0xBB), то пример будет работать с тем модулем, адрес которого был указан.
void setup(){                                                                                                  //
    Serial.begin(9600);                                                                                        //
    while(!Serial){;}                                                                                          // * Ждём завершения инициализации шины UART.
    delay(500);                     Serial.print( "На шине I2C " );                                            //
    if( pwrkey.begin(&Wire) ){      Serial.print( "найден модуль с адресом 0x" );                              //   Инициируем работу с модулем, указав ссылку на объект для работы с шиной I2C на которой находится модуль (Wire, Wire1, Wire2 ...).
                                    Serial.print( pwrkey.getAddress(), HEX );                                  //   Выводим текущий адрес модуля.
                                    Serial.print( ", который является " );                                     //
        switch( pwrkey.getModel() ){                                                                           //   Сравниваем модель модуля с константами:
            case DEF_MODEL_2RM:     Serial.print( "электромеханическим реле на 2-канала" );           break;   //
            case DEF_MODEL_4RT:     Serial.print( "твердотельным реле на 4-канала" );                 break;   //
            case DEF_MODEL_4NC:     Serial.print( "силовым ключом на 4 N-канала с измерением тока" ); break;   //
            case DEF_MODEL_4PC:     Serial.print( "силовым ключом на 4 P-канала с измерением тока" ); break;   //
            case DEF_MODEL_4NP:     Serial.print( "силовым ключом на 4 N-канала до 10A" );            break;   //
            case DEF_MODEL_4PP:     Serial.print( "силовым ключом на 4 P-канала до 10A" );            break;   //
            default:                Serial.print( "неизвестным силовым ключом или реле" );                     //
        }                           Serial.print( ".\r\n" );                                                   //
        if(pwrkey.changeAddress(i)){ Serial.print( "Адрес модуля изменён на 0x");                              //   Меняем адрес модуля на указанный в переменной i.
                                    Serial.print( pwrkey.getAddress(), HEX );                                  //   Выводим текущий адрес модуля.
                                    Serial.print( " и сохранён в flash память модуля." );                      //   Выводим текущий адрес модуля.
        }else{                      Serial.print( "Адрес модуля изменить не удалось!");                        //   Если метод pwrkey.changeAddress() вернул false, значит библиотеке не удалось сменить адрес модуля.
        }                                                                                                      //
    }else{                          Serial.print( "нет ни силовых ключей, ни реле!" );                         //   Если метод pwrkey.begin() вернул false, значит библиотеке не удалось найти модуль реле или силовых ключей.
    }                               Serial.print( "\r\n" );                                                    //
}                                                                                                              //
                                                                                                               //
void loop(){                                                                                                   //
}                                                                                                              //