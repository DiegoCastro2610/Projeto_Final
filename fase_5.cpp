#include <Arduino.h>
#include "LiquidCrystal_I2C.h"
#include <Adafruit_MMA8451.h>
#include <MQUnifiedsensor.h>

#define placa "ESP32"
#define volt 3.3
#define pino 36
#define sensor "MQ-135"
#define bitresolution 12

LiquidCrystal_I2C lcd(0x27, 20, 4);
Adafruit_MMA8451 acelerometro = Adafruit_MMA8451();
MQUnifiedsensor MQ135(placa, volt, bitresolution, pino, sensor);

int vidaInimiga = 3;
int localPersonagen = 0;
int movimentoAtaque = 14;
float tempoAtaque = 3000;
float tempoPassado;
int i;
int AD;
int stamina = 2;
float eixoY;
float R0; // qualidade do ar
float bafo;
int lcdST = 0;

void art();
void ataqueInimigo();
void ADPersonagem();
void Vitoria();

void setup()
{
    lcd.init();
    lcd.backlight();

    acelerometro.begin();
    acelerometro.setRange(MMA8451_RANGE_4_G); // sensibilidade do sensor

    Serial.begin(9600);

    MQ135.setRegressionMethod(1);
    MQ135.setA(110.47); // valor do gas carbonico minimo
    MQ135.setB(-2.862); // valor maximo do gas carbonico
    MQ135.init();
    MQ135.update();            // atualiza o valor do sensor
    R0 = MQ135.calibrate(3.6); // calibra o valor do ar
    MQ135.setR0(R0);           // salva o valor calibrado

    art();
}

void loop()
{
    if (vidaInimiga == 0)
    {
        Vitoria();
    }
    delay(100);
    ADPersonagem();
    ataqueInimigo();
}

void art()
{
    byte personagem[] = {
        B00100,
        B01110,
        B00100,
        B01110,
        B10101,
        B00100,
        B01010,
        B01010};

    byte thiago[] = {
        B01110,
        B10001,
        B11011,
        B11111,
        B01110,
        B10101,
        B00100,
        B01010};

    byte sholdbi[] = {
        B01110,
        B10001,
        B11011,
        B10001,
        B01110,
        B10101,
        B00100,
        B01010};

    byte candelabro[] = {
        B00100,
        B00100,
        B10101,
        B10101,
        B01010,
        B00000,
        B00000,
        B00000};

    byte janela[] = {
        B00000,
        B11111,
        B10101,
        B10101,
        B11111,
        B10101,
        B10101,
        B11111};

    lcd.createChar(0, personagem);
    lcd.createChar(1, thiago);
    lcd.createChar(2, sholdbi);
    lcd.createChar(3, janela);
    lcd.setCursor(0, 2);
    lcd.write(0); // personagem principal

    lcd.setCursor(17, 2);
    lcd.write(2); // sholdibi

    lcd.setCursor(4, 1);
    lcd.write(3); // janela

    lcd.setCursor(14, 1);
    lcd.write(3); // janela

    lcd.setCursor(9, 0);
    lcd.write(1); // thiago

    lcd.setCursor(8, 0);
    lcd.print("|");

    lcd.setCursor(10, 0);
    lcd.print("|");

    lcd.setCursor(8, 1);
    lcd.print("---");

    lcd.setCursor(16, 0);
    lcd.print("ST0/2");
}

void ataqueInimigo()
{
    float tempo = millis();
    if (tempo - tempoPassado >= tempoAtaque)
    {
        for (i = 0; i <= 13; i++)
        {
            lcd.setCursor(movimentoAtaque, 2);
            lcd.print("<- ");
            movimentoAtaque -= 1;
            delay(500);
        }

        if (AD == 0)
        {
            lcd.clear();
            lcd.setCursor(6, 1);
            lcd.print("GAME OVER");
            lcd.setCursor(4, 2);
            lcd.print("Sholdibi Wins");
            while (1)
                ;
        }
        if (AD == 1)
        {
            movimentoAtaque = 14;
            lcd.setCursor(1, 2);
            lcd.print("|");
            delay(200);
            lcd.setCursor(1, 2);
            lcd.print("   ");
            stamina -= 1;
            lcdST += 1;
            lcd.setCursor(18, 0);
            lcd.print(lcdST);
            

            tempoPassado = tempo;
            tempoPassado += 7000;
            tempoAtaque -= 200;
        }
        if (AD == 2)
        {
            movimentoAtaque = 14;
            lcd.setCursor(0, 2);
            lcd.print("   ");
            lcd.setCursor(16, 2);
            lcd.write(0);
            delay(500);
            lcd.setCursor(16, 2);
            lcd.print(" ");
            lcd.setCursor(0, 2);
            lcd.write(0);
            stamina = 2;
            vidaInimiga -= 1;
            lcdST = 0;
            lcd.setCursor(18, 0);
            lcd.print(lcdST);

            tempoPassado = tempo;
            tempoPassado += 7000;
            tempoAtaque -= 200;
        }
    }
}

void ADPersonagem()
{
    acelerometro.read();
    eixoY = acelerometro.y_g;
    MQ135.update();
    bafo = MQ135.readSensor(); // le o valor atualizado pelo update

    if (eixoY > 0.4 && stamina >= 0) // defende
    {
        AD = 1;
    }
    else if (eixoY < -0.4 && stamina == 0) // ataca
    {
        AD = 2;
    }
    else
    {
        AD = 0;
    }
}

void Vitoria()
{
    lcd.clear();
    lcd.setCursor(2, 1);
    lcd.print("matou o sholdibi");
    delay(1000);
    lcd.clear();
    lcd.setCursor(9, 0);
    lcd.write(1);
    delay(500);
    lcd.clear();
    lcd.setCursor(9, 1);
    lcd.write(1);
    delay(500);
    lcd.clear();
    lcd.setCursor(5,0);
    lcd.print("Thiago:");
    lcd.setCursor(3,1);
    lcd.print("muito obrigado");
    lcd.setCursor(9, 2);
    lcd.write(1);
    while(1);
}