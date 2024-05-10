
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "dlink-95D9";    //NOME DA REDE
const char* password = "yrilo63014";    //SENHA DA REDE
const char* mqtt_server = "broker.mqtt-dashboard.com";    //ENDEREÇO DO BROKER MQTT

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastMsg = 0;
const long interval = 10000; 

#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

char Sala;                                                                                        
char Quarto;  

const int pinoPIR = D7;

void setup_wifi() {

  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  
  //Serial.println();
  Serial.println("");                                                                             
  if ((char)payload[0] == 'S') {                                                                  
    digitalWrite(Sala, HIGH);                                                                     
    snprintf (msg, MSG_BUFFER_SIZE, "A luz da sala está ligada");                                 
    Serial.print("Publica mensagem: ");                                                           
    Serial.println(msg);                                                                          
    client.publish("casa/sala", msg);                                                             
  }
  Serial.println("");                                                                             
  if ((char)payload[0] == 's') {                                                                  
    digitalWrite(Sala, LOW);                                                                      
    snprintf (msg, MSG_BUFFER_SIZE, "A luz da sala está desligada");                              
    Serial.print("Publica mensagem: ");                                                           
    Serial.println(msg);                                                                          
    client.publish("casa/sala", msg);                                                             
  }
  Serial.println("");                                                                             
  if ((char)payload[0] == 'Q') {                                                                  
    digitalWrite(Quarto, HIGH);                                                                   
    snprintf (msg, MSG_BUFFER_SIZE, "A luz do quarto está ligada");                               
    Serial.print("Publica mensagem: ");                                                           
    Serial.println(msg);                                                                          
    client.publish("casa/quarto", msg);                                                           
  }
   Serial.println("");                                                                            
   if ((char)payload[0] == 'q') {                                                                 
    digitalWrite(Quarto, LOW);                                                                    
    snprintf (msg, MSG_BUFFER_SIZE, "A luz do quarto está desligada");                            
    Serial.print("Publica mensagem: ");                                                           
    Serial.println(msg);                                                                          
    client.publish("casa/quarto", msg); 
  }
    Serial.println("");                                                                             
  if ((char)payload[0] == 'T') {                                                                  
    digitalWrite(LED_BUILTIN, HIGH);                                                                   
    snprintf (msg, MSG_BUFFER_SIZE, "O LED de teste está ligado");                               
    Serial.print("Publica mensagem: ");                                                           
    Serial.println(msg);                                                                          
    client.publish("casa/teste", msg);                                                           
  }
   Serial.println("");                                                                            
   if ((char)payload[0] == 't') {                                                                 
    digitalWrite(LED_BUILTIN, LOW);                                                                    
    snprintf (msg, MSG_BUFFER_SIZE, "O LED de teste está desligado");                            
    Serial.print("Publica mensagem: ");                                                           
    Serial.println(msg);                                                                          
    client.publish("casa/teste", msg);  
}
}
///////////////////////////////////////////////////////////////////////////////////////////

void reconnect() {
  
  while (!client.connected()) {       // LOOP PARA RECONECTAR
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP8266Client-";   //CRIA UM CLIENT ID ALEATÓRIO
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {   //TENTATIVA DE CONEXÃO
      Serial.println("connected");
      client.subscribe("inTopicW1");    //SUBSCRIBER MQTT
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);    //AGUARDA 5S PARA TENTAR NOVAMENTE
    }
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup() {

  Sala = 2;  //PINAGEM                                                                                      
  Quarto = 13;                                                                                     
                                                                                
  pinMode(Sala, OUTPUT);  //MODO DE PINAGEM                                                                        
  pinMode(Quarto, OUTPUT);  
  pinMode(pinoPIR, INPUT);
  
  Serial.begin(115200); //PORTA SERIAL WEMOS D1
  setup_wifi();
  client.setServer(mqtt_server, 1883); //BROKER E PORTA MQTT 
  client.setCallback(callback); //FUNÇÃO DE CALLBACK
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void loop() {

  if (!client.connected()) { //RECONEXÃO
    reconnect();
  }
  client.loop();  
  
  unsigned long now = millis(); 
  
if (now - lastMsg >= interval) { //INTERVALO ENTRE AS LEITURAS DO SENSOR DE PRESENÇA
lastMsg = now;
    
 if(digitalRead(pinoPIR) == HIGH){ //SE A LEITURA DO PINO FOR IGUAL A HIGH, FAZ
    //digitalWrite(LED_BUILTIN, LOW); //ACENDE O LED
    snprintf (msg, MSG_BUFFER_SIZE, "Sensor de presença ativado", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("casa/sensor", msg); //PUBLICA A MENSAGEM PARA ACENDER A LAMPADA
 }
 else{ //SENÃO, FAZ
  //digitalWrite(LED_BUILTIN, HIGH); //APAGA O LED
  snprintf (msg, MSG_BUFFER_SIZE, "Sensor de presença desativado", value);
  Serial.print("Publish message: ");
  Serial.println(msg);
  client.publish("casa/sensor", msg); //PUBLICA A MENSAGEM PARA APAGAR A LAMPADA
 }

}
}


/*

const int pinoPIR = 8; //PINO DIGITAL UTILIZADO PELO SENSOR
const int pinoLED = 12; //PINO DIGITAL UTILIZADO PELO LED

void setup(){
  pinMode(LED_BUILTIN, OUTPUT); //DEFINE O PINO COMO SAÍDA
  pinMode(pinoPIR, INPUT); //DEFINE O PINO COMO ENTRADA
}
void loop(){
 if(digitalRead(pinoPIR) == HIGH){ //SE A LEITURA DO PINO FOR IGUAL A HIGH, FAZ
    digitalWrite(LED_BUILTIN, HIGH); //ACENDE O LED
 }else{ //SENÃO, FAZ
  digitalWrite(LED_BUILTIN, LOW); //APAGA O LED
 }
}

*/

/*
 * 
const int trigPin = 12;
const int echoPin = 14;

//define sound velocity in cm/uS
#define SOUND_VELOCITY 0.034
#define CM_TO_INCH 0.393701

long duration;
float distanceCm;
float distanceInch;

void setup() {
  Serial.begin(115200); // Starts the serial communication
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
}

void loop() {
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance
  distanceCm = duration * SOUND_VELOCITY/2;
  
  // Convert to inches
  distanceInch = distanceCm * CM_TO_INCH;
  
  // Prints the distance on the Serial Monitor
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);
  Serial.print("Distance (inch): ");
  Serial.println(distanceInch);
  
  delay(1000);
}

*/
