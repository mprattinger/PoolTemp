#include <OneWire.h>
#include <DallasTemperature.h>
#include <ArduinoJson.h>
#include <NtpClientLib.h>

#define TEMP D4

OneWire oneWire(TEMP);
DallasTemperature sensor(&oneWire);

void initTempSensors() {
    sensor.begin();
}

const char* getPayload(float temp) {
  const size_t capacity JSON_OBJECT_SIZE(20);
  DynamicJsonDocument doc(capacity);

  String timestamp = NTP.getTimeDateString().c_str();

  doc["sensor"] = "PoolTemp";
  doc["data"] = temp;
  doc["timestamp"] = timestamp;

  String output;
  serializeJson(doc, output);
  return output.c_str();
}

const char* readTemp() {
  Serial.println("Requesting temperatures...");
  sensor.requestTemperatures();
  Serial.println("DONE");

  float tempC = sensor.getTempCByIndex(0);
  if(tempC != DEVICE_DISCONNECTED_C) {
     Serial.print("Temp for device 1 (index 0) is: ");
     Serial.println(tempC);
     return getPayload(tempC);
   } else {
     Serial.println("Error: Couldn't read temp data!");
     return "ERROR";
  }
}