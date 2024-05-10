const int pinoPIR = D7; //PINO DIGITAL UTILIZADO PELO SENSOR

void setup(){
  pinMode(LED_BUILTIN, OUTPUT); //DEFINE O PINO COMO SAÍDA
  pinMode(pinoPIR, INPUT); //DEFINE O PINO COMO ENTRADA
}
void loop(){
 if(digitalRead(pinoPIR) == HIGH){ //SE A LEITURA DO PINO FOR IGUAL A HIGH, FAZ
    digitalWrite(LED_BUILTIN, LOW); //ACENDE O LED
 }else{ //SENÃO, FAZ
  digitalWrite(LED_BUILTIN, HIGH); //APAGA O LED
 }
}
