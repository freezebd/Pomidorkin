// ПРИМЕР ЧТЕНИЯ ДАННЫХ БЕЗ ПРОВЕРКИ:                                              //
// Чтение данных с проверкой доступно в примере "getData"                          //
                                                                                   //
#include <iarduino_Modbus.h>                                                       //   Подключаем библиотеку для работы по протоколу Modbus.
#include <iarduino_MB_HTL.h>                                                       //   Подключаем библиотеку для работы с модулем HTL.
                                                                                   //
ModbusClient    modbus(Serial1, 2);                                                //   Создаём объект для работы по протоколу Modbus указывая объект шины UART-RS485 для протокола и вывод DE конвертера UART-RS485.
iarduino_MB_HTL sensor(modbus);                                                    //   Создаём объект для работы с модулем HTL указывая объект протокола Modbus.
                                                                                   //
void setup(){                                                                      //
     Serial.begin(9600); while(!Serial);                                           //   Инициируем передачу данных в монитор последовательного порта, указав его скорость.
     Serial1.begin(9600); while(!Serial1);                                         //   Инициируем работу с шиной UART-RS485 указав её скорость.
     modbus.begin();                                                               //   Инициируем работу по протоколу Modbus.
//   modbus.setTimeout(10);                                                        //   Указываем максимальное время ожидания ответа по протоколу Modbus.
//   modbus.setDelay(4);                                                           //   Указываем минимальный интервал между отправляемыми сообщениями по протоколу Modbus.
//   modbus.setTypeMB( MODBUS_RTU );                                               //   Указываем тип протокола Modbus: MODBUS_RTU (по умолчанию), или MODBUS_ASCII.
     sensor.begin(3);                                                              //   Инициируем работу с модулем HTL, указав его адрес. Если адрес не указан sensor.begin(), то он будет найден, но это займёт некоторое время.
//   sensor.setPeriod( SENSOR_SHT, 0.5f );                                         //   Указываем модулю обновлять данные датчика влажности и температуры каждые 0.5 секунд.
//   sensor.setPeriod( SENSOR_LTR, 1.0f );                                         //   Указываем модулю обновлять данные датчика освещённости каждую секунду.
}                                                                                  //
                                                                                   //
void loop(){                                                                       //
     Serial.print( (String) "TEM="  + sensor.getTEM()  + "°C, "     );             //   Выводим температуру.
     Serial.print( (String) "HUM="  + sensor.getHUM()  + "%, "      );             //   Выводим относительную влажность.
     Serial.print( (String) "HUM="  + sensor.getHUMA() + "г/м3, "   );             //   Выводим абсолютную влажность.
     Serial.print( (String) "LUX="  + sensor.getLUX()  + "лк.\r\n"  );             //   Выводим освещённость.
     delay(1000);                                                                  //
}                                                                                  //