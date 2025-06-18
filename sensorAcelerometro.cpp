#include <Arduino.h>
#include <Adafruit_MMA8451.h>

float tempopassado;

Adafruit_MMA8451 acelerometro = Adafruit_MMA8451();

void setup() 
{
  acelerometro.begin();
  Serial.begin(9600);
  acelerometro.setRange(MMA8451_RANGE_4_G);//sensibilidade do sensor
}

void loop() 
{
 float tempo = millis();
 if(tempo - tempopassado == 1000)
 {
  acelerometro.read();
  Serial.printf("/valor do eixo y: %f",acelerometro.y_g);//analisa a gravidade do eixo y
  Serial.println("");
  tempopassado = tempo;
 }
}
