#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
extern LiquidCrystal_I2C lcd;
#include "Itens.h"

#define pinBotaoEsq 23
#define pinBotaoDir 15
#define pinBotaoConfirm 3

// static bool chaveMestra = false;
static int posicaoChave;

static int leituraInicialBtEsq = LOW;
static int leituraInialBtDir = LOW;
static int leituraInicialBtConfirm = LOW;

int leitura1 = digitalRead(pinBotaoEsq);
int leitura2 = digitalRead(pinBotaoDir);
int leitura3 = digitalRead(pinBotaoConfirm);

byte chaveChar[8] = {
    B01010, B10101,
    B01010, B00100,
    B00100, B00110,
    B00110, B00100};

int gerarChave()
{
  randomSeed(analogRead(A0));
  posicaoChave = random(0, 19);

  Serial.println(posicaoChave);

  lcd.createChar(0, chaveChar);

  lcd.setCursor(posicaoChave, 2);
  lcd.print("!");
  return posicaoChave;
}

bool pegarItem(bool chaveMestra)
{
  Serial.print("voce pegou a chave seu malandrinhoo... ");
  lcd.setCursor(posicaoChave, 2);
  lcd.print(" ");

  return true;
}
