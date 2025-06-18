#include <Arduino.h>

#include "LiquidCrystal_I2C.h"

LiquidCrystal_I2C lcd (0x27, 20, 4);

void art();


void setup() 
{
 lcd.init();
 Serial.begin(9600);
 lcd.backlight();

 art();
}

void loop() 
{
 
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
  B01010
};

byte thiago[] = {
  B01110,
  B10001,
  B11011,
  B11111,
  B01110,
  B10101,
  B00100,
  B01010
};

byte sholdbi[] = {
  B01110,
  B10001,
  B11011,
  B10001,
  B01110,
  B10101,
  B00100,
  B01010
};

byte candelabro[] = {
  B00100,
  B00100,
  B10101,
  B10101,
  B01010,
  B00000,
  B00000,
  B00000
};

byte janela[] = {
  B00000,
  B11111,
  B10101,
  B10101,
  B11111,
  B10101,
  B10101,
  B11111
};

byte cofre[] = {
  B00000,
  B11111,
  B10001,
  B10101,
  B11111,
  B10101,
  B10001,
  B11111
};


 lcd.createChar(0, personagem);
 lcd.createChar(1, thiago);
 lcd.createChar(2, sholdbi);
 lcd.createChar(3, janela);
 lcd.createChar(4, cofre);

  lcd.setCursor(0,2);
  lcd.write(0);//personagem principal

  lcd.setCursor(17,2);
  lcd.write(2);//sholdibi

  lcd.setCursor(4,1);
  lcd.write(3);//janela

  lcd.setCursor(14,1);
  lcd.write(3);//janela

  lcd.setCursor(19,2);
  lcd.write(4);//cofre

  lcd.setCursor(9,0);
  lcd.write(1);//thiago

  lcd.setCursor(8,0);
  lcd.print("|");

  lcd.setCursor(10,0);
  lcd.print("|");

  lcd.setCursor(8,1);
  lcd.print("---");

}