#include "userTimers.h"

#include <Arduino.h>
#include <iarduino_I2C_Relay.h> 


#include "data.h"
#include "nastroyki.h"
#include "settings.h"
#include "reley.h"

static bool t6_rightDay = 0;  // День недели для таймера 6
static uint8_t lastWeekDay = 0;

void userDhtRelays() {  // === термореле DHT1 AirTemp для нагрев  воздуха

    switch (data.Air1.StateAir) {  
        // инициализация
        //  ползунок включен - отрабатываем
        // выключен и включено реле - уйдем на выключение
        case 0:
            if (db[kk::airTempRele_enabled].toInt() != 0) {
                data.Air1.StateAir = 5;
            } else if (data.Air1.TempRele_on) {
                data.Air1.StateAir = 20;  // выключим по перемещению ползунка в OFF
            }
            break;
        case 5:                                          
            if (data.Air1.tx10 <= data.Air1.tTrigx10 - data.Air1.tTresholdx10) {  // data.dhtOne.tx10 >= data.dhtOne.tTrigx10) { // ожидание понижение температуры
                data.Air1.StateAir = 10;
            }
            break;
        case 10:  // включаем нагрев
            reley_Air_Temt_on();
            data.Air1.TempRele_on = true;
            data.Air1.StateAir = 15;
            break;
        case 15:  // ожидаем повышение температуры - трешхолд
            if (data.Air1.tx10 >= data.Air1.tTrigx10 ) {
                data.Air1.StateAir = 20;
            }
            break;
        case 20:  // используется при переключении ползунка в морде
            reley_Air_Temp_off();
            data.Air1.TempRele_on = false;
            data.Air1.StateAir = 0;
            break;
    }  // switch (dht1State)

    // реле Датчика почвы 1 для увлажнения почвы
    switch (data.Soil1.StateHume) {  //(data.dhtTwo.State)
        // инициализация
        //  ползунок включен - отрабатываем
        // выключен и включено реле - уйдем на выключение
        case 0:
            if (db[kk::soilHumeRele_enabled].toInt() != 0) {
                data.Soil1.StateHume = 5;
            } else if (data.Soil1.HumeRele_on) {
                data.Soil1.StateHume = 20;  // выключим по перемещению ползунка в OFF
            }
            break;
        case 5:   // ожидание понижения влажности
            if (data.Soil1.hx10 <= data.Soil1.hTrigx10) { 
                data.Soil1.StateHume = 10;
            }
            break;
        case 10:  // включаем полив
            reley_Soil_on();
            data.Soil1.HumeRele_on = true;
            data.Soil1.StateHume = 15;
            break;
        case 15:                                                                  // ожидаем повышения влажности + трешхолд
            if (data.Soil1.hx10 >= data.Soil1.hTrigx10 + data.Soil1.hTresholdx10) {  //(data.dhtTwo.hum >= data.dhtTwo.hTrig + data.dhtTwo.hTreshold) {
                data.Soil1.StateHume = 20;
            }
            break;
        case 20:  // используется при переключении ползунка в морде
            // выключаем полив
            reley_Soil_off();
            // relay1.digitalWrite(1,LOW);
            data.Soil1.HumeRele_on = false;
            data.Soil1.StateHume = 0;
            break;
    }   // switch (dhtTwo.State)
        // реле Датчика почвы 2 для увлажнения почвы
    switch (data.Soil2.StateHume) {  //(data.dhtTwo.State)
        // инициализация
        //  ползунок включен - отрабатываем
        // выключен и включено реле - уйдем на выключение
        case 0: 
            if (db[kk::soil2HumeRele_enabled].toInt() != 0) {
                data.Soil2.StateHume = 5;
            } else if (data.Soil2.HumeRele_on) {
                data.Soil2.StateHume = 20;  // выключим по перемещению ползунка в OFF
            }
            break;
        case 5:   // ожидание понижения влажности
            if (data.Soil2.hx10 <= data.Soil2.hTrigx10) { 
                data.Soil2.StateHume = 10;
            }
            break;  
        case 10:  // включаем полив
            reley_Soil_on();
            data.Soil2.HumeRele_on = true;
            data.Soil2.StateHume = 15;
            break;  
        case 15:  // ожидаем повышения влажности + трешхолд
            if (data.Soil2.hx10 >= data.Soil2.hTrigx10 + data.Soil2.hTresholdx10) {  //(data.dhtTwo.hum >= data.dhtTwo.hTrig + data.dhtTwo.hTreshold) {
                data.Soil2.StateHume = 20;
            }
            break;  
        case 20:  // используется при переключении ползунка в морде
            // выключаем полив
            reley_Soil_off();
            // relay1.digitalWrite(1,LOW);
            data.Soil2.HumeRele_on = false;
            data.Soil2.StateHume = 0;
            break;  
    }  // switch (data.Soil2.StateHume)
}  // userDhtRelays()

void userSixTimers() { // Таймеры с1 - по 6 ===
   
    // === таймер Реле 1
    // if (db[kk::t1Discr_enabled].toBool()) {
    if (data.t1discr_enbl) {
        if (db[kk::t1Discr_startTime].toInt() < db[kk::t1Discr_endTime].toInt())  // если нет перехода через полночь
        {
            if ((db[kk::t1Discr_startTime].toInt() <= data.secondsNow) && (data.secondsNow <= db[kk::t1Discr_endTime].toInt())) {
                if (!data.rel1_on)  // avoid extra digWrite
                {
                   // digitalWrite(RELE_1, ON);
                   reley_1_on();
                    data.rel1_on = 1;
                }
            } else {
                if (data.rel1_on)  // avoid extra digWrite
                {
                    //digitalWrite(RELE_1, OFF);
                    reley_1_off();
                    data.rel1_on = 0;
                }
            }
        } else if (db[kk::t1Discr_startTime].toInt() > db[kk::t1Discr_endTime].toInt())  // если есть переход через полночь
        {
            if ((db[kk::t1Discr_startTime].toInt() >= data.secondsNow) && (data.secondsNow >= db[kk::t1Discr_endTime].toInt())) {
                if (data.rel1_on)  // avoid extra digWrite
                {
                    //digitalWrite(RELE_1, OFF);
                    reley_1_off();
                    data.rel1_on = 0;
                }
            } else {
                if (!data.rel1_on)  // avoid extra digWrite
                {
                    //digitalWrite(RELE_1, ON);
                    reley_1_on();
                    data.rel1_on = 1;
                }
            }
        }
    } else {
        if (data.rel1_on)  // если было включено, выключим
        {
            //digitalWrite(RELE_1, OFF);
            reley_1_off();
            data.rel1_on = 0;
        }
    }
    // таймер 2 ===
    //=== таймер Реле 2
    // if (db[kk::t2Discr_enabled].toBool()) {
    if (data.t2discr_enbl) {
        if (db[kk::t2Discr_startTime].toInt() < db[kk::t2Discr_endTime].toInt())  // если нет перехода через полночь
        {
            if ((db[kk::t2Discr_startTime].toInt() <= data.secondsNow) && (data.secondsNow <= db[kk::t2Discr_endTime].toInt())) {
                if (!data.rel2_on)  // avoid extra digWrite
                {
                   // digitalWrite(RELE_2, ON);
                    data.rel2_on = 1;
                }
            } else {
                if (data.rel2_on)  // avoid extra digWrite
                {
                   // digitalWrite(RELE_2, OFF);
                    data.rel2_on = 0;
                }
            }
        } else if (db[kk::t2Discr_startTime].toInt() > db[kk::t2Discr_endTime].toInt())  // если есть переход через полночь
        {
            if ((db[kk::t2Discr_startTime].toInt() >= data.secondsNow) && (data.secondsNow >= db[kk::t2Discr_endTime].toInt())) {
                if (data.rel2_on)  // avoid extra digWrite
                {
                   // digitalWrite(RELE_2, OFF);
                    data.rel2_on = 0;
                }
            } else {
                if (!data.rel2_on)  // avoid extra digWrite
                {
                  //  digitalWrite(RELE_2, ON);
                    data.rel2_on = 1;
                }
            }
        }
    } else {
        if (data.rel2_on)  // если было включено, выключим
        {
          //  digitalWrite(RELE_2, OFF);
            data.rel2_on = 0;
        }
    }
    // таймер 3 ===
    //=== таймер Реле 3
    // if (db[kk::t3Discr_enabled].toBool()) {
    if (data.t3discr_enbl) {
        if (db[kk::t3Discr_startTime].toInt() < db[kk::t3Discr_endTime].toInt())  // если нет перехода через полночь
        {
            if ((db[kk::t3Discr_startTime].toInt() <= data.secondsNow) && (data.secondsNow <= db[kk::t3Discr_endTime].toInt())) {
                if (!data.rel3_on)  // avoid extra digWrite
                {
                   // digitalWrite(RELE_3, ON);
                    data.rel3_on = 1;
                }
            } else {
                if (data.rel3_on)  // avoid extra digWrite
                {
                   // digitalWrite(RELE_3, OFF);
                    data.rel3_on = 0;
                }
            }
        } else if (db[kk::t3Discr_startTime].toInt() > db[kk::t3Discr_endTime].toInt())  // если есть переход через полночь
        {
            if ((db[kk::t3Discr_startTime].toInt() >= data.secondsNow) && (data.secondsNow >= db[kk::t3Discr_endTime].toInt())) {
                if (data.rel3_on)  // avoid extra digWrite
                {
                  //  digitalWrite(RELE_3, OFF);
                    data.rel3_on = 0;
                }
            } else {
                if (!data.rel3_on)  // avoid extra digWrite
                {
                   // digitalWrite(RELE_3, ON);
                    data.rel3_on = 1;
                }
            }
        }
    } else {
        if (data.rel3_on)  // если было включено, выключим
        {
           // digitalWrite(RELE_3, OFF);
            data.rel3_on = 0;
        }
    }
    // таймер 4 ===
    //=== таймер Реле4
    // if (db[kk::t4Discr_enabled].toBool()) {
    if (data.t4discr_enbl) {
        if (db[kk::t4Discr_startTime].toInt() < db[kk::t4Discr_endTime].toInt())  // если нет перехода через полночь
        {
            if ((db[kk::t4Discr_startTime].toInt() <= data.secondsNow) && (data.secondsNow <= db[kk::t4Discr_endTime].toInt())) {
                if (!data.rel4_on)  // avoid extra digWrite
                {
                  //  digitalWrite(RELE_4, ON);
                    data.rel4_on = 1;
                }
            } else {
                if (data.rel4_on)  // avoid extra digWrite
                {
                   // digitalWrite(RELE_4, OFF);
                    data.rel4_on = 0;
                }
            }
        } else if (db[kk::t4Discr_startTime].toInt() > db[kk::t4Discr_endTime].toInt())  // если есть переход через полночь
        {
            if ((db[kk::t4Discr_startTime].toInt() >= data.secondsNow) && (data.secondsNow >= db[kk::t4Discr_endTime].toInt())) {
                if (data.rel4_on)  // avoid extra digWrite
                {
                  //  digitalWrite(RELE_4, OFF);
                    data.rel4_on = 0;
                }
            } else {
                if (!data.rel4_on)  // avoid extra digWrite
                {
                   // digitalWrite(RELE_4, ON);
                    data.rel4_on = 1;
                }
            }
        }
    } else {
        if (data.rel4_on)  // если было включено, выключим
        {
            data.rel4_on = 0;
          // digitalWrite(RELE_4, OFF);
        }
    }
    // таймер 5===
    //=== таймер Реле 5
    // if (db[kk::t5Discr_enabled].toBool()) {
    if (data.t5discr_enbl) {
        if (db[kk::t5Discr_startTime].toInt() < db[kk::t5Discr_endTime].toInt())  // если нет перехода через полночь
        {
            if ((db[kk::t5Discr_startTime].toInt() <= data.secondsNow) && (data.secondsNow <= db[kk::t5Discr_endTime].toInt())) {
                if (!data.rel5_on)  // avoid extra digWrite
                {
                  //  digitalWrite(RELE_5, ON);
                    data.rel5_on = 1;
                }
            } else {
                if (data.rel5_on)  // avoid extra digWrite
                {
                   // digitalWrite(RELE_5, OFF);
                    data.rel5_on = 0;
                }
            }
        } else if (db[kk::t5Discr_startTime].toInt() > db[kk::t5Discr_endTime].toInt())  // если есть переход через полночь
        {
            if ((db[kk::t5Discr_startTime].toInt() >= data.secondsNow) && (data.secondsNow >= db[kk::t5Discr_endTime].toInt())) {
                if (data.rel5_on)  // avoid extra digWrite
                {
                  //  digitalWrite(RELE_5, OFF);
                    data.rel5_on = 0;
                }
            } else {
                if (!data.rel5_on)  // avoid extra digWrite
                {
                   // digitalWrite(RELE_5, ON);
                    data.rel5_on = 1;
                }
            }
        }
    } else {
        if (data.rel5_on)  // если было включено, выключим
        {
          //  digitalWrite(RELE_5, OFF);
            data.rel5_on = 0;
        }
    }

    // таймер 6===+++++++++++++++++
    // проверяем, правильный ли день для включения таймера 6
    switch (curDataTime.weekDay) {
        case 1:
            if (db[kk::t6Discr_inMonday].toInt() == 1) t6_rightDay = 1;
            else t6_rightDay = 0;
            break;
        case 2:
            if (db[kk::t6Discr_inTuesday].toInt() == 1) t6_rightDay = 1;
            else t6_rightDay = 0;
            break;
        case 3:
            if (db[kk::t6Discr_inWensday].toInt() == 1) t6_rightDay = 1;
            else t6_rightDay = 0;
            break;
        case 4:
            if (db[kk::t6Discr_inThursday].toInt() == 1) t6_rightDay = 1;
            else t6_rightDay = 0;
            break;
        case 5:
            if (db[kk::t6Discr_inFriday].toInt() == 1) t6_rightDay = 1;
            else t6_rightDay = 0;
            break;
        case 6:
            if (db[kk::t6Discr_inSaturday].toInt() == 1) t6_rightDay = 1;
            else t6_rightDay = 0;
            break;
        case 7:
            if (db[kk::t6Discr_inSunday].toInt() == 1) t6_rightDay = 1;
            else t6_rightDay = 0;
            break;
    }
    //=== таймер Реле 6
    // if (db[kk::t6Discr_enabled].toBool()) {
    if (data.t6discr_enbl && t6_rightDay) {
        if (db[kk::t6Discr_startTime].toInt() < db[kk::t6Discr_endTime].toInt())  // если нет перехода через полночь
        {
            if ((db[kk::t6Discr_startTime].toInt() <= data.secondsNow) && (data.secondsNow <= db[kk::t6Discr_endTime].toInt())) {
                if (!data.rel6_on)  // avoid extra digWrite
                {
                   // digitalWrite(RELE_6, ON);
                    rele2.digitalWrite(2,HIGH);
                    data.rel6_on = 1; 
                }
            } else {
                if (data.rel6_on)  // avoid extra digWrite
                {
                   // digitalWrite(RELE_6, OFF);
                    rele2.digitalWrite(2,LOW);
                    data.rel6_on = 0;
                }
            }
        } else if (db[kk::t6Discr_startTime].toInt() > db[kk::t6Discr_endTime].toInt())  // если есть переход через полночь
        {
            if ((db[kk::t6Discr_startTime].toInt() >= data.secondsNow) && (data.secondsNow >= db[kk::t6Discr_endTime].toInt())) {
                if (data.rel6_on)  // avoid extra digWrite
                {
                   // digitalWrite(RELE_6, OFF);
                    rele2.digitalWrite(2,LOW);
                    data.rel6_on = 0;
                }
            } else {
                if (!data.rel6_on)  // avoid extra digWrite
                {
                   // digitalWrite(RELE_6, ON);
                    rele2.digitalWrite(2,HIGH);
                    data.rel6_on = 1;
                }
            }
        }
    } else {
        if (data.rel6_on)  // если было включено, выключим
        {
           // digitalWrite(RELE_6, OFF);
           rele2.digitalWrite(2,LOW);
            data.rel6_on = 0;
        }
    }
    // таймер6 ===

}  // userSixTimers()