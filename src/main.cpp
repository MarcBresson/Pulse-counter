#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <HAMqtt.h>
#include "Credentials.h"
#include "pulseur.h"

#define MQTT_KEEPALIVE 60

EspMQTTClient client(
  WIFI_SSID,
  WIFI_PASSWORD,
  MQTT_IP,
  MQTT_USERNAME,
  MQTT_PASSWORD
);

// the device needs the client if the availability is handled device-wise. Instead of
// sending one message per entity for availability, you send one message for the device
// availability.
HAMqttDevice device("Compteur electrique", client);

// the entity works with the device to get identifiers, topics or the config payload.
HAMqttEntity compteur(device, "Puissance compteur", HAMqttEntity::SENSOR);

PulseListener pulseur(D1, 10000, 100);

void setup() {
  Serial.begin(9600);
  // adds device config variables.
  device.addConfig("sw_version", "2.0.1");
  device.addConfig("manufacturer", "Marckle");

  client.setKeepAlive(MQTT_KEEPALIVE);
  client.setMaxPacketSize(1024);
  // prints info on Serial everytime it tries to connect, send or receive a payload.
  client.enableDebuggingMessages();

  // enable the entity to report its state to home assistant.
  compteur.addStateTopic();
  // adds custom config variables for the entity
  compteur.addConfig("device_class", "power");
  compteur.addConfig("unit_of_measurement", "W");
}

void onConnectionEstablished(){
  // sends the configuration payload on the configuration topic.
  client.publish(compteur.getDiscoveryTopic(), compteur.getConfigPayload(), true);
}

void loop() {
    client.loop();

    device.manageAvailability(MQTT_KEEPALIVE);

    pulseur.update();

    if (pulseur.isReady()){
      uint16_t pc = pulseur.getPulseCount();
      float_t pi = pulseur.getIntervalValue() / (float_t) 3600000;
      float_t powerValue = pc / pi;

      client.publish(compteur.getStateTopic(), String(powerValue));

      pulseur.reset();
    }
}
