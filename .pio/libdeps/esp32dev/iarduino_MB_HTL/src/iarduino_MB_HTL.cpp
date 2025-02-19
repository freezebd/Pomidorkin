#include "iarduino_MB_HTL.h"		//	╔═══════════════════════╗
									//	║ РЕГИСТР ДИАГНОСТИКИ   ║
									//	╠═══════════╤═══════════╣
									//	║ --------  │ ------10  ║ Флаги сигнализируют о ошибке чтении данных и инициализации датчика влажности и температуры SHT20 соответственно.
//	Для совместимости				//	║ --------  │ ----32--  ║ Флаги сигнализируют о ошибке чтении данных и инициализации датчика углекислого газа SGP30 соответственно.
									//	║ --------  │ --54----  ║ Флаги сигнализируют о ошибке чтении данных и инициализации датчика освещенности и приближения LTR-553ALS-01 соответственно.
									//	╚═══════════╧═══════════╝
									//	╔═══════════════╦═══════╗
									//	║ РЕГИСТРЫ «DO» ║ Биты: ║
									//	╠═══════════════╬═══════╣
#define DO_HL_DATA		0			//	║ DO_HL_DATA    ║   0   ║ Включение светодиода обнаружения устройства.
//	Для совместимости	1			//	║ DO_SGP_ENABLE ║   1   ║ Включение нагревательного элемента датчика углекислого газа (SGP30).
									//	╟───────────────╫───────╢
//						0x0100		//	║ DO_CHANGE_ID  ║   0   ║ Флаг устанавливается самостоятельно после подтверждения смены адреса на шине.
//						0x0101		//	║ DO_BAN_ID 1   ║   0   ║ Бит  запрещает изменять или устанавливать адрес 1.      Только для режима смены адреса на случайное симло.
//						...			//	║ DO_BAN_ID ... ║   0   ║ Биты запрещают изменять или устанавливать адреса 2-246. Только для режима смены адреса на случайное симло.
//						0x01F7		//	║ DO_BAN_ID 247 ║   0   ║ Бит  запрещает изменять или устанавливать адрес 147.    Только для режима смены адреса на случайное симло.
									//	╚═══════════════╩═══════╝
									//	╔═══════════════╦═══════╗
									//	║ РЕГИСТРЫ «DI» ║ Биты: ║
									//	╠═══════════════╬═══════╣
									//	║ НЕТ           ║   0   ║
									//	╚═══════════════╩═══════╝
									//	╔═══════════════╦═══════╗
									//	║ РЕГИСТРЫ «AO» ║Данные:║
									//	╠═══════════════╬═══════╣
#define AO_SHT_PERIOD	0			//	║ AO_SHT_PERIOD ║   5   ║ Период опроса датчика температуры и влажности (SHT20)            в десятых долях секунд ( 0,1...6553,5с ).   5 = 0,5 с.
//	Для совместимости	1			//	║ AO_SGP_PERIOD ║  10   ║ Период опроса датчика углекислого газа (SGP30)                   в десятых долях секунд ( 0,1...6553,5с ).  10 = 1,0 с.
#define AO_LTR_PERIOD	2			//	║ AO_LTR_PERIOD ║  10   ║ Период опроса датчика освещенности и приближения (LTR-553ALS-01) в десятых долях секунд ( 0,1...6553,5с ).  10 = 1,0 с.
									//	╟───────────────╫───────╢
//						0x0100		//	║ AO_ACK_ID     ║   0   ║ Запись значения 0xF0A5 подтверждает смену адреса на шине. При чтении всегда возвращает 0.
#define AO_ACK_SPEED	0x0101		//	║ AO_ACK_SPEED  ║   0   ║ Запись значения 0xF0A5 подтверждает смену скорости  шины. При чтении всегда возвращает 0.
#define AO_ACK_TYPE		0x0102		//	║ AO_ACK_TYPE   ║   0   ║ Запись значения 0xF0A5 подтверждает смену типа протокола. При чтении всегда возвращает 0.
//						0x0110		//	║ AO_SET_ID     ║  xxID ║ Запись приводит к смене адреса на шине, xx=00 до отключения питания, xx=FF с сохранеием в ПЗУ. Требуется подтверждение. При чтении xx=случайное число.
#define AO_SET_SPEED	0x0111		//	║ AO_SET_SPEED  ║  xxSP ║ Запись приводит к смене скорости  шины, xx=00 до отключения питания, xx=FF с сохранеием в ПЗУ. Требуется подтверждение. При чтении xx=случайное число.
#define AO_SET_TYPE		0x0112		//	║ AO_SET_TYPE   ║  xxTP ║ Запись приводит к смене типа протокола, xx=00 до отключения питания, xx=FF с сохранеием в ПЗУ. Требуется подтверждение. При чтении xx=случайное число.
#define AO_USER_DATA	0x0120		//	║ AO_USER_DATA  ║   0   ║ Пользовательское число хранится в Flash памяти модуля.
									//	╚═══════════════╩═══════╝
									//	╔═══════════════╦═══════╗
									//	║ РЕГИСТРЫ «AI» ║Данные:║
									//	╠═══════════════╬═══════╣
#define AI_VIN			0			//	║ AI_VIN        ║   0   ║ Напряжение питания шины RS-485 в мВ.
#define AI_TEM			1			//	║ AI_TEM        ║   0   ║ Температура в десятых долях °С со знаком ( -40,0...+125,0°C ).
#define AI_HUM			2			//	║ AI_HUM        ║   0   ║ Влажность   в десятых долях % ( 0,0...100,0% ).
#define AI_HUMA			3			//	║ AI_HUMA       ║   0   ║ Влажность   в сотых долях г/м3 ( 0,00...655,35г/м3 ).
//	Для совместимости	4			//	║ AI_ECO2       ║   0   ║ Эквивалент СО2, в частях на миллион ( 400ppm ... 60'000ppm ).
//	Для совместимости	5			//	║ AI_TVOC       ║   0   ║ Общее количество летучих органических соединений, в частях на миллиард ( 0ppb ... 60'000ppb ).
#define AI_LUX			6			//	║ AI_LUX        ║   0   ║ Освещённость в люксах ( 0...65535лк ).
									//	╚═══════════════╩═══════╝
																															//
//		КОНСТРУКТОР КЛАССА:																									//
		iarduino_MB_HTL::iarduino_MB_HTL(ModbusClient &obj){objModbus = &obj;}												//	&obj - ссылка на объект для работы по протоколу Modbus.
																															//
//		ФУНКЦИЯ ИНИЦИАЛИЗАЦИИ МОДУЛЯ ДАТЧИКОВ:																				//	Возвращает результат инициализации: true-успех / false-провал.
bool	iarduino_MB_HTL::begin(uint8_t id){																					//	id - адрес модуля на шине.
			objModbus->codeError=ERROR_GATEWAY_NO_DEVICE;																	//	Ошибкой выполнения данной функции может быть только отсутствие устройства.
		//	Самостоятельный поиск id устройства:																			//
			if( id==0 )						{ if( objModbus->findID(DEF_MODEL_HTL  ) ){id=objModbus->read();}}				//	Если адрес не указан, ищем адрес первого устройства с совпавшим идентификатором.
			if( id==0 )						{ if( objModbus->findID(DEF_MODEL_HTL+1) ){id=objModbus->read();}}				//	Если адрес не указан, ищем адрес первого устройства с совпавшим идентификатором.
		//	Проверяем устройство:																							//
			if( id==0 )						{ return false; }																//	Адрес устройства не указан и не найден.
			if( MB_getInfo(id)<15 )			{ return false; }																//	Устройство не найдено, или информация об устройстве неполная.
			if( objModbus->read()!=0x77 )	{ return false; }																//	Идентификатор линейки устройств не соответствует устройствам iArduino.
				objModbus->read();																							//	Индикатор пуска не проверяем (00=OFF, FF=ON).
			if( objModbus->read()!=id )		{ return false; }																//	Адрес полученный из информации об устройстве не совпадает с фактическим адресом устройства.
			uint8_t model = objModbus->read();																				//	Получаем идентификатор модели устройства.
			if( model!=DEF_MODEL_HTL && model!=(DEF_MODEL_HTL+1) ){ return false; }											//	Идентификатор устройства не совпадает с DEF_MODEL_HTL и DEF_MODEL_HTL+1.
		//	Устройство прошло проверку:																						//
			valID=id;																										//	Сохраняем адрес устройства.
			valVers=objModbus->read();																						//	Сохраняем версию прошивки устройства.
			return true;																									//
}																															//
																															//
//		ФУНКЦИЯ ПЕРЕЗАГРУЗКИ МОДУЛЯ ДАТЧИКОВ:																				//	Возвращает результат перезагрузки: true-успех / false-провал.
bool	iarduino_MB_HTL::reset(void){																						//	
			objModbus->codeError=ERROR_SYNTAX; if( !valID ){ return false; }												//	Ошибка синтаксиса, модуль не инициализирован.
			return ( MB_diagnostic(1,0xFF00)<0? false:true );																//	Выполняем команду диагностики, функция 0x0001 с очисткой журнала событий 0xFF00.
}																															//
																															//
//		ФУНКЦИЯ СМЕНЫ ID МОДУЛЯ ДАТЧИКОВ:																					//	Возвращает результат смены ID: true-успех / false-провал.
bool	iarduino_MB_HTL::changeID(uint8_t id){																				//	id - новый адрес модуля на шине (1-247).
			objModbus->codeError=ERROR_SYNTAX; if( !valID ){ return false; }												//	Ошибка синтаксиса, модуль не инициализирован.
			return MB_changeID(id);																							//	Меняем адрес устройства с valID на id, после чего valID=id. Функция возвращает 1 при успехе, 0 при неудаче.
}																															//
																															//
//		ФУНКЦИЯ СМЕНЫ СКОРОСТИ ПЕРЕДАЧИ ДАННЫХ:																				//	Возвращает результат смены скорости: true-успех / false-провал. Скорость необходимо подтвердить в течении 2 секунд.
bool	iarduino_MB_HTL::setSpeedMB(uint32_t s){																			//	s - скорость передачи данных (2400/4800/9600/19200/38400/57600/115200)
			objModbus->codeError=ERROR_SYNTAX; if( !valID ){ return false; }												//	Ошибка синтаксиса, модуль не инициализирован.
			if( s!=2400 && s!=4800 && s!=9600 && s!=19200 && s!=38400 && s!=57600 && s!=115200 ){ return false; }			//	Проверяем значение скорости.
			return MB_writeAO(AO_SET_SPEED, 0xFF00|(s/2400));																//	Записываем значение 0xFFXX в регистр "Holding Register" AO[AO_SET_SPEED]. Функция возвращает 1 при успехе, 0 при неудаче.
}																															//
																															//
//		ФУНКЦИЯ ПОДТВЕРЖДЕНИЯ СКОРОСТИ ПЕРЕДАЧИ ДАННЫХ:																		//	Возвращает результат подтверждения скорости: true-успех / false-провал. 
bool	iarduino_MB_HTL::ackSpeedMB(void){																					//
			objModbus->codeError=ERROR_SYNTAX; if( !valID ){ return false; }												//	Ошибка синтаксиса, модуль не инициализирован.
			if( !MB_writeAO(AO_ACK_SPEED, 0xF0A5) ){ return false; }														//	Записываем значение 0xF0A5 в регистр "Holding Register" AO[AO_ACK_SPEED]. Функция возвращает 1 при успехе, 0 при неудаче.
			delay(50);																										//	Ждём 50 мс, пока модуль не сохранит новую скорость в flash память.
			return true;																									//	Возвращаем флаг успеха.
}																															//
																															//
//		ФУНКЦИЯ СМЕНЫ ТИПА ПРОТОКОЛА MODBUS:																				//	Возвращает результат смены типа протокола Modbus: true-успех / false-провал. Тип необходимо подтвердить в течении 2 секунд.
bool	iarduino_MB_HTL::setTypeMB(uint8_t type){																			//	type - тип протокола Modbus (MODBUS_RTU/MODBUS_ASCII).
			objModbus->codeError=ERROR_SYNTAX; if( !valID ){ return false; }												//	Ошибка синтаксиса, модуль не инициализирован.
			if( type!=MODBUS_RTU && type!=MODBUS_ASCII ){ return false; }													//	Проверяем значение типа протокола Modbus.
			return MB_writeAO(AO_SET_TYPE, (type==MODBUS_ASCII?0xFFFF:0xFF00));												//	Записываем значение 0xFFXX в регистр "Holding Register" AO[AO_SET_TYPE]. Функция возвращает 1 при успехе, 0 при неудаче.
}																															//
																															//
//		ФУНКЦИЯ ПОДТВЕРЖДЕНИЯ ТИПА ПРОТОКОЛА MODBUS:																		//	Возвращает результат подтверждения типа протокола Modbus: true-успех / false-провал. 
bool	iarduino_MB_HTL::ackTypeMB(void){																					//
			objModbus->codeError=ERROR_SYNTAX; if( !valID ){ return false; }												//	Ошибка синтаксиса, модуль не инициализирован.
			if( !MB_writeAO(AO_ACK_TYPE, 0xF0A5) ){ return false; }															//	Записываем значение 0xF0A5 в регистр "Holding Register" AO[AO_ACK_TYPE]. Функция возвращает 1 при успехе, 0 при неудаче.
			delay(50);																										//	Ждём 50 мс, пока модуль не сохранит новый тип протокола Modbus в flash память.
			return true;																									//	Возвращаем флаг успеха.
}																															//
																															//
//		ФУНКЦИЯ ЗАПИСИ ПОЛЬЗОВАТЕЛЬСКОГО ЗНАЧЕНИЯ В FLASH ПАМЯТЬ МОДУЛЯ:													//	Возвращает результат записи: true-успех / false-провал.
bool	iarduino_MB_HTL::writeFlash(uint16_t data){																			//	data - целое беззнаковое число от 0 до 65535.
			objModbus->codeError=ERROR_SYNTAX; if( !valID ){ return false; }												//	Ошибка синтаксиса, модуль не инициализирован.
			if( !MB_writeAO(AO_USER_DATA, data) ){ return false; }															//	Записываем значение data в регистр "Holding Register" AO[AO_USER_DATA]. Функция возвращает 1 при успехе, 0 при неудаче.
			delay(50);																										//	Ждём 50 мс, пока модуль не сохранит записанное значение в flash память.
			return true;																									//	Возвращаем флаг успеха.
}																															//
																															//
//		ФУНКЦИЯ ЧТЕНИЯ ПОЛЬЗОВАТЕЛЬСКОГО ЗНАЧЕНИЯ ИЗ FLASH ПАМЯТИ:															//	Возвращает прочитанное значение, или -1 при провале чтения.
int32_t	iarduino_MB_HTL::readFlash(void){																					//
			objModbus->codeError=ERROR_SYNTAX; if( !valID ){ return -1; }													//	Ошибка синтаксиса, модуль не инициализирован.
			return MB_readAO(AO_USER_DATA);																					//	Читаем значение из регистра "Holding Register" AO[AO_USER_DATA]. Функция возвращает значение, или -1 при неудаче.
}																															//
																															//
//		ФУНКЦИЯ ПОЛУЧЕНИЯ НАПРЯЖЕНИЯ ПИТАНИЯ:																				//	Возвращает напряжение питания в Вольтах, или -1 при провале чтения.
float	iarduino_MB_HTL::getPWR(void){																						//
			objModbus->codeError=ERROR_SYNTAX; if( !valID ){ return -1.0f; }												//	Ошибка синтаксиса, модуль не инициализирован.
			int32_t Vcc = MB_readAI(AI_VIN);																				//	Читаем напряжение из регистра "Input Register" AI[AI_VIN]. Функция возвращает значение, или -1 при неудаче.
			if( Vcc<0 ){ return -1.0f; }else{ return ((float)Vcc)/1000.0f; }												//	Возвращаем -1 (ошибка), или напряжение в Вольтах.
}																															//
																															//
//		ФУНКЦИЯ ИЗМЕНЕНИЯ СОСТОЯНИЯ СВЕТОДИОДА ОБНАРУЖЕНИЯ УСТРОЙСТВА:														//	Возвращает результат изменения состояния светодиода: true-успех / false-провал.
bool	iarduino_MB_HTL::setIDLED(bool f){																					//	f - состояние светодиода (true/false)
			objModbus->codeError=ERROR_SYNTAX; if( !valID ){ return false; }												//	Ошибка синтаксиса, модуль не инициализирован.
			return MB_writeDO(DO_HL_DATA, f);																				//	Записываем f в регистр "Coil" DO[DO_HL_DATA].
}																															//
																															//
//		ФУНКЦИЯ ПОЛУЧЕНИЯ ФЛАГОВ ОШИБОК ИЗ РЕГИСТРА ДИАГНОСТИКИ:															//	Возвращает значение регистра диагностики, каждый из 16 бит которого является флагом ошибки, или -1 при провале чтения.
int32_t	iarduino_MB_HTL::getErr(void){																						//
			objModbus->codeError=ERROR_SYNTAX; if( !valID ){ return -1; }													//	Ошибка синтаксиса, модуль не инициализирован.
			return MB_diagnostic(2);																						//	Выполняем команду диагностики, номер функции диагностики 0x0002 = получить регистр диагностики. Функция возвращает данные результата выполнения функции диагностики, или -1 при неудаче.
}																															//
																															//
//		ФУНКЦИЯ ПРОВЕРКИ ДАТЧИКА МОДУЛЯ:																					//	Возвращает ERROR_(SHT/LTR)_INIT, ERROR_(SHT/LTR)_DATA, 0 если ошибок нет, или -1 при провале чтения.
int8_t	iarduino_MB_HTL::checkSensor(uint8_t sensor){																		//	sensor - проверяемый датчик SENSOR_ALL/SHT/LTR.
			int32_t i=getErr(); if( i<0 ){ return -1; }																		//	Получаем значение регистра диагностики.
			if( sensor==SENSOR_SHT ){ i &= (ERROR_SHT_INIT|ERROR_SHT_DATA); }												//	Удаляем все флаги ошибок кроме относящихся к датчику температуры и влажности.
			if( sensor==SENSOR_LTR ){ i &= (ERROR_LTR_INIT|ERROR_LTR_DATA); }												//	Удаляем все флаги ошибок кроме относящихся к датчику освещенности.
			return (int8_t) i;																								//	Возвращаем оставшиеся флаги ошибок.
}																															//
																															//
//		ФУНКЦИЯ ПОЛУЧЕНИЯ ТЕМПЕРАТУРЫ:																						//	Возвращает температуру в °С со знаком ( -40,0...+125,0°C ), или -100 при провале чтения.
float	iarduino_MB_HTL::getTEM(void){																						//
			objModbus->codeError=ERROR_SYNTAX; if( !valID ){ return -100.0f; }												//	Ошибка синтаксиса, модуль не инициализирован.
			int32_t tem = MB_readAI(AI_TEM);																				//	Читаем температуру из регистра "Input Register" AI[AI_TEM]. Функция возвращает значение, или -1 при неудаче.
			if( tem<0 ){ return -100.0f; }else{ return ((float)((int16_t)tem))/10.0f; }										//	Возвращаем -100 (ошибка), или температуру в °С.
}																															//
																															//
//		ФУНКЦИЯ ПОЛУЧЕНИЯ ОТНОСИТЕЛЬНОЙ ВЛАЖНОСТИ:																			//	Возвращает влажность в % ( 0,0...100,0% ), или -1 при провале чтения.
float	iarduino_MB_HTL::getHUM(void){																						//
			objModbus->codeError=ERROR_SYNTAX; if( !valID ){ return -1.0f; }												//	Ошибка синтаксиса, модуль не инициализирован.
			int32_t hum = MB_readAI(AI_HUM);																				//	Читаем влажность из регистра "Input Register" AI[AI_HUM]. Функция возвращает значение, или -1 при неудаче.
			if( hum<0 ){ return -1.0f; }else{ return ((float)hum)/10.0f; }													//	Возвращаем -1 (ошибка), или влажность в %.
}																															//
																															//
//		ФУНКЦИЯ ПОЛУЧЕНИЯ АБСОЛЮТНОЙ ВЛАЖНОСТИ:																				//	Возвращает влажность в г/м3 ( 0,00...655,35г/м3 ), или -1 при провале чтения.
float	iarduino_MB_HTL::getHUMA(void){																						//
			objModbus->codeError=ERROR_SYNTAX; if( !valID ){ return -1.0f; }												//	Ошибка синтаксиса, модуль не инициализирован.
			int32_t hum = MB_readAI(AI_HUMA);																				//	Читаем влажность из регистра "Input Register" AI[AI_HUMA]. Функция возвращает значение, или -1 при неудаче.
			if( hum<0 ){ return -1.0f; }else{ return ((float)hum)/100.0f; }													//	Возвращаем -1 (ошибка), или влажность в г/м3.
}																															//
																															//
//		ФУНКЦИЯ ПОЛУЧЕНИЯ ОСВЕЩЁННОСТИ:																						//	Возвращает освещённость в люксах ( 0...65535лк ), или -1 при провале чтения.
float	iarduino_MB_HTL::getLUX(void){																						//
			objModbus->codeError=ERROR_SYNTAX; if( !valID ){ return -1.0f; }												//	Ошибка синтаксиса, модуль не инициализирован.
			return (float)MB_readAI(AI_LUX);																				//	Читаем значение из регистра "Input Register" AI[AI_LUX]. Функция возвращает значение, или -1 при неудаче.
}																															//
																															//
//		ФУНКЦИЯ СМЕНЫ ПЕРИОДА ОПРОСА ДАТЧИКА:																				//	Возвращает результат изменения периода опроса: true-успех / false-провал.
bool	iarduino_MB_HTL::setPeriod(uint8_t sensor, float period){															//	sensor - SENSOR_ALL/SHT/LTR, period - период опроса в секундах ( 0,1...6553,5с ).
			objModbus->codeError=ERROR_SYNTAX; if( !valID ){ return false; }												//	Ошибка синтаксиса, модуль не инициализирован.
			uint8_t adr, sum; uint16_t data=(uint16_t)(period*10);															//
			if( sensor==SENSOR_ALL ){ adr=AO_SHT_PERIOD; sum=3; }else														//	Запись периода опроса для всх датчиков      начинается с регистра holding Register" AO[AO_SHT_PERIOD].
			if( sensor==SENSOR_SHT ){ adr=AO_SHT_PERIOD; sum=1; }else														//	Период опроса датчика влажности и температуры хранится в регистре holding Register" AO[AO_SHT_PERIOD].
			if( sensor==SENSOR_LTR ){ adr=AO_LTR_PERIOD; sum=1; }else{ return false; }										//	Период опроса датчика освещённости            хранится в регистре holding Register" AO[AO_LTR_PERIOD].
			if( sum==1 ){ return MB_writeAO(adr, data); }																	//	Записываем значение data в регистр "Holding Register" adr. Функция возвращает 1 при успехе, 0 при неудаче.
			else        { objModbus->beginTransmission(valID, HOLDING_REGISTERS, adr, 3);									//	Инициируем передачу данных в регистры "Holding Register" (AO), начиная с регистра adr, всего 3 регистра.
			              objModbus->write(data); objModbus->write(data); objModbus->write(data);							//	Ставим в очередь на запись 3 значения data.
						  return objModbus->endTransmission();																//	Выполняем запись. Функция возвращает 1 при успехе, 0 при неудаче.
			}																												//
}																															//
																															//
//		ФУНКЦИЯ ПОЛУЧЕНИЯ ПЕРИОДА ОПРОСА ДАТЧИКА:																			//	Возвращает период опроса в секундах ( 0,1...6553,5с ), или -1 при неудаче.
float	iarduino_MB_HTL::getPeriod(uint8_t sensor){																			//	sensor - SENSOR_SHT/LTR.
			objModbus->codeError=ERROR_SYNTAX; if( !valID ){ return -1.0f; }												//	Ошибка синтаксиса, модуль не инициализирован.
			uint8_t adr;																									//
			if( sensor==SENSOR_SHT ){ adr=AO_SHT_PERIOD; }else																//	Период опроса датчика влажности и температуры хранится в регистре holding Register" AO[AO_SHT_PERIOD].
			if( sensor==SENSOR_LTR ){ adr=AO_LTR_PERIOD; }else{ return false; }												//	Период опроса датчика освещённости            хранится в регистре holding Register" AO[AO_LTR_PERIOD].
			int32_t period = MB_readAO(adr);																				//	Читаем значение из регистра "holding Register" adr. Функция возвращает значение, или -1 при неудаче.
			if( period<0 ){ return -1.0f; }else{ return ((float)period)/10.0f; }											//	Возвращаем -1 (ошибка), или период опроса датчика в секундах.
}																															//
																															//
/**		ФУНКЦИИ ДУБЛЁРЫ ОБЪЕКТА objModbus ДЛЯ ЧТЕНИЯ/ЗАПИСИ С НЕСКОЛЬКИХ ПОПЫТОК	**/										//
																															//
int8_t		iarduino_MB_HTL::MB_readDO		(uint16_t reg							){ uint8_t cntRW=maxRW; while(cntRW){ int8_t   i=objModbus->coilRead            (valID, reg);        if( i>-1 ){ return i;    }else{ cntRW--; } } return -1;    }
int8_t		iarduino_MB_HTL::MB_readDI		(uint16_t reg							){ uint8_t cntRW=maxRW; while(cntRW){ int8_t   i=objModbus->discreteInputRead   (valID, reg);        if( i>-1 ){ return i;    }else{ cntRW--; } } return -1;    }
int32_t		iarduino_MB_HTL::MB_readAO		(uint16_t reg							){ uint8_t cntRW=maxRW; while(cntRW){ int32_t  i=objModbus->holdingRegisterRead (valID, reg);        if( i>-1 ){ return i;    }else{ cntRW--; } } return -1;    }
int32_t		iarduino_MB_HTL::MB_readAI		(uint16_t reg							){ uint8_t cntRW=maxRW; while(cntRW){ int32_t  i=objModbus->inputRegisterRead   (valID, reg);        if( i>-1 ){ return i;    }else{ cntRW--; } } return -1;    }
uint16_t	iarduino_MB_HTL::MB_requestFrom	(uint8_t  type, uint16_t reg, uint16_t n){ uint8_t cntRW=maxRW; while(cntRW){ uint16_t i=objModbus->requestFrom         (valID, type,reg,n); if( i> 0 ){ return i;    }else{ cntRW--; } } return  0;    }
uint8_t		iarduino_MB_HTL::MB_getInfo		(uint8_t  id							){ uint8_t cntRW=maxRW; while(cntRW){ uint8_t  i=objModbus->getInfo             (id);                if( i> 0 ){ return i;    }else{ cntRW--; } } return  0;    }
int32_t		iarduino_MB_HTL::MB_diagnostic	(uint16_t func, uint16_t data			){ uint8_t cntRW=maxRW; while(cntRW){ int32_t  i=objModbus->diagnostic          (valID, func, data); if( i>-1 ){ return i;    }else{ cntRW--; } } return -1;    }
bool		iarduino_MB_HTL::MB_changeID	(uint8_t  newID							){ uint8_t cntRW=maxRW; while(cntRW){        if( objModbus->changeID            (valID, newID)  ){ valID=newID;  return true; }else{ cntRW--; } } return false; }
bool		iarduino_MB_HTL::MB_writeDO		(uint16_t reg, bool val					){ uint8_t cntRW=maxRW; while(cntRW){        if( objModbus->coilWrite           (valID, reg, val)             ){ return true; }else{ cntRW--; } } return false; }
bool		iarduino_MB_HTL::MB_writeAO		(uint16_t reg, uint16_t val				){ uint8_t cntRW=maxRW; while(cntRW){        if( objModbus->holdingRegisterWrite(valID, reg, val)             ){ return true; }else{ cntRW--; } } return false; }
