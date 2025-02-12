//#define DEBUG
// имя пароль вашей домашней сети
// можно ввести, подключившись к ESP AP c паролем 1234567890
#define WIFI ""
#define WIFIPASS ""

#define INDIKATOR 2 // на каком пине индикаторный светодиод
#define BTN 0 // встроенная кнопка


// #define CHECKT1 // отладка глюков таймера природного освещения


// для тестов 
#ifndef RELE_1
#define RELE_1 LED_BUILTIN
#endif 
#ifndef RELE_2
#define RELE_2 LED_BUILTIN
#endif 
#ifndef RELE_3
#define RELE_3 LED_BUILTIN
#endif 
#ifndef RELE_4
#define RELE_4 LED_BUILTIN
#endif 
#ifndef RELE_5
#define RELE_5 LED_BUILTIN
#endif 