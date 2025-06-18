#include <Arduino.h>
#include <MQUnifiedsensor.h>

#define placa "ESP32"
#define volt 3.3
#define pino 36
#define sensor "MQ-135"
float R0; // qualidade do ar
#define bitresolution 12
float tempo;
float tempopassado;

MQUnifiedsensor MQ135(placa,volt,bitresolution,pino,sensor);

void setup() 
{
  MQ135.setRegressionMethod(1);
  MQ135.setA(110.47);//valor do gas carbonico minimo           
  MQ135.setB(-2.862);//valor maximo do gas carbonico      
  Serial.begin(9600);
  MQ135.init();
  MQ135.update();//atualiza o valor do sensor
  R0 = MQ135.calibrate(3.6);//calibra o valor do ar
  MQ135.setR0(R0);//salva o valor calibrado
  Serial.print("o valor da qualidade do ar e: ");
  Serial.println(R0);
}

void loop() 
{
tempo = millis();
 if(tempo - tempopassado == 2000)
 {
  MQ135.update();
  float pmm = MQ135.readSensor();//le o valor atualizado pelo update
  Serial.print("seu sensor esta detectando por ppm: ");
  Serial.println(pmm);
  tempopassado = tempo; 
  }
}
