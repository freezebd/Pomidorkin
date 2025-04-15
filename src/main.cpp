// прошивка тут
// C:\Users\Acer\Documents\PlatformIO\Projects\minihub\.pio\build\esp32doit-devkit-v1

#include <Arduino.h>

#include "led.h"
#include "timer.h"
#ifdef ESP8266
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif
#include <GyverDBFile.h>
#include <LittleFS.h>
#include <SettingsGyver.h>
//#include <GyverNTP.h>
#include <WiFiConnector.h>



#include "data.h"  // тут лежит структура data по кошерному
#include "nastroyki.h"
#include "settings.h"
#include "userTimers.h"
#include "modbus.h"
#include "reley.h"
#include "air_temp_control.h"


Timer each5Sec(5000ul);    // таймер раз в 10 сек
Timer each5min(300000ul);  // таймер раз в 5 мин
Timer eachSec(1000ul);     // таймер раз в сек

LED indikator(INDIKATOR, 300, 3, 50, 20);  // каждые 1000 милисек мигаем 3 раза каждых 50 мс, время горения 20 мсек
bool gotWifi = false;                      // если подключено было к сети
int valNum;
uint32_t startSeconds = 0;
uint32_t stopSeconds = 0;
byte initially = 5;        // первых 10 секунд приписываем время в переменную
byte checker = 0;          // автомат modbus
uint32_t prevMs = 0;       // Опрос время цикла loop    

void setup() {
    each5min.rst();
    Serial.begin(115200);
    // Wire.begin(); // 
    // rtc.begin();
    
    //NTP.attachRTC(rtc);

    // Serial.print("Часы >> ");
    // Serial.println(rtc.isOK());
     
    init_modbus(); // Настройка modbus
    init_reley();  // Реле I2C
    init_modbus_air(); // Инициализация датчика воздуха
    
    Serial.println("ESP STARTED !");

    // ======== SETTINGS ========
    WiFi.mode(WIFI_AP_STA);  // режим AP_STA. Вызываем перед sett.begin(), чтобы settings знал о текущем режиме wifi
    sett.begin();
    sett.onBuild(build);
    sett.onUpdate(update);
    sett.config.theme = sets::Colors::Orange; // цвет веб моржы ( по умолчанию зеленый)

    // Оптимизация WiFi
    WiFi.setAutoReconnect(true);
    WiFi.persistent(true);

    // ======== DATABASE ========
#ifdef ESP32
    LittleFS.begin(true);
#else
    LittleFS.begin();
#endif
    db.begin();
    db.init(kk::wifi_ssid, WIFI);
    db.init(kk::wifi_pass, WIFIPASS);
    db.init(kk::wifi_networks, "");  // Список найденных сетей
    db.init(kk::wifi_selected, "");  // Выбранная сеть
   // db.init(kk::datime, (uint32_t)0ul);
    db.init(kk::datime,"");
    db.init(kk::secondsNow, (uint32_t)0ul);

    db.init(kk::airTempName, "Температура воздуха");
    db.init(kk::airTempRele_enabled, (uint8_t)0);
    db.init(kk::airRele_startTemp, (uint8_t)30);
    db.init(kk::airRele_TempThreshold, (uint8_t)1);
    // Инициализация новых ключей для управления температурой
    db.init(kk::airRele_dayTemp, (uint8_t)25);      // Дневная температура по умолчанию 25°C
    db.init(kk::airRele_nightTemp, (uint8_t)20);    // Ночная температура по умолчанию 20°C
    db.init(kk::airRele_dayStartTime, (uint32_t)21600ul);  // Начало дня по умолчанию 6:00
    db.init(kk::airRele_nightStartTime, (uint32_t)72000ul); // Начало ночи по умолчанию 20:00
    db.init(kk::airRele_tempHysteresis, (uint8_t)1); // Гистерезис по умолчанию 1°C

    db.init(kk::airHumeName, "Влажность воздуха");
    db.init(kk::airHumeRele_enabled, (uint8_t)0);
    db.init(kk::airRele_startHume, (uint8_t)30);
    db.init(kk::airRele_HumeTreshold, (uint8_t)1);

  //  db.init(kk::soilTempName, "Имя датчика почвы ");
    db.init(kk::soilTempRele_enabled, (uint8_t)0);
    db.init(kk::soilRele_startTemp, (uint8_t)26);
    db.init(kk::soilRele_TempThreshold, (uint8_t)1);

    db.init(kk::soilHumeName, "Имя датчика почвы 1");
    db.init(kk::soilHumeRele_enabled, (uint8_t)0);
    db.init(kk::soilRele_startHume, (uint8_t)30);
    db.init(kk::soilRele_HumeTreshold, (uint8_t)1);

    db.init(kk::soil2TempName, "Имя датчика почвы 2");
    db.init(kk::soil2TempRele_enabled, (uint8_t)0);
    db.init(kk::soil2Rele_startTemp, (uint8_t)26);
    db.init(kk::soil2Rele_TempThreshold, (uint8_t)1);

  //  db.init(kk::soil2HumeName, "Имя датчика почвы 2");
    db.init(kk::soil2HumeRele_enabled, (uint8_t)0);
    db.init(kk::soil2Rele_startHume, (uint8_t)30);
    db.init(kk::soil2Rele_HumeTreshold, (uint8_t)1);


    db.init(kk::t1Discr_name, "Реле 1");
    db.init(kk::t1Discr_enabled, (uint8_t)0);
    db.init(kk::t1Discr_startTime, (uint32_t)21600ul);
    db.init(kk::t1Discr_endTime, (uint32_t)72000ul);
    db.init(kk::t6Discr_name, "Реле 6 недельное");
    db.init(kk::t6Discr_enabled, (uint8_t)0);
    db.init(kk::t6Discr_startTime, (uint32_t)21600ul);
    db.init(kk::t6Discr_endTime, (uint32_t)72000ul);
    db.init(kk::t6Discr_inMonday, (uint8_t)0);
    db.init(kk::t6Discr_inTuesday, (uint8_t)0);
    db.init(kk::t6Discr_inWensday, (uint8_t)0);
    db.init(kk::t6Discr_inThursday, (uint8_t)0);
    db.init(kk::t6Discr_inFriday, (uint8_t)0);
    db.init(kk::t6Discr_inSaturday, (uint8_t)0);
    db.init(kk::t6Discr_inSunday, (uint8_t)0);
    db.init(kk::t6Discr_algorithm, (uint8_t)0);  // По умолчанию работа по времени
    db.init(kk::t6Discr_temp_threshold, (uint8_t)25);
    db.init(kk::t6Discr_hum_threshold, (uint8_t)50);
    db.init(kk::t6Discr_hysteresis, (uint8_t)1);
    db.init(kk::old_address, (uint8_t)0x00);
    db.init(kk::new_address, (uint8_t)0x00);
    
    //db.dump(Serial);

    data.t1discr_enbl = db[kk::t1Discr_enabled];  // запустим суточные таймеры
    data.t6discr_enbl = db[kk::t6Discr_enabled];
    userSixTimers();
    // пересчитываем температуру и влажность воздуха и почвы в х10 чтобы не множиться в цикле
    data.Air1.tTrigx10 = db[kk::airRele_startTemp].toInt() * 10;
    data.Air1.hTrigx10 = db[kk::airRele_startHume].toInt() * 10;
    data.Soil1.tTrigx10 = db[kk::soilRele_startTemp].toInt() * 10;
    data.Soil1.hTrigx10 = db[kk::soilRele_startHume].toInt() * 10;
    data.Soil2.tTrigx10 = db[kk::soil2Rele_startTemp].toInt() * 10;
    data.Soil2.hTrigx10 = db[kk::soil2Rele_startHume].toInt() * 10;  
    
    // берем показания
    switch (db[kk::airRele_TempThreshold].toInt()) {
        case 0:
            data.Air1.tTresholdx10 = 5;
            break;
        case 1:
            data.Air1.tTresholdx10 = 10;
            break;
        case 2:
            data.Air1.tTresholdx10 = 20;
            break;
        case 3:
            data.Air1.tTresholdx10 = 30;
            break;
    }
    switch (db[kk::airRele_HumeTreshold].toInt()) {
        case 0:
            data.Air1.hTresholdx10 = 5;
            break;
        case 1:
            data.Air1.hTresholdx10 = 10;
            break;
        case 2:
            data.Air1.hTresholdx10 = 20;
            break;
        case 3:
            data.Air1.hTresholdx10 = 30;
            break;
    }
   
    switch (db[kk::soilRele_HumeTreshold].toInt()) {
        case 0:
            data.Soil1.hTresholdx10 = 1;
            break;
        case 1:
            data.Soil1.hTresholdx10 = 2;
            break;
        case 2:
            data.Soil1.hTresholdx10 = 5;
            break;
        case 3:
            data.Soil1.hTresholdx10 = 10;
            break;
    }
    switch (db[kk::soil2Rele_HumeTreshold].toInt()) {
        case 0:
            data.Soil2.hTresholdx10 = 1;
            break;
        case 1:
            data.Soil2.hTresholdx10 = 2;    
            break;
        case 2:
            data.Soil2.hTresholdx10 = 5;
            break;
        case 3:
            data.Soil2.hTresholdx10 = 10;
            break;  
    }   


    userRelays();
    

    // ======== WIFI ========
    // подключение и реакция на подключение или ошибку
    WiFiConnector.setPass("12345678");  // пароль точки доступа
    WiFiConnector.setTimeout(10);       // сколько секунд пытаться приконнектиттся
    WiFiConnector.onConnect([]() {
        Serial.print("Con with IP: ");
        Serial.println(WiFi.localIP());
        indikator.setPeriod(3000, 1, 200, 150);  // раз в 000 сек, 0 раз взмигнем - по 00 милисек периоды, гореть будем 0 милисек
        gotWifi = true;
        NTP.begin(3);
      
    });
    WiFiConnector.onError([]() {
        Serial.print("Error! start AP ");
        Serial.println(WiFi.softAPIP());
        indikator.setPeriod(600, 2, 100, 50);  // раз в  секунду два раза взмигнем - по 200 милисек, гореть будем 50 милисек
        if (each5min.ready()) ESP.restart();   // через 5 минут ребутаемся
    });

    WiFiConnector.connect(db[kk::wifi_ssid], db[kk::wifi_pass]);
}   // setup
void loop() {

    uint32_t ms = millis();
      sett.tick();
      ms = millis() - ms;   
      if (ms > 100) measureExecutionTime("sett.tick", []() {  // измерение времени выполнения функции
        sett.tick();
    }); 

    userRelays();  // мониторим данные по воздуху и почве
    userSixTimers();  // мониторим изменеие по реле
    
    
    // если wifi связь есть, сбрасываем вочдог таймер 5 минутный.
    // если нет связи, ждем 5 минут и ребутаемся, а то мало ли
    // если связь восстановилась после потери, снова мигаем плавно
    WiFiConnector.tick();  // поддержка wifi связи
    if (WiFiConnector.connected()) {
        if (!gotWifi) indikator.setPeriod(3000, 1, 200, 150);  // спокойное мигание после реконнекта к wifi
        gotWifi = true;
        each5min.rst();
    } else {
        if (gotWifi) {
            gotWifi = false;  // для запуска частой мигалки
            // общее время, кол-во, период одного, один зажжен на.
            indikator.setPeriod(1000, 10, 100, 70);  // часто мигаем
        }
        if (each5min.ready()) ESP.restart();
    }  // WiFi.connected()

    indikator.tick();  // in loop
    // sett.tick();       // поддержка веб интерфейса

    // if (rtc.tick()) {
    //     data.secondsNow = rtc.daySeconds();
    //     data.datime = rtc.getTime().getUnix();
    //     curDataTime = rtc.getTime();
    // } 
    if (NTP.tick()) {
      //  Serial.println(NTP.toString());
        data.secondsNow = NTP.daySeconds();
        data.datime = NTP.toString();
        curDataTime = NTP.getUnix();
    }

    // if (each5Sec.ready())  // раз в 5 сек
    // {
    // }  // each5Sec

    if (eachSec.ready()) {  // раз в 1 сек

        data.secondsNow++;                  // инкермент реалтайм
        data.secondsUptime++;               // инкермент аптайм
        if (data.secondsUptime == 86399) {  // инкремент дней аптайма
            data.secondsUptime = 0;
            data.uptime_Days++;
        }

        switch (checker) { // раз в секунду опрашиваем датчики
            case 0:
                checker = 5;
                break;
            case 5:
                readSensorSoil();
                checker = 10;
                break;
            case 10:
                readSensorAir();
                checker = 15;
                break;
            case 15:
                readSensorSoil2();
                checker = 5;
                break;      
        }
    }
   
}  // loop