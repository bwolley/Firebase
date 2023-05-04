#include "DHT.h"
#define DHTTYPE DHT11
#define DHTPIN 4
DHT dht(DHTPIN, DHTTYPE);
#include <Wire.h>
#include <Adafruit_INA219.h>
Adafruit_INA219 ina219;

#define CDS A1
int measurePin = A0;
int SensorLED = 5;
float dustDensity;
int Time1 = 280;
int Time2 = 40;
int Time3 = 9680;

unsigned long lastSend, prev_time, prev_time2, prev_time3, prev_time4, prev_time5;
String c, t, h, mA, V, d, str_sensor;

void setup() {
  Serial.begin(115200);
  pinMode(SensorLED, OUTPUT);
  pinMode(measurePin, INPUT);
  if (! ina219.begin()) {
    Serial.println("Failed to find INA219 chip");
    while (1) { delay(10); }
  }
  dht.begin();
}

void loop() {
  getDHT();
  getVOLT();
  getCDS();
  getDUST();

  sensor_start();
  sensor(t);
  sensor(h);
  sensor(c);
  sensor(V);
  sensor(mA);
  sensor(d);
  sensor_end();
  send_sensor(3000);
}

void getDHT() {
  float temp = dht.readTemperature(); float humi = dht.readHumidity();
  t = String(temp); h = String(humi);
}

void getCDS() {
  int cds = analogRead(CDS);
  c = String(cds);
}

void getDUST() {
  digitalWrite(SensorLED, LOW);
  delayMicroseconds(Time1);
  float voMeasured = analogRead(measurePin);
  delayMicroseconds(Time2);
  digitalWrite(SensorLED, HIGH); 
  delayMicroseconds(Time3); 

  float calcVoltage = voMeasured * (5.0 / 1024.0); 
  float dustDensity = (calcVoltage - 0.3)/0.005;

  d = String(dustDensity);
}

void getVOLT() {
  float shuntvoltage = 0;
  float busvoltage = 0;
  float current_mA = 0;
  float loadvoltage = 0;

  shuntvoltage = ina219.getShuntVoltage_mV();
  busvoltage = ina219.getBusVoltage_V();
  current_mA = ina219.getCurrent_mA();
  loadvoltage = busvoltage + (shuntvoltage / 1000);

  V = String(loadvoltage);
  mA = String(current_mA);
}



void sensor_start() { str_sensor = "!,"; }
void sensor(String value) {
  if ( value.length() > 0 ) {
    str_sensor += value; str_sensor += ",";
  } else if ( value.length() == 0 ) {}
}
void sensor_end() { str_sensor += "@\n"; }
void send_sensor(int msec) {
  if ((millis() - prev_time2) >= msec) {
    Serial.write(str_sensor.c_str());
    prev_time2 = millis();
  }
}
