#include <OneWire.h>
#include <DallasTemperature.h>

#define TEMPERATURE_SENSOR_BUS 2 //(Connect to Pin 2)
#define FAN_PWM_BUS 3 //(Connect to Pin 3)
#define MALFUNCTION_LED_BUS 5 //(Connect to Pin 5)
double temperature = 0.0d;

OneWire signalWire(TEMPERATURE_SENSOR_BUS); //Set up a oneWire instance to communicate with any OneWire device
DallasTemperature sensors(&signalWire); //Tell Dallas Temperature Library to use oneWire Library 

void activateMalfunctionLED();

void setup() {
  pinMode(TEMPERATURE_SENSOR_BUS, INPUT);
  pinMode(FAN_PWM_BUS, OUTPUT);
  pinMode(MALFUNCTION_LED_BUS, OUTPUT);
  Serial.begin(9600);
  Serial.println("Program - Projector Cooling");
  Serial.println("Temperature Sensor: DS18B20");
  sensors.begin();
}


void loop() {
  Serial.println();
  Serial.print("Requesting temperature...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  Serial.println("Done");
  
  temperature = sensors.getTempCByIndex(0);
  
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

