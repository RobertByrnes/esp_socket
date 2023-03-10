#include "TCallBattery.h"
#include <Arduino.h>

// Constructor
TCallBattery::TCallBattery() {}

// Destructor
TCallBattery::~TCallBattery() {}

void TCallBattery::initBatteryMonitor()
{
    esp_adc_cal_characteristics_t adc_chars;
    esp_adc_cal_value_t val_type = esp_adc_cal_characterize(
        (adc_unit_t)ADC_UNIT_1, 
        (adc_atten_t)ADC_ATTEN_DB_2_5, 
        (adc_bits_width_t)ADC_WIDTH_BIT_12, 
        1100, 
        &adc_chars
    );
    pinMode(VOLTAGE_DIVIDER_PIN, OUTPUT);
}

float TCallBattery::getBatteryVoltage()
{
    digitalWrite(VOLTAGE_DIVIDER_PIN, HIGH);
    delay(1);
    float measurement = (float) analogRead(VOLTAGE_MEASUREMENT_PIN);
    float battery_voltage = (measurement / 4095.0) * 7.26;
    digitalWrite(VOLTAGE_DIVIDER_PIN, LOW);
    return measurement*2;
}

float TCallBattery::getSampledVoltage()
{
    this->resetUnits();
    int sample_count = 0;
    while (sample_count < NUM_SAMPLES) {
        this->analog += analogRead(VOLTAGE_MEASUREMENT_PIN);
        sample_count++;
        delay(10);
    }
    this->voltage = ((this->analog/NUM_SAMPLES)/2350.0)*4.20;
    this->batteryLevel = this->getPercentage();

    if (this->analog/NUM_SAMPLES < 1200) {
        this->batteryLevel = 0;
    }

    sample_count = 0;
    return this->voltage;
}

float TCallBattery::getAnalog()
{
    return this->analog;
}

int TCallBattery::getPercentage()
{
    int index = std::distance(
        this->battery_percentages.begin(), 
        std::find_if(this->battery_percentages.begin(), 
        this->battery_percentages.end(), 
        [&](std::pair<float,int>& pair) { return pair.first == roundoff(this->voltage, 1); })
    );
    
    this->batteryLevel = this->battery_percentages[index].second;
    return this->batteryLevel;
}

void TCallBattery::resetUnits()
{
    this->analog = 0;
    this->batteryLevel = 0;
    this->analog = 0;
}

float TCallBattery::roundoff(float value, unsigned char prec)
{
  float pow_10 = std::pow(10.0f, (float)prec);
  return std::round(value * pow_10) / pow_10;
}
