// путь где лежат бинарный файл проекта
// C:\Users\Freez\OneDrive\Документы\PlatformIO\Projects\Pomidorkin\.pio\build\esp32dev
#include "settings.h"

#include <GyverDS3231.h>
#include <LittleFS.h>
#include <SettingsGyver.h>  // Изменил на GyverWS
#include <WiFiConnector.h>

#define AIR_TEMP_RELE_ENABLED 0
#define AIR_TEMP_RELE_START_TEMP 30
#define AIR_TEMP_RELE_TEMP_THRESHOLD 1

#include "modbus.h"
#include "nastroyki.h"
#include "reley.h"  // Добавляем для доступа к change_relay_address()
#include "userTimers.h"

GyverDBFile db(&LittleFS, "/pomidorkin.db");  // база данных для хранения настроек будет автоматически записываться в файл при изменениях
SettingsGyver sett("Помидоркин@", &db);       // указывается заголовок меню, подключается база данных
GyverDS3231 rtc;
Datime curDataTime(rtc);

bool flagreley = true;  // флаг для перезагрузки вебморды при смене адреса реле
static bool notice_f;   // флаг на отправку уведомления о подключении к wifi

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

// sets::Logger logger(150);

void update(sets::Updater &upd) {
    // отправляем свежие значения по имени (хэшу) виджета

    upd.update(kk::secondsNow, data.secondsNow);        // Секунды с начало суток
    upd.update(kk::datime, data.datime);                // Время в секундах с 1970 года
    upd.update(kk::secondsUptime, data.secondsUptime);  // Секунды аптайм

    // Обновляем значение SSID только если оно не пустое
    String currentSSID = db[kk::wifi_ssid];
    if (currentSSID.length() > 0) {
        upd.update(kk::wifi_ssid, currentSSID);
    }

    if (!data.uptime_Days) {
        upd.update(kk::uptimeDays, (String)("0 дней"));
    } else if (data.uptime_Days == 1)
        upd.update(kk::uptimeDays, (String)("1 день"));
    else if (data.uptime_Days < 5)
        upd.update(kk::uptimeDays, (String)(data.uptime_Days + String(" дня")));
    else if (data.uptime_Days >= 5)
        upd.update(kk::uptimeDays, (String)(data.uptime_Days + String(" дней")));

    // день недели выводим, оч красиво
    upd.update(kk::dayofweek, (String)(WEEKdays[curDataTime.weekDay]));  // день недели (1 понедельник - 7 воскресенье)

    upd.update("t1Discr_led"_h, data.rel1_on);  // светодиоды реле 1
    upd.update("t6Discr_led"_h, data.rel6_on);  // светодиоды реле 6
    upd.update("t6Discr_led1"_h, data.rel6_on);  // светодиоды реле 6
    upd.update("t1Discr_led1"_h, data.rel1_on);  // светодиоды реле 1
    upd.update("airTempRele_led"_h, data.Air1.TempRele_on);  // светодиоды реле температуры воздуха
    upd.update("airHumeRele_led"_h, data.Air1.HumeRele_on);  // светодиоды реле влажности воздуха
    upd.update("soilHumeRele_led"_h, data.Soil1.HumeRele_on);  // светодиоды реле влажности почвы 1
    upd.update("soil2HumeRele_led"_h, data.Soil2.HumeRele_on);  // светодиоды реле влажности почвы 2

    // Обновление значений датчиков для реле 6
    if (db[kk::t6Discr_algorithm].toInt() == 1) {
        upd.update("sensor_value"_h, (String)(data.Air1.tfloat + String(" °C")));
    } else if (db[kk::t6Discr_algorithm].toInt() == 2) {
        upd.update("sensor_value"_h, (String)(data.Air1.hfloat + String(" %")));
    } else if (db[kk::t6Discr_algorithm].toInt() == 3) {
        upd.update("sensor_value"_h, (String)(data.Soil1.hfloat + String(" %")));
    } else if (db[kk::t6Discr_algorithm].toInt() == 4) {
        upd.update("sensor_value"_h, (String)(data.Soil2.hfloat + String(" %")));
    }

    upd.update(kk::floattempair, (String)(data.Air1.tfloat + String(" °C")));              // обновление веб интерфейса температуры воздуха
    upd.update(kk::airTempRele_led, data.Air1.TempRele_on);                                // обновление веб интерфейса светодиода реле температуры воздуха
    upd.update(kk::airRele_startTemp, (String)(data.Air1.tTrigx10 / 10 + String(" °C")));  // обновление веб интерфейса  включения реле температуры воздуха
    upd.update(kk::floathumeair, (String)(data.Air1.hfloat + String(" %")));               // обновление веб интерфейса влажности воздуха
    upd.update(kk::airHumeRele_led, data.Air1.HumeRele_on);                                // обновление веб интерфейса светодиода реле влажности воздуха
    upd.update(kk::airRele_startHume, (String)(data.Air1.hTrigx10 / 10 + String(" %")));   // обновление веб интерфейса  включения реле влажности воздуха

    upd.update(kk::floattempsoil, (String)(data.Soil1.tfloat + String(" °C")));              // обновление веб интерфейса температуры почвы
    upd.update(kk::soilTempRele_led, data.Soil1.TempRele_on);                                // обновление веб интерфейса светодиода реле температуры почвы
    upd.update(kk::soilRele_startTemp, (String)(data.Soil1.tTrigx10 / 10 + String(" °C")));  // обновление веб интерфейса  включения реле температуры почвы
    upd.update(kk::floathumsoil, (String)(data.Soil1.hfloat + String(" %")));                // обновление веб интерфейса влажности почвы
    upd.update(kk::soilHumeRele_led, data.Soil1.HumeRele_on);                                // обновление веб интерфейса светодиода реле влажности почвы
    upd.update(kk::soilRele_startHume, (String)(data.Soil1.hTrigx10 / 10 + String(" %")));   // обновление веб интерфейса  включения реле влажности почвы

    upd.update(kk::floattempsoil2, (String)(data.Soil2.tfloat + String(" °C")));              // обновление веб интерфейса температуры почвы 2
    upd.update(kk::soil2TempRele_led, data.Soil2.TempRele_on);                                // обновление веб интерфейса светодиода реле температуры почвы 2
    upd.update(kk::soil2Rele_startTemp, (String)(data.Soil2.tTrigx10 / 10 + String(" °C")));  // обновление веб интерфейса  включения реле температуры почвы 2
    upd.update(kk::floathumsoil2, (String)(data.Soil2.hfloat + String(" %")));                // обновление веб интерфейса влажности почвы 2
    upd.update(kk::soil2HumeRele_led, data.Soil2.HumeRele_on);                                // обновление веб интерфейса светодиода реле влажности почвы 2
    upd.update(kk::soil2Rele_startHume, (String)(data.Soil2.hTrigx10 / 10 + String(" %")));   // обновление веб интерфейса  включения реле влажности почвы 2

    upd.update(kk::old_address, String(data.old_address));  // обновление веб интерфейса старого адреса реле
    upd.update(kk::new_address, String(data.new_address));  // обновление веб интерфейса нового адреса реле

    upd.update("lbl1"_h, (String)(curDataTime.weekDay + String(" день недели")));
    upd.update("lbl2"_h, millis());

    // Обновление графики
    float v[] = {data.Air1.tfloat, data.Air1.hfloat};
    // upd.updatePlot(H(run), v);
    upd.updatePlot(H(stack), v);

    if (notice_f)  // уведомление при вводе wifi данных
    {
        notice_f = false;
        upd.notice("Если не ошибся с вводом, устройство подключится к  wifi сети, светодиод медленно замигает");
    }
    if (sens_alert) {
        sens_alert = false;
        upd.alert("Ошибка чтения датчика");
    }
    if (change_relay_address_alert) {
        change_relay_address_alert = false;
        upd.alert("Ошибка изменения адреса реле");
    }
    if (change_relay_address_notice) {
        change_relay_address_notice = false;
        upd.notice("Адрес реле успешно изменен с " + String(data.old_address) + " на " + String(data.new_address));
    }
    if (relay_address_error) {
        relay_address_error = false;
        upd.alert("Неверный новый адрес (должен быть между 1 и 127)");
    }
    if (module_address_error) {
        module_address_error = false;
        upd.alert("Модуль с адресом " + String(data.old_address) + " не найден");
    }
}  // update

void measureExecutionTime(const char *functionName, void (*func)()) {  // измерение времени выполнения функции
    unsigned long startTime = millis();
    func();
    unsigned long duration = millis() - startTime;

    Serial.print("Время выполнения ");
    Serial.print(functionName);
    Serial.print(": ");
    Serial.print(duration);
    Serial.println(" мс");
}

void build(sets::Builder &b) {
    unsigned long startTime = millis();

    if (b.build.isAction()) {  // можно узнать, было ли действие по виджету

        Serial.print("Set: 0x");
        Serial.print(b.build.id, HEX);
        Serial.print(" = ");
        Serial.println(b.build.value);
    }

    switch (b.build.id) {  // костыль на моментальное обновление индикаторных светодиодов

        case kk::t1Discr_startTime:
            userSixTimers();
            b.reload();
            break;
        case kk::t1Discr_endTime:
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
            userRelays();
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
            userRelays();
            b.reload();
            break;

        case kk::airRele_startHume:
            // пересчитываем влажность воздуха х10 чтобы не множиться в цикле. аналогично в setup()
            data.Air1.hTrigx10 = db[kk::airRele_startHume].toInt() * 10;
            userRelays();
            b.reload();
            break;
        case kk::airRele_HumeTreshold:
            switch (db[kk::airRele_HumeTreshold].toInt()) {
                case 0:
                    data.Air1.hTresholdx10 = 1;
                    break;
                case 1:
                    data.Air1.hTresholdx10 = 2;
                    break;
                case 2:
                    data.Air1.hTresholdx10 = 5;
                    break;
                case 3:
                    data.Air1.hTresholdx10 = 10;
                    break;
            }
            userRelays();
            b.reload();
            break;

        case kk::soilRele_startTemp:
            // пересчитываем температуру почвы датчика 1 х10 чтобы не множиться в цикле.
            data.Soil1.tTrigx10 = db[kk::soilRele_startTemp].toInt() * 10;
            userRelays();
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
            userRelays();
            b.reload();
            break;

        case kk::soilRele_startHume:
            // пересчитываем влажность почвы х10 чтобы не множиться в цикле.
            data.Soil1.hTrigx10 = db[kk::soilRele_startHume].toInt() * 10;
            userRelays();
            b.reload();
            break;

        case kk::soilRele_HumeTreshold:  // порог отключения влажности почвы датчика 1
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
            userRelays();
            b.reload();
            break;
        case kk::soil2Rele_startTemp:
            // пересчитываем температуру почвы датчика 2 х10 чтобы не множиться в цикле.
            data.Soil2.tTrigx10 = db[kk::soil2Rele_startTemp].toInt() * 10;
            userRelays();
            b.reload();
            break;
        case kk::soil2Rele_TempThreshold:
            switch (db[kk::soil2Rele_TempThreshold].toInt()) {
                case 0:
                    data.Soil2.tTresholdx10 = 2;
                    break;
                case 1:
                    data.Soil2.tTresholdx10 = 5;
                    break;
                case 2:
                    data.Soil2.tTresholdx10 = 10;
                    break;
                case 3:
                    data.Soil2.tTresholdx10 = 30;
                    break;
            }
            userRelays();
            b.reload();
            break;
        case kk::soil2Rele_startHume:
            // пересчитываем влажность почвы датчика 2 х10 чтобы не множиться в цикле.
            data.Soil2.hTrigx10 = db[kk::soil2Rele_startHume].toInt() * 10;
            userRelays();
            b.reload();
            break;
        case kk::soil2Rele_HumeTreshold:
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
            b.reload();
            break;

    }  //  switch (b.build.id)

    {  //++++++++++++++++++++++++++++ WEB интерфейс ВЕБ морда формируется здесь++++++++++++++++++++++
        sets::Group g(b, "Дата & Время");
        // if (rtc.tick() || rtc.updateNow() ) {
        //  if (rtc.updateNow()) {
        {
            sets::Row g(b);
            b.Label(kk::dayofweek, "");  // текущая дата
            if (b.Date(kk::datime, ""))  // Установка даты
            {
                rtc.setTime((db[datime].toInt32()) + rtc.daySeconds());
                db.update();
                b.reload();
            }
            if (b.Time(kk::secondsNow))  // установка время
            {
                rtc.setTime((db[datime].toInt32()) + (db[secondsNow].toInt32()));  // UNIX - текущие секунды + установ. секунды
                db.update();
                b.reload();
            }
        }
        //  }  // rtc.setTime()

        {
            sets::Row g(b);
            // sets::Row g(b, "Row");
            b.Label(kk::uptimeDays, "Аптайм");
            b.Time(kk::secondsUptime, " ");
        }
        {
            sets::Row g(b);
            b.LED("t1Discr_led1"_h, "", data.rel1_on, sets::Colors::Gray, sets::Colors::Yellow);
            b.LED("airTempRele_led"_h, "", data.Air1.TempRele_on, sets::Colors::Gray, sets::Colors::Red);
            b.LED("airHumeRele_led"_h, "", data.Air1.HumeRele_on, sets::Colors::Gray, sets::Colors::Blue);
            b.LED("soilHumeRele_led"_h, "", data.Soil1.HumeRele_on, sets::Colors::Gray, sets::Colors::Pink);
            b.LED("soil2HumeRele_led"_h, "", data.Soil2.HumeRele_on, sets::Colors::Gray, sets::Colors::Pink);
            b.LED("t6Discr_led1"_h, "", data.rel6_on, sets::Colors::Gray, sets::Colors::Violet);
        }
    }
    static uint8_t tab;                             // статическая
    if (b.Tabs("Домой;Таймеры;Настройки", &tab)) {  // Вкладки
        // при нажатии перезагружаемся и выходим
        b.reload();
        return;
    }
    if (tab == 0) {
        // Сюда добавляем показания со всех датчиков
        {
            sets::Group g(b, db[kk::airTempName]);  // Воздух
            {
                sets::Row g(b);
                b.Label(kk::floattempair, "Температура", String(data.Air1.tfloat + String(" °C")), sets::Colors::Red);
                // b.Label("°С");
            }
            {
                sets::Row g(b);
                b.Label(kk::floathumeair, "Влажность", String(data.Air1.hfloat + String(" %")), sets::Colors::Blue);
            }
            b.PlotStack(H(stack), "Температура;Влажность");
        }
        {
            sets::Group g(b, db[kk::soilHumeName]);  // датчик почвы 1
            {
                sets::Row g(b);
                b.Label(kk::floattempsoil, "Температура", String(data.Soil1.tfloat + String(" °C")), sets::Colors::Pink);  // DHT22 темп 2
                // b.Label("°С");
            }
            {
                sets::Row g(b);
                b.Label(kk::floathumsoil, "Влажность", String(data.Soil1.hfloat + String(" %")), sets::Colors::Pink);  // Влажность 2
                // b.Label("%");
            }
        }
        {
            sets::Group g(b, db[kk::soil2TempName]);  // датчик почвы 2
            {
                sets::Row g(b);
                b.Label(kk::floattempsoil2, "Температура", String(data.Soil2.tfloat + String(" °C")), sets::Colors::Pink);  // DHT22 темп 2
                // b.Label("°С");
            }
            {
                sets::Row g(b);
                b.Label(kk::floathumsoil2, "Влажность", String(data.Soil2.hfloat + String(" %")), sets::Colors::Pink);  // Влажность 2
                // b.Label("%");
            }

            // b.PlotRunning(H(run), "Температура;Влажность");
        }
    } else if (tab == 1) {  // Закладка 2 , управление всеми реле

        {  //"Воздух"

            if (b.Switch(kk::airTempRele_enabled, "Нагрев", nullptr, sets::Colors::Red)) {  // Реле нагрем воздуха
                if (db[kk::airTempRele_enabled].toInt() == 0) 
                    data.Air1.StateAir = 0;  // принудительно выключаем реле
                    userRelays();
                    b.reload();
            }
            if (db[kk::airTempRele_enabled].toInt() != 0) {
                sets::Group g(b, db[kk::airTempName]);  // Воздух
                {
                    sets::Row g(b);
                    b.Label(kk::floattempair, "Температура", String(data.Air1.tfloat + String(" °C")), sets::Colors::Red);
                }
                {
                    sets::Row g(b);
                    b.LED(kk::airTempRele_led, "Cтатус >>", data.Air1.TempRele_on, sets::Colors::Gray, sets::Colors::Red);
                }
                b.Number(kk::airRele_startTemp, "Включается если ниже", nullptr, 0, 90);
                b.Select(kk::airRele_TempThreshold, "Порог отключения", "0,5 °C;1 °C;2 °C;3 °C");
                if (data.Air1.tfloat == -80) {  // если датчик отвалился, принудительно выключаем свитч и  выключаем реле
                    (db[kk::airTempRele_enabled] = 0);
                    data.Air1.StateAir = 0;  // принудительно выключаем реле
                    userRelays();
                    b.reload();
                }
            }
            if (b.Switch(kk::airHumeRele_enabled, "Увлажнение", nullptr, sets::Colors::Blue)) {  // Реле 1 увлажнение воздуха

                if (db[kk::airHumeRele_enabled].toInt() == 0)
                    data.Air1.StateHume = 0;  // принудительно выключаем реле
                userRelays();
                b.reload();
            }
            if (db[kk::airHumeRele_enabled].toInt() != 0) {
                sets::Group g(b, db[kk::airHumeName]);
                {
                    sets::Row g(b);
                    b.Label(kk::floathumeair, "Влажность", String(data.Air1.hfloat + String(" %")), sets::Colors::Blue);
                }
                {
                    sets::Row g(b);
                    b.LED(kk::airHumeRele_led, "Cтатус >>", data.Air1.HumeRele_on, sets::Colors::Gray, sets::Colors::Blue);

                    // b.Label(" ");
                }
                b.Number(kk::airRele_startHume, "Включается если ниже", nullptr, 10, 100);
                b.Select(kk::airRele_HumeTreshold, "Порог отключения", "0,5 h%;1 h%;2 h%;3 h%");
            }
        }  //"Воздух"

        {  //"Почва 1"

            if (b.Switch(kk::soilHumeRele_enabled, "Полив", nullptr, sets::Colors::Pink)) {  // Реле 1
                if (db[kk::soilHumeRele_enabled].toInt() == 0)
                    data.Soil1.StateHume = 0;  // принудительно выключаем реле
                userRelays();
                b.reload();
            }
            if (db[kk::soilHumeRele_enabled].toInt() != 0) {
                sets::Group g(b, db[kk::soilHumeName]);  // датчик почвы 1
                {
                    sets::Row g(b);
                    b.Label(kk::floathumsoil, "Влажность", String(data.Soil1.hfloat + String(" %")), sets::Colors::Pink);  // Влажность 2
                    // b.Label("%");
                }
                {
                    sets::Row g(b);
                    b.LED(kk::soilHumeRele_led, "Cтатус >>", data.Soil1.HumeRele_on, sets::Colors::Gray, sets::Colors::Pink);
                    b.Label(" ");
                }
                b.Number(kk::soilRele_startHume, "Включается, если ниже", nullptr, 10, 100);
                b.Select(kk::soilRele_HumeTreshold, "Порог отключения,", "1 %;2 %;5 %;10 %");
            }
        }  //"Почва 1"

        {  // "Почва 2"

            if (b.Switch(kk::soil2HumeRele_enabled, "Полив", nullptr, sets::Colors::Pink)) {  // Реле 1
                if (db[kk::soil2HumeRele_enabled].toInt() == 0)
                    data.Soil2.StateHume = 0;  // принудительно выключаем реле
                userRelays();
                b.reload();
            }
            if (db[kk::soil2HumeRele_enabled].toInt() != 0) {
                sets::Group g(b, db[kk::soil2TempName]);  // датчик почвы 2
                {
                    sets::Row g(b);
                    b.Label(kk::floathumsoil2, "Влажность", String(data.Soil2.hfloat + String(" %")), sets::Colors::Pink);  // Влажность 2
                    // b.Label("%");
                }
                {
                    sets::Row g(b);
                    b.LED(kk::soil2HumeRele_led, "Cтатус >>", data.Soil2.HumeRele_on, sets::Colors::Gray, sets::Colors::Pink);
                    b.Label(" ");
                }
                b.Number(kk::soil2Rele_startHume, "Включается, если ниже", nullptr, 10, 100);
                b.Select(kk::soil2Rele_HumeTreshold, "Порог отключения,", "1 %;2 %;5 %;10 %");
            }
        }  //"Почва 2"
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
            if (b.Switch(kk::t6Discr_enabled, db[kk::t6Discr_name], nullptr, sets::Colors::Violet))  // Реле 6
            {
                data.t6discr_enbl = db[kk::t6Discr_enabled];
                userSixTimers();
                b.reload();
            }
            if (data.t6discr_enbl) {
                b.Select(kk::t6Discr_algorithm, "Алгоритм работы", "По времени;По температуре воздуха;По влажности воздуха;По влажности почвы 1;По влажности почвы 2");
                b.LED("t6Discr_led"_h, "Статус >>", data.rel6_on, sets::Colors::Gray, sets::Colors::Violet);
                userSixTimers();
                b.reload();
                if (db[kk::t6Discr_algorithm].toInt() > 0) {
                    if (db[kk::t6Discr_algorithm].toInt() == 1) {
                        b.Label("sensor_value"_h, "Настройки датчика");
                        b.Number(kk::t6Discr_temp_threshold, "Порог температуры (°C)", nullptr, 0, 90);
                    } else if (db[kk::t6Discr_algorithm].toInt() == 2) {
                        b.Label("sensor_value"_h, "Настройки датчика");
                        b.Number(kk::t6Discr_hum_threshold, "Порог влажности (%)", nullptr, 0, 100);
                    } else if (db[kk::t6Discr_algorithm].toInt() == 3) {
                        b.Label("sensor_value"_h, "Настройки датчика");
                        b.Number(kk::t6Discr_hum_threshold, "Порог влажности (%)", nullptr, 0, 100);
                    } else if (db[kk::t6Discr_algorithm].toInt() == 4) {
                        b.Label("sensor_value"_h, "Настройки датчика");
                        b.Number(kk::t6Discr_hum_threshold, "Порог влажности (%)", nullptr, 0, 100);
                    }
                    b.Select(kk::t6Discr_hysteresis, "Гистерезис", "1 %/°C;2 %/°C;3 %/°C;4 %/°C;5 %/°C;10 %/°C;15 %/°C;20 %/°C");
                }
                if (db[kk::t6Discr_algorithm].toInt() == 0) {
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
                }
            }
        }
    } /* суточные таймеры */

    //}  // Закладка 2
    else if (tab == 2) {
        // Сюда добовляем настройки

        { /* Настройки , внизу страницы*/
            sets::Group g(b, " ");
            {
                //  sets::Menu g(b, "Опции");

                {
                    sets::Menu g(b, "Интерфейс");
                    /*
                          //пример изменения имени виджета
                          b.Input(kk::btnName, "новое имя кнопки:");
                          if(b.Button(kk::btnflex, db[kk::btnName], db[kk::btnColor])) b.reload();
                    */

                    b.Input(kk::t1Discr_name, "Имя Реле1:");
                    b.Input(kk::t6Discr_name, "Имя Реле6:");
                    b.Input(kk::airTempName, "Температура воздуха");
                    b.Input(kk::airHumeName, "Влажность воздуха");
                    b.Input(kk::soilHumeName, "Имя датчика почвы 1");
                    b.Input(kk::soil2TempName, "Имя датчика почвы 2");
                }
                {
                    sets::Menu g(b, "Настройки конфигурации");
                    {
                        sets::Group g(b, "Настройки WiFi");
                        // Кнопка сканирования сетей
                        if (b.Button(kk::wifi_scan, "Сканировать сети")) {
                            int n = WiFi.scanNetworks();
                            String networks = "";
                            if (n > 0) {
                                for (int i = 0; i < n; ++i) {
                                    String ssid = WiFi.SSID(i);
                                    networks += ssid;
                                    if (i < n - 1) networks += ";";
                                }
                                db[kk::wifi_networks] = networks;
                                db.update();
                            }
                            b.reload();
                        }

                        // Выпадающий список сетей
                        String networks = db[kk::wifi_networks];
                        if (networks.length() > 0) {
                            if (b.Select(kk::wifi_selected, "Доступные сети", networks)) {
                                // Получаем индекс выбранной сети
                                int selectedIndex = db[kk::wifi_selected].toInt();

                                // Получаем имя сети по индексу
                                int startPos = 0;
                                int endPos = 0;
                                for (int i = 0; i <= selectedIndex; i++) {
                                    startPos = endPos;
                                    endPos = networks.indexOf(';', startPos);
                                    if (endPos == -1) {
                                        endPos = networks.length();
                                        break;
                                    }
                                }
                                if (startPos > 0) startPos++;  // Пропускаем разделитель

                                String selectedNetwork = networks.substring(startPos, endPos);
                                if (selectedNetwork.length() > 0) {
                                    db[kk::wifi_ssid] = selectedNetwork;
                                    db.update();
                                }
                                b.reload();
                            }
                        }

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
                        if (b.Button(kk::btn2, "стереть базу !", sets::Colors::Red)) {
                            Serial.println("could clear db");
                            db.clear();
                            db.update();
                        }
                        b.endButtons();  // завершить кнопки
                    }
                }  // Расширенные

                {  // Меню "Реле"
                    sets::Menu g(b, "Реле");
                    {
                        sets::Group g(b, "Найденные реле");

                        if (b.Button(0x1001, "Сканировать реле", sets::Colors::Green)) {
                            measureExecutionTime("scan_relays", []() {  // измерение времени выполнения функции
                                data.relay_count = scan_relays(data.relays);

                            });
                            db.update();
                            b.reload();
                        }

                        if (data.relay_count > 0) {
                            b.Label("Поиск и смена адреса реле");
                            for (uint8_t i = 0; i < data.relay_count; i++) {
                                String addr = String(data.relays[i].address);
                                b.Label("Реле " + String(i + 1) + " (адрес " + addr + ")");
                                if (b.Button(kk::relay_seach + i, "Выбрать", sets::Colors::Blue)) {
                                    data.old_address = data.relays[i].address;
                                    db[kk::old_address] = String(data.old_address);
                                    db.update();
                                }
                            }
                        } else {
                            b.Label("Реле не найдены.'Сканировать реле'");
                        }

                        b.Label("Смена адреса реле:");
                        b.Number(kk::old_address, "Текущий адрес (1-127)");
                        b.Number(kk::new_address, "Новый адрес (1-127)");

                        if (b.Button(0x1002, "Изменить адрес", sets::Colors::Blue)) {
                            measureExecutionTime("change_relay_address", []() {  // измерение времени выполнения функции
                                data.old_address = db[kk::old_address].toInt();
                                data.new_address = db[kk::new_address].toInt();
                                change_relay_address();
                            });
                            db.update();
                        } 
                    }
                }

            }  // настройки
        }  // Подстройки
    }  // меню

    unsigned long totalTime = millis() - startTime;
    Serial.print("Общее время build(): ");
    Serial.print(totalTime);
    Serial.println(" мс");
}  // builder