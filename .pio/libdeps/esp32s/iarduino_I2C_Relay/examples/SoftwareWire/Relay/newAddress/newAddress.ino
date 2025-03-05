// ПРИМЕР СМЕНЫ АДРЕСА МОДУЛЯ:                                                                                 // * Строки со звёздочкой являются необязательными.
// Требуется установить библиотеку <iarduino_I2C_Software.h>                                                   //   https://iarduino.ru/file/627.html
                                                                                                               //
// Данный скетч демонстрирует не только возможность смены адреса на указанный в переменной i,                  //
// но и обнаружение, и вывод текущего адреса модуля на шине I2C.                                               //
                                                                                                               //
// Библиотека <iarduino_I2C_Relay.h> предназначена для работы с реле и силовыми ключами I2C-flash              //
// и меняет только их адреса. Но, в разделе Файл/Примеры/.../findSortDevices находятся примеры смены адресов и //
// сортировки адресов любых модулей iarduino серии Flash I2C.                                                  //
                                                                                                               //
// Модуль электромеханических реле на 2-канала FLASH-I2C:                                                      //   https://iarduino.ru/shop/Expansion-payments/rele-2ch-i2c.html
// Информация о модуле и описание библиотеки:                                                                  //   https://wiki.iarduino.ru/page/rele-2ch-i2c/
                                                                                                               //
// Модуль твердотельных реле на 4-канала FLASH-I2C:                                                            //   https://iarduino.ru/shop/Expansion-payments/solid-rele-4ch-i2c.html
// Информация о модуле и описание библиотеки:                                                                  //   https://wiki.iarduino.ru/page/solid-rele-4ch-i2c/
                                                                                                               //
// Информация о подключении модулей к шине I2C:                                                                //   https://wiki.iarduino.ru/page/i2c_connection/
                                                                                                               //
uint8_t i = 0x09;                                                                                              //   Назначаемый модулю новый адрес (0x07 < адрес < 0x7F).
                                                                                                               //
#include <iarduino_I2C_Software.h>                                                                             //   Подключаем библиотеку для работы с программной шиной I2C, до подключения библиотеки iarduino_I2C_Relay.
SoftTwoWire sWire(3,4);                                                                                        //   Создаём объект программной шины I2C указав выводы которым будет назначена роль линий: SDA, SCL.
                                                                                                               //
#include <iarduino_I2C_Relay.h>                                                                                //   Подключаем библиотеку для работы с реле и силовыми ключами.
iarduino_I2C_Relay relay;                                                                                      //   Объявляем объект relay для работы с функциями и методами библиотеки iarduino_I2C_Relay.
                                                                                                               //   Если при объявлении объекта указать адрес, например, relay(0xBB), то пример будет работать с тем модулем, адрес которого был указан.
void setup(){                                                                                                  //
    Serial.begin(9600);                                                                                        //
    while(!Serial){;}                                                                                          // * Ждём завершения инициализации шины UART.
    delay(500);                     Serial.print( "На шине I2C " );                                            //
    if( relay.begin(&sWire) ){      Serial.print( "найден модуль с адресом 0x" );                              //   Инициируем работу с модулем, указав ссылку на объект для работы с шиной I2C на которой находится модуль (Wire, Wire1, Wire2 ...).
                                    Serial.print( relay.getAddress(), HEX );                                   //   Выводим текущий адрес модуля.
                                    Serial.print( ", который является " );                                     //
        switch( relay.getModel() ){                                                                            //   Сравниваем модель модуля с константами:
            case DEF_MODEL_2RM:     Serial.print( "электромеханическим реле на 2-канала" );           break;   //
            case DEF_MODEL_4RT:     Serial.print( "твердотельным реле на 4-канала" );                 break;   //
            case DEF_MODEL_4NC:     Serial.print( "силовым ключом на 4 N-канала с измерением тока" ); break;   //
            case DEF_MODEL_4PC:     Serial.print( "силовым ключом на 4 P-канала с измерением тока" ); break;   //
            case DEF_MODEL_4NP:     Serial.print( "силовым ключом на 4 N-канала до 10A" );            break;   //
            case DEF_MODEL_4PP:     Serial.print( "силовым ключом на 4 P-канала до 10A" );            break;   //
            default:                Serial.print( "неизвестным силовым ключом или реле" );                     //
        }                           Serial.print( ".\r\n" );                                                   //
        if(relay.changeAddress(i)){ Serial.print( "Адрес модуля изменён на 0x");                               //   Меняем адрес модуля на указанный в переменной i.
                                    Serial.print( relay.getAddress(), HEX );                                   //   Выводим текущий адрес модуля.
                                    Serial.print( " и сохранён в flash память модуля." );                      //   Выводим текущий адрес модуля.
        }else{                      Serial.print( "Адрес модуля изменить не удалось!");                        //   Если метод relay.changeAddress() вернул false, значит библиотеке не удалось сменить адрес модуля.
        }                                                                                                      //
    }else{                          Serial.print( "нет ни силовых ключей, ни реле!" );                         //   Если метод relay.begin() вернул false, значит библиотеке не удалось найти модуль реле или силовых ключей.
    }                               Serial.print( "\r\n" );                                                    //
}                                                                                                              //
                                                                                                               //
void loop(){                                                                                                   //
}                                                                                                              //