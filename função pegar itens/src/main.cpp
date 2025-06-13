#include <Arduino.h>
#include "Itens.h"
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);

#define pinBotaoEsq 23
#define pinBotaoDir 15
#define pinBotaoConfirm 3

static int leituraInicialBtEsq = LOW;
static int leituraInicialBtDir = LOW;
static int leituraInicialBtConfirm = LOW;

static bool chaveMestra = false;
static int posicao = 0;
static int posicaoChave = 0;

void setup()
{
  Serial.begin(9600);

  pinMode(pinBotaoEsq, INPUT_PULLUP);
  pinMode(pinBotaoDir, INPUT_PULLUP);
  pinMode(pinBotaoConfirm, INPUT_PULLUP);

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 2);
  lcd.print("*");

  gerarChave();
}

void loop()
{
  int leitura1 = digitalRead(pinBotaoEsq);
  int leitura2 = digitalRead(pinBotaoDir);
  int leitura3 = digitalRead(pinBotaoConfirm);

  if (leituraInicialBtEsq != leitura1)
  {
    if (leitura1 == HIGH)
    {
      lcd.setCursor(posicao, 2);
      lcd.print(" ");

      posicao = (posicao == 0) ? 19 : posicao - 1;

      lcd.setCursor(posicao, 2);
      lcd.print("*");
      Serial.print("Andou para a coluna ");
      Serial.println(posicao);
    }
    leituraInicialBtEsq = leitura1;
  }

  if (leituraInicialBtDir != leitura2)
  {
    if (leitura2 == HIGH)
    {
      lcd.setCursor(posicao, 2);
      lcd.print(" ");

      posicao = (posicao == 19) ? 0 : posicao + 1;

      lcd.setCursor(posicao, 2);
      lcd.print("*");
      Serial.print("Andou para a coluna ");
      Serial.println(posicao);
    }
    leituraInicialBtDir = leitura2;
  }

  if (posicao == posicaoChave && (chaveMestra == false))
  {
    pegarItem();
  }
}
