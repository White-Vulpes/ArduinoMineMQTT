
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define WLAN_SSID       "White.Vulpes"
#define WLAN_PASS       "9381000182"

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "WhiteVulpes"
#define AIO_KEY         "aio_ZDrH45GZkpI6PNn5fFrpzAhxBrzi"

WiFiClient client;

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);


Adafruit_MQTT_Subscribe ledBrightness = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Switch");

void MQTT_connect();

uint8_t ledPin = 13;
uint16_t potAdcValue = 0;
String ledBrightValue;

void setup() {
  Serial.begin(9600);
  delay(10);

  Serial.println(F("Adafruit MQTT demo"));
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());
  pinMode(ledPin, OUTPUT);

  mqtt.subscribe(&ledBrightness);
}

void loop() {
  MQTT_connect();
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(200))) {
    if (subscription == &ledBrightness) {
      ledBrightValue = (char *)ledBrightness.lastread;
      if(ledBrightValue == "[@] ON"){
        Serial.println("in on");
        digitalWrite(ledPin, HIGH);
      }else{
        Serial.println("in off");
        digitalWrite(ledPin, LOW);
      }
    }
  }
}

void MQTT_connect() {
  int8_t ret;
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) {
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);
       retries--;
       if (retries == 0) {
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}
