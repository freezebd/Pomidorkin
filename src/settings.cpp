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

    upd.update(kk::floattempdht1, data.Air1.tfloat);   // rs485 Температура float
    upd.update(kk::humdht1, data.Air1.hfloat);          // rs485 Влажность float
    upd.update(kk::dht1Rele_led, data.Air1.Rel_on);
    upd.update(kk::dht1TempRele_startTemp, data.Air1.tTrigx10 / 10 );
    //upd.update(kk::dht1TempRele_TempThreshold, data.Air1.tTreshold );

    upd.update(kk::floattempdht2, data.Soil1.tfloat); // rs 485
    upd.update(kk::humdht2, data.Soil1.hfloat);        // rs 485
    upd.update(kk::dht2Rele_led, data.Soil1.Rel_on);    // реле влажность 
    //upd.update(kk::floattempDS1, data.dsOne.tfloat);
    //upd.update(kk::DS1Rele_led, data.dsOne.rel_on);
    //upd.update(kk::floattempDS2, data.dsTwo.tfloat);
    //upd.update(kk::DS2Rele_led, data.dsTwo.rel_on);
    upd.update(kk::DS2Rele_led, data.Soil1.Rel_on);

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

        case kk::dht1TempRele_startTemp:
            // пересчитываем температуру х10 чтобы не множиться в цикле. аналогично в setup()
            data.Air1.tTrigx10 = db[kk::dht1TempRele_startTemp].toInt() * 10;
            userDhtRelays();
            b.reload();
            break;
        case kk::dht1TempRele_TempThreshold:
            switch (db[kk::dht1TempRele_TempThreshold].toInt()) {
                case 0:
                    data.Air1.tTreshold = 5;
                    break;
                case 1:
                    data.Air1.tTreshold = 10;
                    break;
                case 2:
                    data.Air1.tTreshold = 20;
                    break;
                case 3:
                    data.Air1.tTreshold = 30;
                    break;
            }
            userDhtRelays();
            b.reload();
            break;

        case kk::dht2HumRele_startHum:
            // пересчитываем влажность
            // data.dhtTwo.hTrig = db[kk::dht2HumRele_startHum].toInt();
            data.Soil1.hTrig = db[kk::dht2HumRele_startHum].toInt();
            userDhtRelays();
            b.reload();
            break;
        case kk::dht2HumRele_HumThreshold:
            switch (db[kk::dht2HumRele_HumThreshold].toInt()) {
                case 0:
                    // data.dhtTwo.hTreshold = 1;
                    data.Soil1.hTreshold = 1;
                    break;
                case 1:
                    // data.dhtTwo.hTreshold = 2;
                    data.Soil1.hTreshold = 2;
                    break;
                case 2:
                    // data.dhtTwo.hTreshold = 5;
                    data.Soil1.hTreshold = 5;
                    break;
                case 3:
                    // data.dhtTwo.hTreshold = 10;
                    data.Soil1.hTreshold = 10;
                    break;
            }
            userDhtRelays();
            b.reload();
            break;

            // case kk::DS1Rele_startTemp:
            //     // пересчитываем температуру х10 чтобы не множиться в цикле. аналогично в setup()
            //     data.dsOne.tTrigx10 = db[kk::DS1Rele_startTemp].toInt() * 10;
            //     userDSRelays();
            //     b.reload();
            //     break;

            // case kk::DS1Rele_TempThreshold:
            //     switch (db[kk::DS1Rele_TempThreshold].toInt()) {
            //         case 0:
            //             data.dsOne.tTreshold = 2;
            //             break;
            //         case 1:
            //             data.dsOne.tTreshold = 5;
            //             break;
            //         case 2:
            //             data.dsOne.tTreshold = 10;
            //             break;
            //         case 3:
            //             data.dsOne.tTreshold = 30;
            //             break;
            //     }
            //     userDSRelays();
            //     b.reload();
            //     break;

            // case kk::DS2Rele_startTemp:
            //     // пересчитываем температуру х10 чтобы не множиться в цикле. аналогично в setup()
            //     data.dsTwo.tTrigx10 = db[kk::DS2Rele_startTemp].toInt() * 10;
            //     userDSRelays();
            //     b.reload();
            //     break;

            // case kk::DS2Rele_TempThreshold:
            //     switch (db[kk::DS2Rele_TempThreshold].toInt()) {
            //         case 0:
            //             data.dsTwo.tTreshold = 2;
            //             break;
            //         case 1:
            //             data.dsTwo.tTreshold = 5;
            //             break;
            //         case 2:
            //             data.dsTwo.tTreshold = 10;
            //             break;
            //         case 3:
            //             data.dsTwo.tTreshold = 30;
            //             break;
            //     }
            //     userDSRelays();
            //     b.reload();
            //     break;
    
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
        sets::Group g(b, db[kk::DS1name]); // ИЗМЕНИТЬ ИМЯ ПЕРЕМЕННОЙ НА НОРМАЛЬНОЕ !!!!
        {
            sets::Row g(b);
            b.LabelFloat(kk::floattempdht1, db[kk::dht1name], data.Air1.tfloat, 1, 0xec9736);  // DHT22 темп 1
           // b.Label("°С");
        }
        {
            sets::Row g(b);
            b.LabelFloat(kk::humdht1, "Влажность", data.Air1.hfloat, 1, 0xd17e1f);  // 0xd17e1f влажность 1
            // b.Label("%", "");
            //b.Label("%");
        }
        if (b.Switch(kk::dht1TempRele_enabled, "Нагрев", nullptr, 0xb7701e)) {  // Реле 1

            if (db[kk::dht1TempRele_enabled].toInt() == 0)
                data.Air1.State = 0;  // принудительно выключаем реле
                userDhtRelays();
                b.reload();
        }
        if (db[kk::dht1TempRele_enabled].toInt() != 0) {
            {
                sets::Row g(b);
                b.LED(kk::dht1Rele_led, "Cтатус >>", data.Air1.Rel_on, sets::Colors::Gray, sets::Colors::Yellow);

                //b.Label(" ");
            }
            b.Number(kk::dht1TempRele_startTemp, "Включается если ниже, °C");
            b.Select(kk::dht1TempRele_TempThreshold, "Порог отключения", "0,5 °C;1 °C;2 °C;3 °C");
        }
    }     //"Воздух"   
    
    {     //"Почва" 
        sets::Group g(b, db[kk::DS2name]); // ИЗЕНИТЬ НА НОРМАЛЬНОЕ ИМЯ ДАТЧИКА
        {
            sets::Row g(b);
            b.LabelFloat(kk::floattempdht2, db[kk::dht2name], data.Soil1.tfloat, 1, 0x3da7f2);  // DHT22 темп 2
            //b.Label("°С");
        }
        {
            sets::Row g(b);
            b.LabelFloat(kk::humdht2, "Влажность", data.Soil1.hfloat, 1, 0x2680bf);  // Влажность 2
            //b.Label("%");
        }
        if (b.Switch(kk::dht2HumRele_enabled, "Увлажнение", nullptr, 0x3da7f2)) {  // Реле 1
            if (db[kk::dht2HumRele_enabled].toInt() == 0)
                data.Soil1.State = 0;  // принудительно выключаем реле
            userDhtRelays();
            b.reload();
        }
        if (db[kk::dht2HumRele_enabled].toInt() != 0) {
            {
                sets::Row g(b);
                b.LED(kk::dht2Rele_led, "Cтатус >>", data.Soil1.Rel_on, sets::Colors::Gray, sets::Colors::Blue);
                b.Label(" ");
            }
            b.Number(kk::dht2HumRele_startHum, "Включается, если ниже");
            b.Select(kk::dht2HumRele_HumThreshold, "Порог отключения,", "1 %;2 %;5 %;10 %");
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
                b.Input(kk::dht1name, "dht22 1:");
                b.Input(kk::dht2name, "dht22 2:");
                b.Input(kk::DS1name, "ДАТЧИК 1");
                b.Input(kk::DS2name, "ДАТЧИК 2:");
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