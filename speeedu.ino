#include <SPEEEduino_LowLevel_Library.h>
#include <ArduinoJson.h>

SPEEEduino_LowLevel device = SPEEEduino_LowLevel();
String reqData = "GET / HTTP/1.1\r\nHost: xdk.herokuapp.com\r\n\r\n";
String reqMail = "GET /alert HTTP/1.1\r\nHost: xdk.herokuapp.com\r\n\r\n";

int buzzPin = 10;
int fanPin = 11;
int heatPin = 12;
int ledPin = 13;

void setup() {
  pinMode(buzzPin, OUTPUT);
  pinMode(fanPin, OUTPUT);
  pinMode(heatPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  tone(buzzPin, 440);
  Serial.begin(9600);
  device.slowOpenLink();
  device.setDHCPEnabled(true);
  device.setConnectionAmount(SINGLE);
  device.setWiFiMode(STATION);
  device.joinAP("FABLAB14", "fabpass123");
  device.beginSingleConnection(TCP, "xdk.herokuapp.com", "80");
  Serial.println("SUCCESS setup");
}

void loop() {
  device.sendDataSingleConnection(reqData);
  String data = device.receiveData(SINGLE).content;

  int i;
  char* json;
  for (i=0;i<data.length();i++) {
    if (data[i] == '{') {
      json = &(data[i]);
    }
  }

  Serial.println(json);

  StaticJsonBuffer<100> jsonBuffer;

  JsonObject& root = jsonBuffer.parseObject(json);

  double humidity    = root["humidity"];
  double millilux    = root["millilux"];
  double noiselevel  = root["noiselevel"];
  double temperature = root["temperature"];

  Serial.println(humidity);
  Serial.println(millilux);
  Serial.println(noiselevel);
  Serial.println(temperature);

  if (millilux < 1000) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }

  if (temperature < 25) {
    // Trigger heater
  } else {
    // Switch off heater
  }

  // Remember to add cooldown condition below, otherwise the emails will pile up
  if (false) { // Infrared sensor
    device.sendDataSingleConnection(reqMail);
    String res = device.receiveData(SINGLE).content;

    int j;
    char* sent;
    for (j=0;j<res.length();j++) {
      if (res[j] == '{') {
        sent = &(res[j]);
      }
    }

    Serial.println(sent);

    StaticJsonBuffer<100> jsonBuffer;

    JsonObject& sentObj = jsonBuffer.parseObject(sent);

    String sentStatus = sentObj["sent"];

    if (sentStatus == "OK") {
      // Restart cooldown
    }
  }

  Serial.println("SUCCESS loop");
  delay(5000);
}

