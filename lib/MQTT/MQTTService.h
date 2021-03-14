#include <ESP8266WiFi.h>
#include <PubSubClient.h>

void initMQTT(BearSSL::WiFiClientSecure &tlsClient, PubSubClient &client, const char *thumb, const char *domaain);
void connectToIOTHub(PubSubClient &client, const char *deviceId, const char *user, const char *pwd);