#include <OneWire.h>
#include <DallasTemperature.h>

#define TEMPERATURE_SENSOR_BUS 2 //(Connect to Pin 2)
#define FAN_PWM_BUS 3 //(Connect to Pin 3)
#define MALFUNCTION_LED_BUS 5 //(Connect to Pin 5)
#define TEMPERATURE_SENSOR_INDEX 0 // Index of sensors connected to data pin, default: 0.
#define TEMPERATURE_SENSOR_RESOLUTION 9 // How many bits to use for temperature values: 9, 10, 11 or 12. Lower resolution means faster measurements.
double temperature = 0.0d;

/*
9 bits: increments of 0.5C, 93.75ms to measure temperature;
10 bits: increments of 0.25C, 187.5ms to measure temperature;
11 bits: increments of 0.125C, 375ms to measure temperature;
12 bits: increments of 0.0625C, 750ms to measure temperature.
*/

OneWire signalWire(TEMPERATURE_SENSOR_BUS); //Set up a oneWire instance to communicate with any OneWire device
DallasTemperature sensors(&signalWire); //Tell Dallas Temperature Library to use oneWire Library 
DeviceAddress sensorDeviceAddress;

void activateMalfunctionLED();

void setup() {
  pinMode(TEMPERATURE_SENSOR_BUS, INPUT);
  pinMode(FAN_PWM_BUS, OUTPUT);
  pinMode(MALFUNCTION_LED_BUS, OUTPUT);
  Serial.begin(9600);
  Serial.println();
  Serial.println("Program - Projector Cooling");
  Serial.println("Temperature Sensor: DS18B20");
  sensors.begin();
  sensors.getAddress(sensorDeviceAddress, 0);
  sensors.setResolution(sensorDeviceAddress, TEMPERATURE_SENSOR_RESOLUTION);
}

void loop() {
  Serial.println();
  Serial.print("Requesting temperature...");
  sensors.requestTemperatures(); // Send the command to get temperatures. Measurement may take up to 750ms.
  Serial.println("Done");
  
  temperature = sensors.getTempCByIndex(TEMPERATURE_SENSOR_INDEX);
  
  Serial.print("Sensor 1 (at index 0) = ");
  Serial.print(temperature);
  Serial.println(" Degrees Celsius");
  
  /*
  Serial.print("Device 1 (at index 0) = ");
  Serial.print(sensors.getTempFByIndex(0));
  Serial.println(" Degrees Fahrenheit");
  */
  
  if (temperature != -127.00) {
    if (temperature <= 20.00) {
      analogWrite(FAN_PWM_BUS, 80);
    } else if (temperature <= 22.00) {
      analogWrite(FAN_PWM_BUS, 130);
    } else if (temperature <= 24.00) {
      analogWrite(FAN_PWM_BUS, 150);
    } else if (temperature <= 26.00) {
      analogWrite(FAN_PWM_BUS, 180);
    } else if (temperature <= 28.00) {
      analogWrite(FAN_PWM_BUS, 210);
    } else if (temperature <= 30.00) {
      analogWrite(FAN_PWM_BUS, 255);
    } else if (temperature <= 32.00) {
      analogWrite(FAN_PWM_BUS, 255);
    } else if (temperature <= 33.00) {
      analogWrite(FAN_PWM_BUS, 255);
    } else if (temperature <= 35.00) {
      analogWrite(FAN_PWM_BUS, 255);
    }   
  } else {
    Serial.println("Warning! Sensor malfunction.");
    Serial.println("Possible reasons: no sensor connected, failed sensor, overheating, etc.");
    Serial.println("To prevent damage from overheating the fans now run at full speed.");
    analogWrite(FAN_PWM_BUS, 255);
    activateMalfunctionLED();
  }
}

void activateMalfunctionLED() {
  for (int fade = 0; fade <= 255; fade += 5) {
    analogWrite(MALFUNCTION_LED_BUS, fade);
    delay(10);
  }
  for (int fade = 255; fade >= 0; fade -= 5) {
    analogWrite(MALFUNCTION_LED_BUS, fade);
    delay(10);
  }
}

