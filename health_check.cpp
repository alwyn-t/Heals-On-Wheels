#include <Adafruit_AHTX0.h>

#include <Wire.h>
#include "MAX30105.h"

#include "heartRate.h"

#include "health_check.h"

Adafruit_AHTX0 aht;
MAX30105 particleSensor;

void setupAHT() {
  Serial.println("Initializing AHT module");
  if (! aht.begin(&Wire, (int32_t)0, (uint8_t)56)) {
    Serial.println("Could not find AHT. Please check the Temperature, Humidity and Pressure (Purple) module.");
    while (1) delay(10);
  }
  Serial.println("AHT20 found");
}

void setupMAX30105() {
  Serial.println("Initializing MAX30105 module");
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) //Use default I2C port, 400kHz speed
  {
    Serial.println("Could not find MAX30105. Please check the Heart Rate Monitor (Black) module.");
    while (1);
  }
  particleSensor.setup(); // Configure sensor with default settings
  particleSensor.setPulseAmplitudeRed(0); // Turn Red LED to low to indicate sensor is running
  particleSensor.setPulseAmplitudeGreen(0); // Turn off Green LED
  Serial.println("MAX30105 found");
}



//const long samplingTime = 5000;

double getHeartRate() {
  long lastPulse = 0;
  long dt = millis() - lastPulse;
  float pulsesPerMinute = 60 / (dt / 1000.);
  float totalTime = 0;
  float totalPulse = 0;

  long infr = particleSensor.getIR();
  particleSensor.setPulseAmplitudeRed(0x0A); // Turn on RED LED to show the sensor is running
  Serial.println("Please place your index finger firmly onto the sensor");
  while (infr < 50000) {
    delay(10);
    infr = particleSensor.getIR();
  }; // not close enough
  long startTime = millis();
  particleSensor.setPulseAmplitudeGreen(0x0A); // Turn on GREEN LED to show the sensor is running
  Serial.println("Please hold this pressure for 5 seconds");
  while (millis() < startTime + samplingTime) { // run sampling for 5 seconds
    // Serial.println(dt);
    infr = particleSensor.getIR();
    dt = millis() - lastPulse;
    lastPulse = millis();
    pulsesPerMinute = 60.0 / (dt / 1000.0); // 3000
    // if (20 < pulsesPerMinute && pulsesPerMinute < 255) {
    if (20 < pulsesPerMinute) {
      totalTime += dt;
      totalPulse += pulsesPerMinute;
    }
    if (infr < 50000) {
      Serial.println("Measurement was not accurate, please try again");
      Serial.println("Heart rate sample cancelled");
      return (double)-1;
    }
  }
  particleSensor.setPulseAmplitudeRed(0); // Turn off RED LED to show the sensor is running
  particleSensor.setPulseAmplitudeGreen(0); // Turn off GREEN LED to show the sensor is running

  Serial.printf("Pulse: %f\n", totalPulse);
  Serial.printf("Time: %f\n", totalTime);
  Serial.printf("Measured Heart Rate Reading: %fBPM\n", totalPulse / totalTime);

  return (double)(totalPulse / totalTime);
}

double getTemperature() {
  sensors_event_t temp;
  aht.getTemperatureSensor()->getEvent(&temp);
  Serial.printf("Current Temperature Reading: %f℃ (degrees C)\n", temp.temperature);
  return (double)(temp.temperature);
}

double getHumidity() {
  sensors_event_t humi;
  aht.getTemperatureSensor()->getEvent(&humi);
  Serial.printf("Current Humidity Reading: %f% rH\n", humi.relative_humidity);
  return (double)(humi.relative_humidity);
}

int examinePatient(std::string patientName) {
  double heartRate = getHeartRate();
  if (heartRate == -1) {
    return -1;
  }
  double temp = getTemperature();
  double humi = getHumidity();

  Serial.printf("Examination of patient %s: %frh\n", patientName.c_str(), heartRate);
  Serial.printf("Heart Rate: %fBPM\n", heartRate);
  Serial.printf("Temperature: %f℃ (degrees C)\n", temp);
  Serial.printf("Humidity: %f% rH\n", humi);
  return 0;
}

