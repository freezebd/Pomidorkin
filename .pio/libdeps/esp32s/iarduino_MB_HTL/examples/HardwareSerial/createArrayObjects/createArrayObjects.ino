// ПРИМЕР ЧТЕНИЯ ДАННЫХ СО ВСЕХ МОДУЛЕЙ HTL НА ШИНЕ:                               //
// без указания их адресов в скетче.                                               //
                                                                                   //
#include <iarduino_Modbus.h>                                                       //   Подключаем библиотеку для работы по протоколу Modbus.
#include <iarduino_MB_HTL.h>                                                       //   Подключаем библиотеку для работы с модулем HTL.
                                                                                   //
ModbusClient     modbus(Serial1, 2);                                               //   Создаём объект для работы по протоколу Modbus указывая объект шины UART-RS485 для протокола и вывод DE конвертера UART-RS485.
iarduino_MB_HTL* sensor;                                                           //   Создаём указатель который будет переопределён в массив объектов работы с модулями HTL.
                                                                                   //
uint8_t sum=0;                                                                     //   Определяем переменную для хранения количества найденных модулей HTL.
uint8_t i=0;                                                                       //
                                                                                   //
void setup(){                                                                      //
     Serial.begin(9600); while(!Serial);                                           //   Инициируем передачу данных в монитор последовательного порта, указав его скорость.
     Serial1.begin(9600); while(!Serial1);                                         //   Инициируем работу с шиной UART-RS485 указав её скорость.
     modbus.begin();                                                               //   Инициируем работу по протоколу Modbus.
//   modbus.setTimeout(10);                                                        //   Указываем максимальное время ожидания ответа по протоколу Modbus.
//   modbus.setDelay(4);                                                           //   Указываем минимальный интервал между отправляемыми сообщениями по протоколу Modbus.
//   modbus.setTypeMB( MODBUS_RTU );                                               //   Указываем тип протокола Modbus: MODBUS_RTU (по умолчанию), или MODBUS_ASCII.
                                                                                   //
//   Выполняем поиск всех модулей HTL шине modbus (занимает несколько сек):        //
     Serial.print("Поиск модулей HTL ... " );                                      //
     sum = modbus.findID( DEF_MODEL_HTL );                                         //   Ищем адреса всех устройств с идентификатором DEF_MODEL_HTL.
     uint8_t arrID[sum];                                                           //   Объявляем массив arrID для хранения найденных адресов.
     while( modbus.available() ){ arrID[i++]=modbus.read(); }                      //   Заполняем массив arrID найденными адресами.
     Serial.print("найдено "); Serial.print(sum); Serial.println(" модулей.");     //
                                                                                   //
//   Если модули HTL не найдены, то переходим в loop() где ничего не произойдёт.   //
     if( sum==0 ){ return; }                                                       //
                                                                                   //
//   Переопределяем указатель sensor в массив объектов:                            //
     sensor=(iarduino_MB_HTL*)malloc(sizeof(iarduino_MB_HTL)*sum);                 //   Выделяем под массив sensor требуемый объем памяти.
     for(i=0; i<sum; i++){ sensor[i]=modbus; sensor[i].begin(arrID[i]); }          //   Инициируем работу с найденными модулями.
                                                                                   //
//   Выводим адреса найденных модулей HTL:                                         //
     Serial.print("Адреса найденных модулей HTL: ");                               //
     for(i=0; i<sum; i++){ Serial.print( sensor[i].getID() ); Serial.print(", ");} //
     Serial.println();                                                             //
}                                                                                  //
                                                                                   //
void loop(){                                                                       //
     for(i=0; i<sum; i++){                                                         //   Проходим по всем модулям HTL.
         Serial.print( (String) "ID="   + sensor[i].getID()   + ", "       );      //   Выводим ID текущего модуля HTL.
         Serial.print( (String) "TEM="  + sensor[i].getTEM()  + "°C, "     );      //   Выводим температуру.
         Serial.print( (String) "HUM="  + sensor[i].getHUM()  + "%, "      );      //   Выводим относительную влажность.
         Serial.print( (String) "HUM="  + sensor[i].getHUMA() + "г/м3, "   );      //   Выводим абсолютную влажность.
         Serial.print( (String) "LUX="  + sensor[i].getLUX()  + "лк.\r\n"  );      //   Выводим освещённость.
         delay(1000);                                                              //
     }                                                                             //
}                                                                                  //