#ifndef HEALTH_CHECK
#define HEALTH_CHECK
#include <Adafruit_AHTX0.h>

#include <Wire.h>
#include "MAX30105.h"

#include "heartRate.h"

//extern Adafruit_AHTX0 aht;
//extern MAX30105 particleSensor;

void setupAHT();

void setupMAX30105();


const long samplingTime = 5000;
double getHeartRate();

double getTemperature();

double getHumidity();

int examinePatient(std::string patientName);

#endif