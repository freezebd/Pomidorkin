// ПРИМЕР ПОЛУЧЕНИЯ СПИСКА АДРЕСОВ ВСЕХ УСТРОЙСТВ НА ШИНЕ:                         //
                                                                                   //
#include <SoftwareSerial.h>                                                        //   Подключаем библиотеку для работы с программной шиной UART.
#include <iarduino_Modbus.h>                                                       //   Подключаем библиотеку для работы по протоколу Modbus.
#include <iarduino_MB_HTL.h>                                                       //   Подключаем библиотеку для работы с модулем HTL.
                                                                                   //
SoftwareSerial rs485(8,9);                                                         //   Создаём объект для работы с программной шиной UART-RS485 указывая выводы RX, TX.
ModbusClient   modbus(rs485, 2);                                                   //   Создаём объект для работы по протоколу Modbus указывая объект программной шины UART-RS485 и вывод DE конвертера UART-RS485.
                                                                                   //
void setup(){                                                                      //
     Serial.begin(9600);   while(!Serial);                                         //   Инициируем передачу данных в монитор последовательного порта, указав его скорость.
     rs485.begin(9600); // while(!rs485 );                                         //   Инициируем работу с программной шиной UART-RS485 указав её скорость.
     modbus.begin();                                                               //   Инициируем работу по протоколу Modbus.
//   modbus.setTimeout(10);                                                        //   Указываем максимальное время ожидания ответа по протоколу Modbus.
//   modbus.setDelay(4);                                                           //   Указываем минимальный интервал между отправляемыми сообщениями по протоколу Modbus.
//   modbus.setTypeMB( MODBUS_RTU );                                               //   Указываем тип протокола Modbus: MODBUS_RTU (по умолчанию), или MODBUS_ASCII.
     Serial.println("Поиск устройств...");                                         //
//   Выполняем поиск всех устройств на шине (занимает несколько секунд):           //
     modbus.findID();                                                              //   Ищем адреса всех устройств на шине.
//   modbus.findID( DEF_MODEL_HTL );                                               //   Ищем адреса всех устройств с идентификатором модуля DEF_MODEL_HTL.
     if( modbus.available() ){                                                     //
     //  Устройства найдены:                                                       //
         Serial.print("Найдено ");                                                 //
         Serial.print( modbus.available() );                                       //
         Serial.print(" устройств с адресами: ");                                  //
     //  Выводим адреса устройств:                                                 //
         while( modbus.available() ){                                              //
             Serial.print( modbus.read() );                                        //
             Serial.print(", ");                                                   //
         }   Serial.println();                                                     //
     }else{  Serial.println("Не найдено ни одного устройства."); }                 //
}                                                                                  //
                                                                                   //
void loop(){                                                                       //
}                                                                                  //