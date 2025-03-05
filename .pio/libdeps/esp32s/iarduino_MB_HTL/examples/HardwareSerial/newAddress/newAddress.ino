// ПРИМЕР СМЕНЫ АДРЕСА:                                                            //
                                                                                   //
uint8_t nowAddress = 3;                                                            //   Текущий адрес ( 1 - 247 ).
uint8_t newAddress = 10;                                                           //   Новый адрес ( 1 - 247 ).
                                                                                   //
#include <iarduino_Modbus.h>                                                       //   Подключаем библиотеку для работы по протоколу Modbus.
#include <iarduino_MB_HTL.h>                                                       //   Подключаем библиотеку для работы с модулем HTL.
                                                                                   //
ModbusClient    modbus(Serial1, 2);                                                //   Создаём объект для работы по протоколу Modbus указывая объект шины UART-RS485 для протокола и вывод DE конвертера UART-RS485.
iarduino_MB_HTL sensor(modbus);                                                    //   Создаём объект для работы с модулем HTL указывая объект протокола Modbus.
                                                                                   //
void setup(){                                                                      //
     int f;                                                                        //
     Serial.begin(9600); while(!Serial);                                           //   Инициируем передачу данных в монитор последовательного порта, указав его скорость.
     Serial1.begin(9600); while(!Serial1);                                         //   Инициируем работу с шиной UART-RS485 указав её скорость.
     modbus.begin();                                                               //   Инициируем работу по протоколу Modbus.
//   modbus.setTimeout(10);                                                        //   Указываем максимальное время ожидания ответа по протоколу Modbus.
//   modbus.setDelay(4);                                                           //   Указываем минимальный интервал между отправляемыми сообщениями по протоколу Modbus.
//   modbus.setTypeMB( MODBUS_RTU );                                               //   Указываем тип протокола Modbus: MODBUS_RTU (по умолчанию), или MODBUS_ASCII.
//   Инициируем работу с модулем HTL:                                              //
     f =    sensor.begin(nowAddress);                                              //   Инициируем работу с модулем HTL, указав текущий адрес модуля nowAddress.
     if(f){ Serial.println("Модуль HTL найден");            }                      //   Если адрес не указан f=sensor.begin(), то он будет найден, но это займёт некоторое время.
     else { Serial.println("Модуль HTL не найден"); return; }                      //
//   Меняем адрес модуля HTL:                                                      //
     f =    sensor.changeID(newAddress);                                           //   Меняем адрес модуля HTL на новый newAddress.
     if(f){ Serial.println("Адрес изменён");            }                          //
     else { Serial.println("Адрес не изменён"); return; }                          //
//   Выводим сообшение о модуле HTL:                                               //
     Serial.println((String) "Текущий адрес   = "+sensor.getID()      );           //   Выводим текущий адрес модуля HTL.
     Serial.println((String) "Версия прошивки = "+sensor.getVersion() );           //   Выводим версию прошивки модуля HTL.
}                                                                                  //
                                                                                   //
void loop(){                                                                       //
//   Мигаем светодиодом обнаружения устройства (на разъёме):                       //
     sensor.setIDLED(false); delay(1000);                                          //
     sensor.setIDLED(true ); delay(1000);                                          //
}                                                                                  //
