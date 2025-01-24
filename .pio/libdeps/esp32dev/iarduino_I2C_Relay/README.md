[![](https://iarduino.ru/img/logo.svg)](https://iarduino.ru)[![](https://wiki.iarduino.ru/img/git-shop.svg?3)](https://iarduino.ru) [![](https://wiki.iarduino.ru/img/git-wiki.svg?2)](https://wiki.iarduino.ru) [![](https://wiki.iarduino.ru/img/git-lesson.svg?2)](https://lesson.iarduino.ru)[![](https://wiki.iarduino.ru/img/git-forum.svg?2)](http://forum.trema.ru)

# iarduino\_I2C\_Relay

**This is a library for Arduino IDE. It allows to control [Mechanical Relays](https://iarduino.ru/shop/Expansion-payments/rele-2ch-i2c.html), [Solidstate Relays](https://iarduino.ru/shop/Expansion-payments/solid-rele-4ch-i2c.html) and [Power Mosfets](https://iarduino.ru/search/?text=%D1%81%D0%B8%D0%BB%D0%BE%D0%B2%D1%8B%D1%85+%D0%BA%D0%BB%D1%8E%D1%87%D0%B5%D0%B9) I2C modules made by iArduino.ru**

**Данная библиотека для Arduino IDE позволяет управлять модулями I2C [твердотельных реле](https://iarduino.ru/shop/Expansion-payments/solid-rele-4ch-i2c.html), [механических реле](https://iarduino.ru/shop/Expansion-payments/rele-2ch-i2c.html) и [силовых ключей](https://iarduino.ru/search/?text=%D1%81%D0%B8%D0%BB%D0%BE%D0%B2%D1%8B%D1%85+%D0%BA%D0%BB%D1%8E%D1%87%D0%B5%D0%B9) от [iArduino.ru](https://iarduino.ru)**

> Подробнее про установку библиотеки читайте в нашей [инструкции](https://wiki.iarduino.ru/page/Installing_libraries/).

> Подробнее про подключение к [Arduino UNO](https://iarduino.ru/shop/boards/arduino-uno-r3.html)/[Piranha UNO](https://iarduino.ru/shop/boards/piranha-uno-r3.html) читайте на нашей [wiki](https://wiki.iarduino.ru/)


| Модель | Ссылка на магазин |
|---|---|
| Модуль электромеханических реле, 2 канала <img src="https://wiki.iarduino.ru/img/resources/1157/1157.svg" width="150px"></img>| https://iarduino.ru/shop/Expansion-payments/rele-2ch-i2c.html |
| Модуль твердотельных реле, 4 канала <img src="https://wiki.iarduino.ru/img/resources/1158/1158.svg" width="150px"></img>| https://iarduino.ru/shop/Expansion-payments/solid-rele-4ch-i2c.html |
| Скоро здесь будут силовые ключи |  |


## Назначение функций:

```C++
#include <iarduino_I2C_Relay> // Подключаем библиотеку iarduino_I2C_Relay.

iarduino_I2C_Relay ОБЪЕКТ( АДРЕС ); // Создаём объект, указывая адрес модуля на шине I2C.

iarduino_I2C_Relay ОБЪЕКТ; // Или создаём объект без указания адреса модуля на шине I2C.
```

**Инициализация работы** 

```C++
ОБЪЕКТ.begin(); // Инициализация работы с модулем.
```

**Перезагрузка модуля**

```C++
ОБЪЕКТ.reset(); // Перезагрузка модуля.
```

**Смена адреса** 

```C++
ОБЪЕКТ.changeAddress( АДРЕС ); // Смена адреса модуля на шине I2C.
```

**Запрос текущего адреса** 

```C++
ОБЪЕКТ.getAddress(); // Запрос текущего адреса модуля на шине I2C.
```

**Запрос версии прошивки** 

```C++
ОБЪЕКТ.getVersion(); // Запрос версии прошивки модуля.
```

**Запрос типа модуля** 

```C++
ОБЪЕКТ.getModel(); // Запрос типа модуля.
```

**Установка логического уровня на канале** 

```C++
ОБЪЕКТ.digitalWrite( КАНАЛ , УРОВЕНЬ ); // Установка логического уровня на канале модуля.
```

**Чтение логического уровня с канала** 

```C++
ОБЪЕКТ.digitalRead( КАНАЛ ); // Чтение логического уровня с канала модуля.
```

**Установка сигнала ШИМ на канале (только модули силовых ключей)** 

```C++
ОБЪЕКТ.analogWrite( КАНАЛ , УРОВЕНЬ ); // Установка сигнала ШИМ на канале модуля.
```

**Чтение уровня сигнала ШИМ (только модули силовых ключей)** 

```C++
ОБЪЕКТ.analogRead( КАНАЛ ); // Чтение уровня сигнала ШИМ с канала модуля.
```

**Чтение силы тока на канале (только модули силовых ключей с возможостью измерения тока)** 

```C++
ОБЪЕКТ.currentRead( КАНАЛ ); // Чтение силы тока проходящего через канал модуля.
```

**Установка защиты по току на канале (только модули силовых ключей с возможостью измерения тока)** 

```C++
ОБЪЕКТ.setCurrentProtection( КАНАЛ , ТОК , РЕЖИМ ); // Установка защиты канала модуля по току.
```

**Отключение защиты по току на канале (только модули силовых ключей с возможостью измерения тока)**

```C++
ОБЪЕКТ.delCurrentProtection( КАНАЛ ); // Отключение защиты канала модуля от превышения тока.
```

**Перезапуск защиты по току на канале (только модули силовых ключей с возможостью измерения тока)** 

```C++
ОБЪЕКТ.resCurrentProtection( КАНАЛ ); // Перезапуск защиты канала модуля от превышения тока.
```

**Проверка выполнения по току на канале (только модули силовых ключей с возможостью измерения тока)** 

```C++
ОБЪЕКТ.getCurrentProtection( КАНАЛ ); // Проверка выполнения защиты по току на канале модуля.
```

**Установка частоты (только модули силовых ключей)** 

```C++
ОБЪЕКТ.freqPWM( ЧАСТОТА ); // Установка частоты ШИМ для всех каналов модуля.
```

**Калибровка измерений по току на канале (только модули силовых ключей с возможостью измерения тока)** 

```C++
ОБЪЕКТ.currentWrite( КАНАЛ , ТОК ); // Калибровка измерений тока.
```
