#include <Arduino.h>
#include <ESP8266WiFi.h>

#include <TimeLib.h>
#include <NtpClientLib.h>
#include <PubSubClient.h>

#include <SerialService.h>
#include <WiFiService.h>
#include <TimeService.h>
#include <TempService.h>
#include <MQTTService.h>

#define LED D0
#define NTP_TIMEOUT 1500
#define THUMBPRINT "8E:50:2E:6E:22:25:9A:D9:44:16:D7:BF:2A:38:7D:6E:05:D3:F8:3A"
#define MQTT_USER "FlintSoftPool.azure-devices.net/PoolTemp/?api-version=2018–06–30"
#define MQTT_DEVICE "PoolTemp"
#define MQTT_ENDPOINT "FlintSoftPool.azure-devices.net"
#define MQTT_PWD "SharedAccessSignature sr=FlintSoftPool.azure-devices.net&sig=rfA8Fqp3emTDqyumXq%2BBgLVbsbgz9cKb9jjlSFylWi0%3D&se=1642196248&skn=iothubowner"
const char* MQTT_PUB_TOPIC = "devices/PoolTemp/messages/events/";

//NTP Settings
int8_t timeZone = 5;
int8_t minutesTimeZone = 30;
const PROGMEM char *ntpServer = "pool.ntp.org";
boolean syncEventTriggered = false; //faprocessSyncEventlse; // True if a time even has been triggered
NTPSyncEvent_t ntpEvent; // Last triggered event

BearSSL::WiFiClientSecure tlsClient;
PubSubClient client(tlsClient);

void setup() {
  // put your setup code here, to run once:

  // Serial.begin(9600);
  // Serial.println("FlintSoft Demo");

  // pinMode(LED, OUTPUT);

  // sensor.begin();
  initSerial();

  initWiFi("PRATTNET", "Lilly&Flint@Gartenweg33");

  //Initialize NTP
  NTP.onNTPSyncEvent ([](NTPSyncEvent_t event) {
      ntpEvent = event;
      syncEventTriggered = true;
  });

  NTP.setInterval (63);
  NTP.setNTPTimeout (NTP_TIMEOUT);
  NTP.begin (ntpServer, timeZone, true, minutesTimeZone);

  initTempSensors();

  initMQTT(tlsClient, client, THUMBPRINT, MQTT_ENDPOINT);

  connectToIOTHub(client, MQTT_DEVICE, MQTT_USER, MQTT_PWD);
}

void loop() {
  if(WiFi.isConnected()){
    if(syncEventTriggered){
      processSyncEvent(ntpEvent);
      syncEventTriggered = false;
    }

    if(!client.connected()) {
      connectToIOTHub(client, MQTT_DEVICE, MQTT_USER, MQTT_PWD);
    }

    client.loop();

    if(client.connected()) {
      const char* tempPayload;
      tempPayload =  readTemp();
      if(tempPayload != "ERROR") {
        client.publish(MQTT_PUB_TOPIC, tempPayload);
      }
    }
  } else {
    reConnectWiFi("PRATTNET");
  }



  delay(1000);
  // put your main code here, to run repeatedly:
  // digitalWrite(LED, LOW);
  // delay(500);
  // digitalWrite(LED, HIGH);
  // delay(500);


}

