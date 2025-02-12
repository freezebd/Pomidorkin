#include "settings.h"

#include <GyverDS3231.h>
#include <LittleFS.h>
#include <SettingsGyver.h>
#include <WiFiConnector.h>

#include "data.h"  // тут лежит структура data по кошерному
#include "nastroyki.h"
#include "userTimers.h"
#include "modbus.h"

GyverDBFile db(&LittleFS, "/pomidorkin.db");      // база данных для хранения настроек будет автоматически записываться в файл при изменениях
SettingsGyver sett("Помидоркин@", &db);  // указывается заголовок меню, подключается база данных
GyverDS3231 rtc;
Datime curDataTime(rtc);


static bool notice_f;                            // флаг на отправку уведомления о подключении к wifi
//static bool sens_alert;                          // флаг на отправку уведомления о ошибке чтения датчиков
static const char *const WEEKdays[] = {
    "Выходной",
    "Понедельник",
    "Вторник",
    "Среда",
    "Четверг",
    "Пятница",
    "Суббота",
    "Воскресенье"};
// это апдейтер. Функция вызывается, когда вебморда запрашивает обновления

//sets::Logger logger(150);


void update(sets::Updater &upd) {
    // отправляем свежие значения по имени (хэшу) виджета

    upd.update(kk::secondsNow, data.secondsNow);           //Секунды с начало суток
    upd.update(kk::datime, data.datime);
    upd.update(kk::secondsUptime, data.secondsUptime);     // Секунды аптайм
      
    if (!data.uptime_Days) {
        upd.update(kk::uptimeDays, (String)("0 дней"));
    } else if (data.uptime_Days == 1)
        upd.update(kk::uptimeDays, (String)("1 день"));
    else if (data.uptime_Days < 5)
        upd.update(kk::uptimeDays, (String)(data.uptime_Days + String(" дня")));
    else if (data.uptime_Days >= 5)
        upd.update(kk::uptimeDays, (String)(data.uptime_Days + String(" дней")));
    
    // день недели выводим, оч красиво
    upd.update(kk::dayofweek, (String)(WEEKdays[curDataTime.weekDay])); // день недели (1 понедельник - 7 воскресенье)

    upd.update("t1Discr_led"_h, data.rel1_on);
    upd.update("t2Discr_led"_h, data.rel2_on);
    upd.update("t3Discr_led"_h, data.rel3_on);
    upd.update("t4Discr_led"_h, data.rel4_on);
    upd.update("t5Discr_led"_h, data.rel5_on);
    upd.update("t6Discr_led"_h, data.rel6_on);

    upd.update(kk::floattempair, data.Air1.tfloat);              // обновление веб интерфейса температуры воздуха
    upd.update(kk::airTempRele_led, data.Air1.TempRele_on);      // обновление веб интерфейса светодиода реле температуры воздуха
    upd.update(kk::airRele_startTemp, data.Air1.tTrigx10 / 10);  // обновление веб интерфейса  включения реле температуры воздуха

    upd.update(kk::floathumeair, data.Air1.hfloat);              // обновление веб интерфейса влажности воздуха
    upd.update(kk::airHumeRele_led, data.Air1.HumeRele_on);      // обновление веб интерфейса светодиода реле влажности воздуха
    upd.update(kk::airRele_startHume, data.Air1.hTrigx10 / 10);  // обновление веб интерфейса  включения реле влажности воздуха

    upd.update(kk::floattempsoil, data.Soil1.tfloat);                      // обновление веб интерфейса температуры почвы
    upd.update(kk::soilTempRele_led, data.Soil1.TempRele_on);              // обновление веб интерфейса светодиода реле температуры почвы
    upd.update(kk::soilRele_startTemp, data.Soil1.tTrigx10 / 10);          // обновление веб интерфейса  включения реле температуры почвы
   // upd.update(kk::soilRele_TempThreshold, data.Soil1.tTresholdx10 / 10);  // обновление веб интерфейса порога отключения реле температуры почвы

    upd.update(kk::floathumsoil, data.Soil1.hfloat);                       // обновление веб интерфейса влажности почвы
    upd.update(kk::soilHumeRele_led, data.Soil1.HumeRele_on);              // обновление веб интерфейса светодиода реле влажности почвы
    upd.update(kk::soilRele_startHume, data.Soil1.hTrigx10 / 10);          // обновление веб интерфейса  включения реле влажности почвы
    // upd.update(kk::soilRele_HumeTreshold, data.Soil1.hTresholdx10 / 10);   // обновление веб интерфейса порога отключения реле влажности почвы

    //upd.update(kk::floattempDS1, data.dsOne.tfloat);
    //upd.update(kk::DS1Rele_led, data.dsOne.rel_on);
    //upd.update(kk::floattempDS2, data.dsTwo.tfloat);
    //upd.update(kk::DS2Rele_led, data.dsTwo.rel_on);


    upd.update("lbl1"_h, (String)(curDataTime.weekDay + String(" день недели")));
    upd.update("lbl2"_h, millis());
    if (notice_f)  // уведомление при вводе wifi данных
    {
        notice_f = false;
        upd.notice("Если не ошибся с вводом, устройство подключится к  wifi сети, светодиод медленно замигает");
        //    upd.alert("Ошибка");
    }
    if (sens_alert) {
        sens_alert = false;
        upd.alert("Ошибка чтения датчиков");
    }
}  // update


void build(sets::Builder &b) {

    if (b.build.isAction()) {// можно узнать, было ли действие по виджету
       
        Serial.print("Set: 0x");
        Serial.print(b.build.id, HEX);
        Serial.print(" = ");
        Serial.println(b.build.value);
    }
    
    switch (b.build.id) {// обработка действий от виджетов:
        // case kk::logUpdate:  // если ввели импут
        //     // logger.println(b.build.id, HEX);
        //     logger.print("старт таймера 1 в ");
        //     logger.println(db[kk::t1Discr_startTime]);
        //     logger.print(" curDataTime.weekDay:");
        //     logger.println(curDataTime.weekDay);
        //     break;
    }

    switch (b.build.id) {  // костыль на моментальное обновление индикаторных светодиодов

        // case kk::ntp_gmt: //// перезапись NTP времени
        //     //NTP.setGMT(b.build.value); // перезапись часового пояса
        //     setStampZone(b.build.value); // указать часовой пояс, если в программе нужен реальный unix
        //     Serial.print("Часовой пояс = ");
        //     Serial.println(b.build.value);
        //     b.reload();
        //     break;
        case kk::t1Discr_startTime:
            userSixTimers();
            b.reload();
            break;
        case kk::t1Discr_endTime:
            userSixTimers();
            b.reload();
            break;
        case kk::t2Discr_startTime:
            userSixTimers();
            b.reload();
            break;
        case kk::t2Discr_endTime:
            userSixTimers();
            b.reload();
            break;
        case kk::t3Discr_startTime:
            userSixTimers();
            b.reload();
            break;
        case kk::t3Discr_endTime:
            userSixTimers();
            b.reload();
            break;
        case kk::t4Discr_startTime:
            userSixTimers();
            b.reload();
            break;
        case kk::t4Discr_endTime:
            userSixTimers();
            b.reload();
            break;
        case kk::t5Discr_startTime:
            userSixTimers();
            b.reload();
            break;
        case kk::t5Discr_endTime:
            userSixTimers();
            b.reload();
            break;
        case kk::t6Discr_startTime:
            userSixTimers();
            b.reload();
            break;
        case kk::t6Discr_endTime:
            userSixTimers();
            b.reload();
            break;

        case kk::airRele_startTemp:
            // пересчитываем температуру воздуха х10 чтобы не множиться в цикле. аналогично в setup()
            data.Air1.tTrigx10 = db[kk::airRele_startTemp].toInt() * 10;
            userDhtRelays();
            b.reload();
            break;
        case kk::airRele_TempThreshold:
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
            userDhtRelays();
            b.reload();
            break;

        case kk::airRele_startHume:
            // пересчитываем влажность воздуха х10 чтобы не множиться в цикле. аналогично в setup()
            data.Air1.hTrigx10 = db[kk::airRele_startHume].toInt() * 10;
            userDhtRelays();
            b.reload();
            break;
        case kk::airRele_HumeTreshold:
            switch (db[kk::airRele_HumeTreshold].toInt()) {
                case 0:
                    // data.dhtTwo.hTreshold = 1;
                    data.Air1.hTresholdx10 = 1;
                    break;
                case 1:
                    // data.dhtTwo.hTreshold = 2;
                    data.Air1.hTresholdx10 = 2;
                    break;
                case 2:
                    // data.dhtTwo.hTreshold = 5;
                    data.Air1.hTresholdx10 = 5;
                    break;
                case 3:
                    // data.dhtTwo.hTreshold = 10;
                    data.Air1.hTresholdx10 = 10;
                    break;
            }
            userDhtRelays();
            b.reload();
            break;

        case kk::soilRele_startTemp:
                // пересчитываем температуру почвы х10 чтобы не множиться в цикле.
                data.Soil1.tTrigx10 = db[kk::soilRele_startTemp].toInt() * 10;
                userDhtRelays();
                b.reload();
                break;

            case kk::soilRele_TempThreshold:
                switch (db[kk::soilRele_TempThreshold].toInt()) {
                    case 0:
                        data.Soil1.tTresholdx10 = 2;
                        break;
                    case 1:
                        data.Soil1.tTresholdx10 = 5;
                        break;
                    case 2:
                        data.Soil1.tTresholdx10 = 10;
                        break;
                    case 3:
                        data.Soil1.tTresholdx10 = 30;
                        break;
                }
                userDhtRelays();
                b.reload();
                break;

            case kk::soilRele_startHume:
                // пересчитываем влажность почвы х10 чтобы не множиться в цикле.
                data.Soil1.hTrigx10 = db[kk::soilRele_startHume].toInt() * 10;
                userDhtRelays();
                b.reload();
                break;

            case kk::soilRele_HumeTreshold:
                switch (db[kk::soilRele_HumeTreshold].toInt()) {
                    case 0:
                        data.Soil1.hTresholdx10 = 2;
                        break;
                    case 1:
                        data.Soil1.hTresholdx10 = 5;
                        break;
                    case 2:
                        data.Soil1.hTresholdx10 = 10;
                        break;
                    case 3:
                        data.Soil1.hTresholdx10 = 30;
                        break;
                }
                userDhtRelays();
                b.reload();
                break;
    
    }  //  switch (b.build.id)

    {// WEB интерфейс ВЕБ морда формируется здесь
        sets::Group g(b, "Дата & Время");
        if (rtc.tick() || rtc.updateNow()) {
            {
                sets::Row g(b);
                b.Label(kk::dayofweek, "");  // текущая дата
            if (b.Date(kk::datime, ""))      // Установка даты
            {
                rtc.setTime((db[datime].toInt32()) + rtc.daySeconds());
                db.update();
                b.reload();  
            }
            if (b.Time(kk::secondsNow))  // установка время
            {
                rtc.setTime((db[datime].toInt32()) + (db[secondsNow].toInt32())); // UNIX - текущие секунды + установ. секунды
                db.update();
                b.reload();
            }
            }
        }  // rtc.setTime()

        {
            sets::Row g(b);
            // sets::Row g(b, "Row");
            b.Label(kk::uptimeDays, "Аптайм");
            b.Time(kk::secondsUptime, " ");
        }

        
    }

    {//"Воздух"  
        sets::Group g(b, db[kk::airTempName]); // ИЗМЕНИТЬ ИМЯ ПЕРЕМЕННОЙ НА НОРМАЛЬНОЕ !!!!
        {
            sets::Row g(b);
            b.LabelFloat(kk::floattempair, "Температура", data.Air1.tfloat, 1, 0xec9736);  
           // b.Label("°С");
        }
        {
            sets::Row g(b);
            b.LabelFloat(kk::floathumeair, "Влажность", data.Air1.hfloat, 1, 0xd17e1f);  
            
        }
        if (b.Switch(kk::airTempRele_enabled, "Нагрев", nullptr, 0xb7701e)) {  // Реле 1

            if (db[kk::airTempRele_enabled].toInt() == 0)
                data.Air1.StateAir = 0;  // принудительно выключаем реле
                userDhtRelays();
                b.reload();
        }
        if (db[kk::airTempRele_enabled].toInt() != 0) {
            {
                sets::Row g(b);
                b.LED(kk::airTempRele_led, "Cтатус >>", data.Air1.TempRele_on, sets::Colors::Gray, sets::Colors::Yellow);

                //b.Label(" ");
            }
            b.Number(kk::airRele_startTemp, "Включается если ниже, °C");
            b.Select(kk::airRele_TempThreshold, "Порог отключения", "0,5 °C;1 °C;2 °C;3 °C");
        }
    }     //"Воздух"   
    
    {     //"Почва" 
        sets::Group g(b, db[kk::soilTempName]); // ИЗЕНИТЬ НА НОРМАЛЬНОЕ ИМЯ ДАТЧИКА
        {
            sets::Row g(b);
            b.LabelFloat(kk::floattempsoil, "Температура", data.Soil1.tfloat, 1, 0x3da7f2);  // DHT22 темп 2
            //b.Label("°С");
        }
        {
            sets::Row g(b);
            b.LabelFloat(kk::floathumsoil, "Влажность", data.Soil1.hfloat, 1, 0x2680bf);  // Влажность 2
            //b.Label("%");
        }
        if (b.Switch(kk::soilHumeRele_enabled, "Увлажнение", nullptr, 0x3da7f2)) {  // Реле 1
            if (db[kk::soilHumeRele_enabled].toInt() == 0)
                data.Soil1.StateHume = 0;  // принудительно выключаем реле
            userDhtRelays();
            b.reload();
        }
        if (db[kk::soilHumeRele_enabled].toInt() != 0) {
            {
                sets::Row g(b);
                b.LED(kk::soilHumeRele_led, "Cтатус >>", data.Soil1.HumeRele_on, sets::Colors::Gray, sets::Colors::Blue);
                b.Label(" ");
            }
            b.Number(kk::soilRele_startHume, "Включается, если ниже h%");
            b.Select(kk::soilRele_HumeTreshold, "Порог отключения,", "1 %;2 %;5 %;10 %");
        }
    }  //"Почва"
    //
    //

    // {  // "Вода"
    //     sets::Group g(b, "Вода");
    //     {
    //         sets::Row g(b);
    //         // b.LabelFloat(kk::floattemp1, "dht1", 1);
    //         b.LabelFloat(kk::floattempDS1, db[kk::DS1name], data.dsOne.tfloat, 1, sets::Colors::Aqua);  // DHT22 темп 1
    //         b.Label("°С");
    //     }
    //     if (b.Switch(kk::DS1Rele_enabled, "Охлаждение", nullptr, sets::Colors::Aqua)) {  // Реле 1
    //         // data.DS1Rele_enbl = db[kk::DS1Rele_enabled];
    //         userDSRelays();
    //         b.reload();
    //     }
    //     // if (data.DS1Rele_enbl) {
    //     if (db[kk::DS1Rele_enabled].toInt() != 0) {
    //         {
    //             sets::Row g(b);
    //             b.LED(kk::DS1Rele_led, "Cтатус >>", data.dsOne.rel_on, sets::Colors::Gray, sets::Colors::Aqua);
    //             b.Label(" ");
    //         }
    //         b.Number(kk::DS1Rele_startTemp, "Включается при превышении, °C");
    //         b.Select(kk::DS1Rele_TempThreshold, "Порог отключения", "0,2 °C;0,5 °C;1 °C;3 °C");
    //     }
    //     b.Label(" ");
    //     {
    //         sets::Row g(b);
    //         // b.LabelFloat(kk::floattemp1, "dht1", 1);
    //         b.LabelFloat(kk::floattempDS2, db[kk::DS2name], data.dsTwo.tfloat, 1, sets::Colors::Aqua);  // DHT22 темп 1
    //         b.Label("°С");
    //     }
    //     if (b.Switch(kk::DS2Rele_enabled, "Нагрев", nullptr, sets::Colors::Aqua)) {  // Реле 1
    //         // data.DS2Rele_enbl = db[kk::DS2Rele_enabled];
    //         userDSRelays();
    //         b.reload();
    //     }
    //     // if (data.DS2Rele_enbl) {
    //     if (db[kk::DS2Rele_enabled].toInt() != 0) {
    //         {
    //             sets::Row g(b);
    //             b.LED(kk::DS2Rele_led, "Cтатус >>", data.dsTwo.rel_on, sets::Colors::Gray, sets::Colors::Aqua);
    //             b.Label(" ");
    //         }  // LED row
    //         b.Number(kk::DS2Rele_startTemp, "Включается, если ниже °C");
    //         b.Select(kk::DS2Rele_TempThreshold, "Порог отключения", "0,2 °C;0,5 °C;1 °C;3 °C");
    //     }
    // }  //"Вода"

    { /* суточные таймеры */
        sets::Group g(b, "Суточные таймеры");
        if (b.Switch(kk::t1Discr_enabled, db[kk::t1Discr_name], nullptr, sets::Colors::Yellow)) {  // Реле 1
            data.t1discr_enbl = db[kk::t1Discr_enabled];
            userSixTimers();
            b.reload();
        }
        if (data.t1discr_enbl) {
            {
                sets::Row g(b);
                b.LED("t1Discr_led"_h, "Cтатус >>", data.rel1_on, sets::Colors::Gray, sets::Colors::Yellow);
                b.Label(" ");
            }  // LED row
            b.Time(kk::t1Discr_startTime, "Включается в ..");
            b.Time(kk::t1Discr_endTime, ".. и отключается в");
            b.Label(" ", " ");
        }
        if (b.Switch(kk::t2Discr_enabled, db[kk::t2Discr_name], nullptr, sets::Colors::Green))  // Реле 2
        {
            data.t2discr_enbl = db[kk::t2Discr_enabled];
            userSixTimers();
            b.reload();
        }
        if (data.t2discr_enbl) {
            {
                sets::Row g(b);
                b.LED("t2Discr_led"_h, "Cтатус >>", data.rel2_on, sets::Colors::Gray, sets::Colors::Green);
                b.Label(" ");
            }  // LED row
            b.Time(kk::t2Discr_startTime, "Вкл в ..");
            b.Time(kk::t2Discr_endTime, ".. откл ");
            b.Label(" ", " ");
        }
        
        if (b.Switch(kk::t3Discr_enabled, db[kk::t3Discr_name], nullptr, sets::Colors::Mint))  // Реле 3
        {
            data.t3discr_enbl = db[kk::t3Discr_enabled];
            userSixTimers();
            b.reload();
        }
        if (data.t3discr_enbl) {
            {
                sets::Row g(b);
                b.LED("t3Discr_led"_h, "Cтатус >>", data.rel3_on, sets::Colors::Gray, sets::Colors::Mint);
                b.Label(" ");
            }  // LED row
            b.Time(kk::t3Discr_startTime, "Вкл в ..");
            b.Time(kk::t3Discr_endTime, ".. откл");
            b.Label(" ", " ");
        }
        if (b.Switch(kk::t4Discr_enabled, db[kk::t4Discr_name], nullptr, sets::Colors::Aqua))  // Реле 4
        {
            data.t4discr_enbl = db[kk::t4Discr_enabled];
            userSixTimers();
            b.reload();
        }
        if (data.t4discr_enbl) {
            {
                sets::Row g(b);
                b.LED("t4Discr_led"_h, "Cтатус >>", data.rel4_on, sets::Colors::Gray, sets::Colors::Aqua);
                b.Label(" ");
            }  // LED row
            b.Time(kk::t4Discr_startTime, "Вкл в ..");
            b.Time(kk::t4Discr_endTime, ".. откл");
            b.Label(" ", " ");
        }
        if (b.Switch(kk::t5Discr_enabled, db[kk::t5Discr_name], nullptr, sets::Colors::Blue))  // Реле 5
        {
            data.t5discr_enbl = db[kk::t5Discr_enabled];
            userSixTimers();
            b.reload();
        }
        if (data.t5discr_enbl) {
            {
                sets::Row g(b);
                b.LED("t5Discr_led"_h, "Cтатус >>", data.rel5_on, sets::Colors::Gray, sets::Colors::Blue);
                b.Label(" ");
            }  // LED row
            b.Time(kk::t5Discr_startTime, "Вкл в ..");
            b.Time(kk::t5Discr_endTime, ".. откл");
            b.Label(" ", " ");
        }
        if (b.Switch(kk::t6Discr_enabled, db[kk::t6Discr_name], nullptr, sets::Colors::Violet))  // Реле 6
        {
            data.t6discr_enbl = db[kk::t6Discr_enabled];
            userSixTimers();
            b.reload();
        }
        if (data.t6discr_enbl) {
            {
                sets::Row g(b);
                b.LED("t6Discr_led"_h, "Статус >>", data.rel6_on, sets::Colors::Gray, sets::Colors::Violet);
                b.Label(" ");
            }  // LED row
            b.Time(kk::t6Discr_startTime, "Вкл в ..");
            b.Time(kk::t6Discr_endTime, ".. откл");
            b.Label("Дни недели", " ");
            b.Switch(kk::t6Discr_inMonday, "Понедельник", nullptr, sets::Colors::Violet);
            b.Switch(kk::t6Discr_inTuesday, "Вторник", nullptr, sets::Colors::Violet);
            b.Switch(kk::t6Discr_inWensday, "Среда", nullptr, sets::Colors::Violet);
            b.Switch(kk::t6Discr_inThursday, "Четверг", nullptr, sets::Colors::Violet);
            b.Switch(kk::t6Discr_inFriday, "Пятница", nullptr, sets::Colors::Violet);
            b.Switch(kk::t6Discr_inSaturday, "Суббота", nullptr, sets::Colors::Violet);
            b.Switch(kk::t6Discr_inSunday, "Воскресенье", nullptr, sets::Colors::Violet);
            b.Label(" ", " ");
            b.Label(" ", " ");
        }
    } /* суточные таймеры */
    {/* Настройки , внизу страницы*/
        sets::Group g(b, " ");
        {
            sets::Menu g(b, "Опции");

            {
                sets::Menu g(b, "Интерфейс");
                /*
                      //пример изменения имени виджета
                      b.Input(kk::btnName, "новое имя кнопки:");
                      if(b.Button(kk::btnflex, db[kk::btnName], db[kk::btnColor])) b.reload();
                */

                b.Input(kk::t1Discr_name, "Имя Реле1:");
                b.Input(kk::t2Discr_name, "Имя Реле2:");
                b.Input(kk::t3Discr_name, "Имя Реле3:");
                b.Input(kk::t4Discr_name, "Имя Реле4:");
                b.Input(kk::t5Discr_name, "Имя Реле5:");
                b.Input(kk::t6Discr_name, "Имя Реле6:");
                b.Input(kk::airTempName, "Имя датчика 1");
               // b.Input(kk::airHumeName, "Имя датчика влажности воздуха:");
                b.Input(kk::soilTempName, "Имя датчика 2");
              //  b.Input(kk::soilHumeName, "Имя датчика влажности почвы:");
            }
            {
                sets::Menu g(b, "Расширенные");
                /// провалились в расширенные пристройки
                {
                    sets::Group g(b, "настройки WiFi");
                    b.Input(kk::wifi_ssid, "SSID");
                    b.Pass(kk::wifi_pass, "Password");
                    if (b.Button(kk::apply, "Save & Restart")) {
                        db.update();  // сохраняем БД не дожидаясь таймаута
                        WiFiConnector.connect(db[kk::wifi_ssid], db[kk::wifi_pass]);
                        notice_f = true;
                        //          ESP.restart();
                    }  // button Save
                }  // настройки wifi

                // кнопки являются "групповым" виджетом, можно сделать несколько кнопок в одной строке
                if (b.beginButtons()) {
                    if (b.Button(kk::btn2, "стереть базу(аккуратно!)", sets::Colors::Red)) {
                        Serial.println("could clear db");
                        db.clear();
                        db.update();
                    }
                    b.endButtons();  // завершить кнопки
                }
            }  // Расширенные
        }
   
     }  // Подстройки

}  // builder