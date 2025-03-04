#pragma once
#include <Arduino.h>
#include "data.h"
#include "settings.h"

class Relay6Algorithms {
public:
    static bool checkTimeBasedAlgorithm();
    static bool checkTemperatureBasedAlgorithm();
    static bool checkHumidityBasedAlgorithm();
    static bool checkSoil1BasedAlgorithm();
    static bool checkSoil2BasedAlgorithm();
    
    static bool executeAlgorithm(uint8_t algorithm);

private:
    typedef bool (*AlgorithmFunc)();
    static const AlgorithmFunc algorithms[5];
}; 