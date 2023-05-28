#ifdef ESP32
#ifndef T_CALL_BATTERY_H
#define T_CALL_BATTERY_H

#include <driver/adc.h>
#include "esp_adc_cal.h"
#include <vector>
#include <tuple>
#include <math.h>

#define VOLTAGE_DIVIDER_PIN 14
#define VOLTAGE_MEASUREMENT_PIN 35
#define NUM_SAMPLES 20


class TCallBattery
{
public:
    // https://lygte-info.dk/info/BatteryChargePercent%20UK.htm
    std::vector<std::pair<float,int>> battery_percentages = {
        {4.4, 100},
        {4.3, 100},
        {4.2, 100},
        {4.1, 92},
        {4.0, 78},
        {3.9, 61},
        {3.8, 43},
        {3.7, 14},
        {3.6, 3},
        {3.5, 1},
        {3.4, 0},
        {3.3, 0},
        {3.4, 0},
    };
    TCallBattery();
    ~TCallBattery();
    void initBatteryMonitor();
    void resetUnits();
    float getBatteryVoltage();
    float getSampledVoltage();
    float getAnalog();
    int getPercentage();
private:
    float analog;
    float voltage;
    int batteryLevel;
    float roundoff(float value, unsigned char prec);
};

#endif
#endif // ESP32