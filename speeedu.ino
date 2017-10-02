#include <SPEEEduino_LowLevel_Library.h>
#include <ArduinoJson.h>

SPEEEduino_LowLevel device = SPEEEduino_LowLevel();
String payload = "GET / HTTP/1.1\r\nHost: xdk.herokuapp.com\r\n\r\n";

void setup() {
  Serial.begin(9600);
  device.slowOpenLink();
  device.setDHCPEnabled(true);
  device.setConnectionAmount(SINGLE);
  device.setWiFiMode(STATION);
  device.joinAP("FABLAB14", "fabpass123");
  Serial.println("SUCCESS setup");
}

void loop() {
  device.beginSingleConnection(TCP, "xdk.herokuapp.com", "80");
  device.sendDataSingleConnection(payload);
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

  Serial.println("SUCCESS loop");
  delay(5000);
}

