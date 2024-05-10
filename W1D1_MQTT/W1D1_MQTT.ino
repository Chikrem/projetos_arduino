/************************************************************************ 
* Programa: Tutorial Comunicação MQTT 
* Autor: Curto Circuito 
* Descrição: Programa baseado no exemplo mqtt_esp8266 do autor Nick O'Leary 
**************************************************************************/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>



const char* ssid = "dlink-E970";                                
const char* password = "coigd97023";                                    
const char* mqtt_server = "broker.mqtt-dashboard.com";                  

WiFiClient espClient;                                                   
PubSubClient client(espClient);                                         
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void setup() {                                                          
  pinMode(BUILTIN_LED, OUTPUT);                                         
  Serial.begin(115200);                                                 
  setup_wifi();                                                         
  client.setServer(mqtt_server, 1883);                                  
  client.setCallback(callback);                                         
}

void setup_wifi() {                                                     

  delay(10);                                                            
  Serial.println();                                                     
  Serial.print("Conectando com ");                                       
  Serial.println(ssid);                                                 
             
  WiFi.begin(ssid, password);                                           
 
  while (WiFi.status() != WL_CONNECTED) {                               
    delay(500);                                                         
    Serial.print(".");                                                  
  }
  randomSeed(micros());
  Serial.println();                                                     
  Serial.println("WiFi conectado");                                     
  Serial.println("Endereço de IP: ");                                   
  Serial.println(WiFi.localIP());                                       
}

void callback(char* topic, byte* payload, unsigned int length) {        
  Serial.print("Mensagem recebida [");                                  
  Serial.print(topic);                                                  
  Serial.print("] ");                                                   
  for (int i = 0; i < length; i++) {                                    
    Serial.print((char)payload[i]);                                     
  }
  Serial.println();                                                     

  if ((char)payload[0] == '1') {                                        
    digitalWrite(BUILTIN_LED, LOW);                                     
  } else {                                                              
    digitalWrite(BUILTIN_LED, HIGH);                                    
  }

}

void reconnect() {                                                       
  while (!client.connected()) {                                          
    Serial.print("Aguardando conexão MQTT...");                          
    if (client.connect("ESP8266Client")) {                               
      Serial.println("conectado");                                       
      client.publish("publicacao_do_esp8266", "hello world");    
      client.subscribe("subscricao_do_esp8266");                         
    } else {                                                             
      Serial.print("falhou, rc=");                                       
      Serial.print(client.state());                                      
      Serial.println(" tente novamente em 5s");                          
      delay(5000);                                                       
    }
  }
}
void loop() {                                                            

  if (!client.connected()) {                                             
    reconnect();
  }
  client.loop();                                                         

  long now = millis();                                                   
  if (now - lastMsg > 5000) {                                            
    lastMsg = now;                                                       
    ++value;                                                             
    snprintf (msg, 75, "hello world #%ld", value);               
    Serial.print("Publica mensagem: ");                                  
    Serial.println(msg);                                                 
    client.publish("publicacao_do_esp8266", msg);                        
  }
}

/************************ FIM DO PROGRAMA***************************/
