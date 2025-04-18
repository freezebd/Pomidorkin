//	Библиотека для работы с модулями силовых ключей и реле:
//  - Модуль силовых ключей, 4N-канала 3А с измерением тока, i2c для Arduino: https://iarduino.ru/shop/Expansion-payments/power-key-4n-a-i2c.html
//  - Модуль силовых ключей, 4P-канала 3А с измерением тока, i2c для Arduino: https://iarduino.ru/shop/Expansion-payments/power-key-4p-a-i2c.html
//  - Модуль силовых ключей, 4N-канала 10A,                  i2c для Arduino: https://iarduino.ru/shop/Expansion-payments/power-key-4n-i2c.html
//  - Модуль силовых ключей, 4P-канала 10A,                  i2c для Arduino: https://iarduino.ru/shop/Expansion-payments/power-key-4p-i2c.html
//  - Модуль электромеханических реле на 2-канала,           i2c для Arduino: https://iarduino.ru/shop/Expansion-payments/rele-2ch-i2c.html
//  - Модуль твердотельных реле на 4-канала,                 i2c для Arduino: https://iarduino.ru/shop/Expansion-payments/solid-rele-4ch-i2c.html
//  Версия: 1.1.7
//  Последнюю версию библиотеки Вы можете скачать по ссылке: https://iarduino.ru/file/513.html
//  Подробное описание функции бибилиотеки доступно по ссылкам:
//  - Функции управления модулем силовых ключей, 4N-канала 3А с измерением тока: https://wiki.iarduino.ru/page/power-key-4n-a-i2c/
//  - Функции управления модулем силовых ключей, 4P-канала 3А с измерением тока: https://wiki.iarduino.ru/page/power-key-4p-a-i2c/
//  - Функции управления модулем силовых ключей, 4N-канала 10A,                : https://wiki.iarduino.ru/page/power-key-4n-i2c/
//  - Функции управления модулем силовых ключей, 4P-канала 10A,                : https://wiki.iarduino.ru/page/power-key-4p-i2c/
//  - Функции управления модулем электромеханических реле на 2-канала,         : https://wiki.iarduino.ru/page/rele-2ch-i2c/
//  - Функции управления модулем твердотельных реле на 4-канала,               : https://wiki.iarduino.ru/page/solid-rele-4ch-i2c/
//  Библиотека является собственностью интернет магазина iarduino.ru и может свободно использоваться и распространяться!
//  При публикации устройств или скетчей с использованием данной библиотеки, как целиком, так и её частей,
//  в том числе и в некоммерческих целях, просим Вас опубликовать ссылку: http://iarduino.ru
//  Автор библиотеки: Панькин Павел.
//  Если у Вас возникли технические вопросы, напишите нам: shop@iarduino.ru

#ifndef iarduino_I2C_Relay_h																											//
#define iarduino_I2C_Relay_h																											//
																																		//
#if defined(ARDUINO) && (ARDUINO >= 100)																								//
#include		<Arduino.h>																												//
#else																																	//
#include		<WProgram.h>																											//
#endif																																	//
																																		//
#include		"iarduino_I2C_Relay_I2C.h"																								//	Подключаем библиотеку выбора реализации шины I2C.
																																		//
#if defined(TwoWire_h) || defined(__ARDUINO_WIRE_IMPLEMENTATION__) || defined(__AVR_ATmega328__) || defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1284P__) || defined(__AVR_ATmega2560__) || defined(ESP8266) || defined(ESP32) || defined(ARDUINO_ARCH_RP2040) || defined(RENESAS_CORTEX_M4) // Если подключена библиотека Wire или платы её поддерживают...
#include		<Wire.h>																												//	Разрешаем использовать библиотеку Wire в данной библиотеке.
#endif																																	//
#if defined( iarduino_I2C_Software_h )																									//	Если библиотека iarduino_I2C_Software подключена в скетче...
#include		<iarduino_I2C_Software.h>																								//	Разрешаем использовать библиотеку iarduino_I2C_Software в данной библиотеке.
#endif																																	//
																																		//
#define			DEF_CHIP_ID_FLASH			0x3C																						//	ID линейки чипов - константа для всех чипов серии Flash (позволяет идентифицировать принадлежность чипа к серии).
#define			DEF_MODEL_2RM				0x0A																						//	Идентификатор модуля электромеханических реле на 2-канала        - константа.
#define			DEF_MODEL_4RT				0x0B																						//	Идентификатор модуля твердотельных реле на 4-канала              - константа.
#define			DEF_MODEL_4NC				0x0C																						//	Идентификатор модуля силовых ключей, 4N-канала с измерением тока - константа.
#define			DEF_MODEL_4PC				0x0D																						//	Идентификатор модуля силовых ключей, 4P-канала с измерением тока - константа.
#define			DEF_MODEL_4NP				0x0E																						//	Идентификатор модуля силовых ключей, 4N-канала 10A               - константа.
#define			DEF_MODEL_4PP				0x0F																						//	Идентификатор модуля силовых ключей, 4P-канала 10A               - константа.
//				Адреса регистров модулей:																								//
#define			REG_FLAGS_0					0x00																						//	Адрес регистра флагов управления для чтения.
#define			REG_BITS_0					0x01																						//	Адрес регистра битов  управления для чтения и записи.
#define			REG_FLAGS_1					0x02																						//	Адрес регистра флагов управления для чтения.
#define			REG_BITS_1					0x03																						//	Адрес регистра битов  управления для чтения и записи.
#define			REG_MODEL					0x04																						//	Адрес регистра содержащего номер типа модуля.
#define			REG_VERSION					0x05																						//	Адрес регистра содержащего версию прошивки.
#define			REG_ADDRESS					0x06																						//	Адрес регистра содержащего текущий адрес модуля на шине I2C. Если адрес указан с флагом IF-PIN-ADDRES то адрес установится (и запишется в регистр) только при наличии 1 на входе PIN_ADDRES.
#define			REG_CHIP_ID					0x07																						//	Адрес регистра содержащего ID линейки чипов «Flash». По данному ID можно определить принадлежность чипа к линейки «Flash».
#define			REG_REL_FREQUENCY_L			0x08	/* Регистр только для силовых ключей */												//	Адрес регистра частоты ШИМ в Гц (младший байт).
#define			REG_REL_FREQUENCY_H			0x09	/* Регистр только для силовых ключей */												//	Адрес регистра частоты ШИМ в Гц (старший байт).
#define			REG_REL_AVERAGING			0x0A	/* Регистр только для силовых ключей с измерением тока */							//	Адрес регистра хранящего коэффициент усреднения показаний АЦП (0-нет...255-макс).
#define			REG_REL_SHUNT				0x0B	/* Регистр только для силовых ключей с измерением тока */							//	Адрес регистра хранящего сопротивление шунтирующего резистора 1 канала силового ключа (0...255 мОм).
//				REG_REL_SHUNT 2 канала		0x0C	/* Регистр только для силовых ключей с измерением тока */							//	Адрес регистра хранящего сопротивление шунтирующего резистора 2 канала силового ключа (0...255 мОм).
//				REG_REL_SHUNT 3 канала		0x0D	/* Регистр только для силовых ключей с измерением тока */							//	Адрес регистра хранящего сопротивление шунтирующего резистора 3 канала силового ключа (0...255 мОм).
//				REG_REL_SHUNT 4 канала		0x0E	/* Регистр только для силовых ключей с измерением тока */							//	Адрес регистра хранящего сопротивление шунтирующего резистора 4 канала силового ключа (0...255 мОм).
#define			REG_REL_FLG					0x10	/* Регистр только для силовых ключей с измерением тока */							//	Адрес регистра флагов контроля защиты по максимальной силе тока (флаги сбрасываются записью логической «1»).
#define			REG_REL_BIT					0x11	/* Регистр только для силовых ключей с измерением тока */							//	Адрес регистра битов установки контроля защиты по максимальной силе тока.
#define			REG_REL_DIGITAL_ALL			0x12																						//	Адрес регистра четыре младших бита которого повторяют (устанавливают и читают) логический уровень на каналах модуля.
#define			REG_REL_DIGITAL_ONE			0x13																						//	Адрес регистра четыре старших бита которого устанавливают биты регистра «REG_REL_DIGITAL_ALL», а четыре младших бита, сбрасывают их.
#define			REG_REL_PWM					0x14	/* Регистр только для силовых ключей */												//	Адрес регистра устанавливающего ШИМ на 1 канале (младший байт).
//				REG_REL_PWM_H 1 канала		0x15	/* Регистр только для силовых ключей */												//	Адрес регистра устанавливающего ШИМ на 1 канале (старший байт).
//				REG_REL_PWM_L 2 канала		0x16	/* Регистр только для силовых ключей */												//	Адрес регистра устанавливающего ШИМ на 2 канале (младший байт).
//				REG_REL_PWM_H 2 канала		0x17	/* Регистр только для силовых ключей */												//	Адрес регистра устанавливающего ШИМ на 2 канале (старший байт).
//				REG_REL_PWM_L 3 канала		0x18	/* Регистр только для силовых ключей */												//	Адрес регистра устанавливающего ШИМ на 3 канале (младший байт).
//				REG_REL_PWM_H 3 канала		0x19	/* Регистр только для силовых ключей */												//	Адрес регистра устанавливающего ШИМ на 3 канале (старший байт).
//				REG_REL_PWM_L 4 канала		0x1A	/* Регистр только для силовых ключей */												//	Адрес регистра устанавливающего ШИМ на 4 канале (младший байт).
//				REG_REL_PWM_H 4 канала		0x1B	/* Регистр только для силовых ключей */												//	Адрес регистра устанавливающего ШИМ на 4 канале (старший байт).
#define			REG_REL_ADC					0x1C	/* Регистр только для силовых ключей с измерением тока */							//	Адрес регистра хранящего значение АЦП считанного с шунтирующего резистора 1 канала (младший байт).
//				REG_REL_ADC_H 1 канала		0x1D	/* Регистр только для силовых ключей с измерением тока */							//	Адрес регистра хранящего значение АЦП считанного с шунтирующего резистора 1 канала (старший байт).
//				REG_REL_ADC_L 2 канала		0x1E	/* Регистр только для силовых ключей с измерением тока */							//	Адрес регистра хранящего значение АЦП считанного с шунтирующего резистора 2 канала (младший байт).
//				REG_REL_ADC_H 2 канала		0x1F	/* Регистр только для силовых ключей с измерением тока */							//	Адрес регистра хранящего значение АЦП считанного с шунтирующего резистора 2 канала (старший байт).
//				REG_REL_ADC_L 3 канала		0x20	/* Регистр только для силовых ключей с измерением тока */							//	Адрес регистра хранящего значение АЦП считанного с шунтирующего резистора 3 канала (младший байт).
//				REG_REL_ADC_H 3 канала		0x21	/* Регистр только для силовых ключей с измерением тока */							//	Адрес регистра хранящего значение АЦП считанного с шунтирующего резистора 3 канала (старший байт).
//				REG_REL_ADC_L 4 канала		0x22	/* Регистр только для силовых ключей с измерением тока */							//	Адрес регистра хранящего значение АЦП считанного с шунтирующего резистора 4 канала (младший байт).
//				REG_REL_ADC_H 4 канала		0x23	/* Регистр только для силовых ключей с измерением тока */							//	Адрес регистра хранящего значение АЦП считанного с шунтирующего резистора 4 канала (старший байт).
#define			REG_REL_CURRENT				0x24	/* Регистр только для силовых ключей с измерением тока */							//	Адрес регистра хранящего значение силы тока (в мА) проходящего через 1 канал (младший байт).
//				REG_REL_CURRENT_H 1 канала	0x25	/* Регистр только для силовых ключей с измерением тока */							//	Адрес регистра хранящего значение силы тока (в мА) проходящего через 1 канал (старший байт).
//				REG_REL_CURRENT_L 2 канала	0x26	/* Регистр только для силовых ключей с измерением тока */							//	Адрес регистра хранящего значение силы тока (в мА) проходящего через 2 канал (младший байт).
//				REG_REL_CURRENT_H 2 канала	0x27	/* Регистр только для силовых ключей с измерением тока */							//	Адрес регистра хранящего значение силы тока (в мА) проходящего через 2 канал (старший байт).
//				REG_REL_CURRENT_L 3 канала	0x28	/* Регистр только для силовых ключей с измерением тока */							//	Адрес регистра хранящего значение силы тока (в мА) проходящего через 3 канал (младший байт).
//				REG_REL_CURRENT_H 3 канала	0x29	/* Регистр только для силовых ключей с измерением тока */							//	Адрес регистра хранящего значение силы тока (в мА) проходящего через 3 канал (старший байт).
//				REG_REL_CURRENT_L 4 канала	0x2A	/* Регистр только для силовых ключей с измерением тока */							//	Адрес регистра хранящего значение силы тока (в мА) проходящего через 4 канал (младший байт).
//				REG_REL_CURRENT_H 4 канала	0x2B	/* Регистр только для силовых ключей с измерением тока */							//	Адрес регистра хранящего значение силы тока (в мА) проходящего через 4 канал (старший байт).
#define			REG_REL_LIMIT				0x2C	/* Регистр только для силовых ключей с измерением тока */							//	Адрес регистра устанавливающего максимальную силу тока (в десятых долях Ампер) для функций ограничения и защиты по току для 1 канала.
//				REG_REL_LIMIT 2 канала		0x2D	/* Регистр только для силовых ключей с измерением тока */							//	Адрес регистра устанавливающего максимальную силу тока (в десятых долях Ампер) для функций ограничения и защиты по току для 2 канала.
//				REG_REL_LIMIT 3 канала		0x2E	/* Регистр только для силовых ключей с измерением тока */							//	Адрес регистра устанавливающего максимальную силу тока (в десятых долях Ампер) для функций ограничения и защиты по току для 3 канала.
//				REG_REL_LIMIT 4 канала		0x2F	/* Регистр только для силовых ключей с измерением тока */							//	Адрес регистра устанавливающего максимальную силу тока (в десятых долях Ампер) для функций ограничения и защиты по току для 4 канала.
#define			REG_REL_WDT					0x30																						//	Адрес регистра хранящего время сторожевого таймера (в сек.) оставшееся до перезагрузки модуля.
//				Позиция битов и флагов:																									//
#define			REL_FLG_A_OFF 				0																							//	Флаг регистра «REG_REL_FLG» указывающий о том что 1 канал отключён в связи с превышением силы тока.        Флаг сбрасывается записью логической 1.
//				REL_FLG_A_OFF   2 канала	1																							//	Флаг регистра «REG_REL_FLG» указывающий о том что 2 канал отключён в связи с превышением силы тока.        Флаг сбрасывается записью логической 1.
//				REL_FLG_A_OFF   3 канала	2																							//	Флаг регистра «REG_REL_FLG» указывающий о том что 3 канал отключён в связи с превышением силы тока.        Флаг сбрасывается записью логической 1.
//				REL_FLG_A_OFF   4 канала	3																							//	Флаг регистра «REG_REL_FLG» указывающий о том что 4 канал отключён в связи с превышением силы тока.        Флаг сбрасывается записью логической 1.
#define			REL_FLG_A_MAX				4																							//	Флаг регистра «REG_REL_FLG» указывающий о том что ток 1 канала ограничивается модулем на указанном уровне. Флаг сбрасывается автоматически.
//				REL_FLG_A_MAX   2 канала	5																							//	Флаг регистра «REG_REL_FLG» указывающий о том что ток 2 канала ограничивается модулем на указанном уровне. Флаг сбрасывается автоматически.
//				REL_FLG_A_MAX   3 канала	6																							//	Флаг регистра «REG_REL_FLG» указывающий о том что ток 3 канала ограничивается модулем на указанном уровне. Флаг сбрасывается автоматически.
//				REL_FLG_A_MAX   4 канала	7																							//	Флаг регистра «REG_REL_FLG» указывающий о том что ток 4 канала ограничивается модулем на указанном уровне. Флаг сбрасывается автоматически.
#define			REL_BIT_A_OFF 				0																							//	Бит  регистра «REG_REL_BIT» указывающий оключить 1 канал при превышении силы тока выше указанной.
//				REL_BIT_A_OFF   2 канала	1																							//	Бит  регистра «REG_REL_BIT» указывающий оключить 2 канал при превышении силы тока выше указанной.
//				REL_BIT_A_OFF   3 канала	2																							//	Бит  регистра «REG_REL_BIT» указывающий оключить 3 канал при превышении силы тока выше указанной.
//				REL_BIT_A_OFF   4 канала	3																							//	Бит  регистра «REG_REL_BIT» указывающий оключить 4 канал при превышении силы тока выше указанной.
#define			REL_BIT_A_MAX				4																							//	Бит  регистра «REG_REL_FLG» указывающий ограничивать силу тока 1 канала на заданном уровне.
//				REL_BIT_A_MAX   2 канала	5																							//	Бит  регистра «REG_REL_FLG» указывающий ограничивать силу тока 2 канала на заданном уровне.
//				REL_BIT_A_MAX   3 канала	6																							//	Бит  регистра «REG_REL_FLG» указывающий ограничивать силу тока 3 канала на заданном уровне.
//				REL_BIT_A_MAX   4 канала	7																							//	Бит  регистра «REG_REL_FLG» указывающий ограничивать силу тока 4 канала на заданном уровне.
#define			REL_BIT_WRITE_L				0																							//	Бит  регистра «REG_REL_DIGITAL_ONE» указывающий сбросить   бит регистра «REG_REL_DIGITAL_ALL» для 1 канала.
//				REL_BIT_WRITE_L 2 канала	1																							//	Бит  регистра «REG_REL_DIGITAL_ONE» указывающий сбросить   бит регистра «REG_REL_DIGITAL_ALL» для 2 канала.
//				REL_BIT_WRITE_L 3 канала	2																							//	Бит  регистра «REG_REL_DIGITAL_ONE» указывающий сбросить   бит регистра «REG_REL_DIGITAL_ALL» для 3 канала.
//				REL_BIT_WRITE_L 4 канала	3																							//	Бит  регистра «REG_REL_DIGITAL_ONE» указывающий сбросить   бит регистра «REG_REL_DIGITAL_ALL» для 4 канала.
#define			REL_BIT_WRITE_H				4																							//	Бит  регистра «REG_REL_DIGITAL_ONE» указывающий установить бит регистра «REG_REL_DIGITAL_ALL» для 1 канала.
//				REL_BIT_WRITE_H 2 канала	5																							//	Бит  регистра «REG_REL_DIGITAL_ONE» указывающий установить бит регистра «REG_REL_DIGITAL_ALL» для 2 канала.
//				REL_BIT_WRITE_H 3 канала	6																							//	Бит  регистра «REG_REL_DIGITAL_ONE» указывающий установить бит регистра «REG_REL_DIGITAL_ALL» для 3 канала.
//				REL_BIT_WRITE_H 4 канала	7																							//	Бит  регистра «REG_REL_DIGITAL_ONE» указывающий установить бит регистра «REG_REL_DIGITAL_ALL» для 4 канала.
																																		//
#ifndef			LOW																														//	digitalWrite( низкий логический уровень )
#define			LOW						0x00																							//
#endif																																	//
																																		//
#ifndef			HIGH																													//	digitalWrite( высокий логический уровень )
#define			HIGH					0x01																							//
#endif																																	//
																																		//
#ifndef			CURRENT_DISABLE																											//	currentWrite( канал, ток, отключать )
#define			CURRENT_DISABLE			0x00																							//
#endif																																	//
																																		//
#ifndef			CURRENT_LIMIT																											//	currentWrite( канал, ток, ограничивать )
#define			CURRENT_LIMIT			0xFF																							//
#endif																																	//
																																		//
#ifndef			ALL_CHANNEL																												//	Выбор всех выводов для функций: pinMode(), pinPull(), pinOutScheme(), digitalWrite(), digitalRead(), analogWrite(), levelRead(), servoAttach(), servoWrite(), servoWriteMicroseconds().
#define			ALL_CHANNEL				0xFF																							//	Единственная функция которой нельзя указывать све выводы: analogRead()
#endif																																	//
																																		//
class iarduino_I2C_Relay{																												//
	public:																																//
	/**	Конструктор класса **/																											//
		iarduino_I2C_Relay							(uint8_t address=0){																//	Конструктор класса													(Параметр: адрес модуля на шине I2C, если не указан (=0), то адрес будет определён).
													if(address>0x7F){ address>>=1; }													//	Корректируем адрес, если он указан с учётом бита RW.
							valAddrTemp		=		address;																			//	Сохраняем переданный адрес модуля.
							selI2C			=		new iarduino_I2C_Select;															//	Переопределяем указатель selI2C на объект производного класса iarduino_I2C_Select.
		}																																//
	/**	Пользовательские функции **/																									//
		#if defined(TwoWire_h) || defined(__ARDUINO_WIRE_IMPLEMENTATION__)																//
		bool				begin					(TwoWire* i=&Wire	){ selI2C->init(i); return _begin();	}						//	Определяем функцию инициализации модуля								(Параметр:  объект для работы с аппаратной шиной I2C).
		#endif																															//
		#if defined(iarduino_I2C_Software_h)																							//
		bool				begin					(SoftTwoWire* i		){ selI2C->init(i); return _begin();	}						//	Определяем функцию инициализации модуля								(Параметр:  объект для работы с программной шиной I2C).
		#endif																															//
		bool				reset					(void														);						//	Объявляем  функцию перезагрузки модуля								(Параметр:  отсутствует).
		bool				changeAddress			(uint8_t													);						//	Объявляем  функцию смены адреса модуля на шине I2C					(Параметр:  новый адрес модуля).
		uint8_t				getAddress				(void														){ return valAddr;	}	//	Определяем функцию возвращающую текущий адрес модуля на шине I2C	(Параметр:  отсутствует).
		uint8_t				getVersion				(void														){ return valVers;	}	//	Определяем функцию возвращающую текущую версию прошивки модуля		(Параметр:  отсутствует).
		uint8_t				getModel				(void														){ return valModel;	}	//	Определяем функцию возвращающую текущий идентификатор модели модуля	(Параметр:  отсутствует).
		bool				getPullI2C				(void														);						//	Определяем функцию возвращающую флаг наличия подтяжки шины I2C		(Параметр:  отсутствует).
		bool				setPullI2C				(bool=true													);						//	Определяем функцию установки подтяжки шины I2C						(Параметр:  флаг подтяжки).
		void				digitalWrite			(uint8_t , uint8_t											);						//	Объявляем  функцию установки логического уровня на канале реле/ключа(Параметры: номер канала, логический уровень).
		uint8_t				digitalRead				(uint8_t 													);						//	Объявляем  функцию чтения логического уровня с канала реле/ключа	(Параметр:  номер канала).
		void				analogWrite				(uint8_t , uint16_t											);						//	Объявляем  функцию установки сигнала ШИМ на канале силового ключа	(Параметры: номер канала, уровень ШИМ от 0x0000 до 0x0FFF).
		uint16_t			analogRead				(uint8_t													);						//	Объявляем  функцию чтения уровня ШИМ установленного на канале ключа	(Параметр:  номер канала).
		void				analogAveraging			(uint8_t 													);						//	Объявляем  функцию установки коэффициента усреднения показаний АЦП	(Параметры: коэффициент).
		void				freqPWM					(uint16_t													);						//	Объявляем  функцию установки частоты ШИМ							(Параметр:  частота в Гц от 1 до 12000).
		void				currentWrite			(uint8_t , float											);						//	Объявляем  функцию калибровки номинала шунтирующего резистора		(Параметры: номер канала, реально измеренная сила тока в А).
		void				currentWrite			(uint8_t , float , uint8_t&									);						//	Объявляем  функцию расчёта    номинала шунтирующего резистора		(Параметры: номер канала, реально измеренная сила тока в А, ссылка для результата в мОм).
		float				currentRead				(uint8_t													);						//	Объявляем  функцию чтения силы тока в А на канале силового ключа	(Параметр:  номер канала).
		void				setCurrentProtection	(uint8_t , float , uint8_t									);						//	Объявляем  функцию установки защиты по току на канале силового ключа(Параметры: номер канала, сила тока от 0.0 до 25.5, тип защиты).
		bool				getCurrentProtection	(uint8_t													);						//	Объявляем  функцию проверки выполнения защиты на канале ключа		(Параметр:  номер канала).
		void				delCurrentProtection	(uint8_t													);						//	Объявляем  функцию отключения защиты на канале силового ключа		(Параметр:  номер канала).
		void				resCurrentProtection	(uint8_t													);						//	Объявляем  функцию перезапуска защиты по току на канале ключа		(Параметр:  номер канала).
		bool				enableWDT				(uint8_t 													);						//	Объявляем  функцию разрешения работы   сторожевого таймера модуля	(Параметр:  время в секундах от 1 до 254).
		bool				disableWDT				(void	 													);						//	Объявляем  функцию запрета    работы   сторожевого таймера модуля	(Параметр:  отсутствует).
		bool				resetWDT				(void														);						//	Объявляем  функцию сброса (перезапуск) сторожевого таймера модуля	(Параметр:  отсутствует).
		bool				getStateWDT				(void														);						//	Объявляем  функцию получения состояния сторожевого таймера модуля	(Параметр:  отсутствует).
	private:																															//
	/**	Внутренние переменные **/																										//
		iarduino_I2C_VirtualSelect* selI2C;																								//	Объявляем  указатель  на  объект полиморфного класса iarduino_I2C_VirtualSelect, но в конструкторе текущего класса этому указателю будет присвоена ссылка на производный класс iarduino_I2C_Select.
		uint8_t				valAddrTemp		=		0;																					//	Определяем переменную для хранения адреса модуля на шине I2C который был указан, но не был проверен.
		uint8_t				valAddr			=		0;																					//	Определяем переменную для хранения адреса модуля на шине I2C.
		uint8_t				valVers			=		0;																					//	Определяем переменную для хранения версии прошивки модуля.
		uint8_t				valModel		=		0;																					//	Определяем переменную для хранения модели модуля.
		uint8_t				valAccess		=		0;																					//	Определяем переменную для хранения уровня доступа модуля: 0-нет, 1-цифровой , 2-цифровой и аналоговый, 3-цифровой, аналоговый и ток.
		uint8_t				valProtect[4]	=		{10,10,10,10};																		//	Определяем массив     для хранения установленного типа защиты по току.
		uint16_t			valFreqPWM;																									//	Объявляем  переменную для хранения установленной частоты ШИМ.
		uint8_t				valWDT			=		0xFF;																				//	Определяем переменную для хранения времени сторожевого таймера в секундах. Значение 0xFF означает, что сторожевой таймер отключён.
		uint8_t				data[8];																									//	Объявляем  массив     для хранения получаемых/передаваемых данных.
	/**	Внутренние функции **/																											//
		bool				_begin					(void														);						//	Объявляем  функцию инициализации модуля								(Параметр:  отсутствует).
		bool				_readBytes				(uint8_t	, uint8_t										);						//	Объявляем  функцию чтения данных в  массив  data					(Параметры:  номер первого регистра, количество байт).
		bool				_writeBytes				(uint8_t	, uint8_t, uint8_t=0							);						//	Объявляем  функцию записи данных из массива data					(Параметры:  номер первого регистра, количество байт, номер первого элемента массива data).
		bool				_checkModel				(uint8_t													);						//	Объявляем  функцию проверки модели модуля							(Параметры:  значение регистра 0x04 «REG_MODEL» проверяемого модуля).
};																																		//
																																		//
#endif																																	//