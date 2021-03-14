#include <ESP8266WiFi.h>
#include <PubSubClient.h>

BearSSL::WiFiClientSecure localTlsClient;
PubSubClient localClient;
const char *localThumb;
const char *localDomain;

void OnMsgCallBack(char *topic, byte *payload, unsigned int length)
{
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    for (int i = 0; i < length; i++)
    {
        Serial.print((char)payload[i]);
    }
    Serial.println();
}

void connectToIOTHub(PubSubClient &client, const char *deviceId, const char *user, const char *pwd)
{
    while (!client.connected())
    {
        Serial.print("Connecting to MQTT Server ... ");
        if (client.connect(deviceId, user, pwd))
        {
            Serial.println("connected.");
        }
        else
        {
            Serial.print("failed, status code =");
            Serial.print(client.state());
            Serial.println(". Try again in 5 seconds.");
            /* Wait 5 seconds before retrying */
            delay(5000);
        }
    }
}

void initMQTT(BearSSL::WiFiClientSecure &tlsClient, PubSubClient &client, const char *thumb, const char *domaain)
{
    localTlsClient = tlsClient;
    localClient = client;
    localThumb = thumb;
    localDomain = domaain;

    tlsClient.setFingerprint(thumb);
    client.setServer(domaain, 8883);
    client.setCallback(OnMsgCallBack);
}