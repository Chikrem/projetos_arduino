/*
 Publishing in the callback

  - connects to an MQTT server
  - subscribes to the topic "inTopic"
  - when a message is received, republishes it to "outTopic"

  This example shows how to control pins using Ethernet shield via MQTT

*/

#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

//Informacoes de endereco IP, gateway, mascara de rede
byte mac[] = { 0xA4, 0x28, 0x72, 0xCA, 0x55, 0x2F };
byte ip[] = { 192, 168, 100, 120 };
byte gateway[] = { 192, 168, 100, 1 };
byte subnet[] = { 255, 255, 255, 0 };
 
const int lightPin = 13;

// Callback function header
void callback(char* topic, byte* payload, unsigned int length);

EthernetClient ethClient;
PubSubClient client("test.mosquitto.org", 1883, callback, ethClient);





// Callback function
void callback(char* topic, byte* payload, unsigned int length) {
    
  //turn the LED ON if the payload is '1' and publish to the MQTT server a confirmation message
  if(payload[0] == '1'){
    digitalWrite(lightPin, HIGH);
    client.publish("outTopicW1", "Light On"); }
    
  //turn the LED OFF if the payload is '0' and publish to the MQTT server a confirmation message
  if (payload[0] == '0'){
    digitalWrite(lightPin, LOW);
    client.publish("outTopicW1", "Light Off");
    }
} // void callback






void setup()
{
  pinMode(lightPin, OUTPUT);
  digitalWrite(lightPin, LOW);
  
  Ethernet.begin(mac, ip);
  if (client.connect("arduinoClient")) {
    client.publish("outTopicW1","hello world");
    client.subscribe("inTopicW1");
  }
}

void loop()
{
  client.loop();
}
