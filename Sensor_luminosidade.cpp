#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

#define ldr 26

void setup()
{
  pinMode(ldr, INPUT);
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
}

void loop()
{
  int valorLDR = analogRead(ldr);
  Serial.println(valorLDR);
  delay(1000);

  if(valorLDR > 3000)
  {
    lcd.noBacklight();
  }
  else
  {
    lcd.backlight();    
  }
}

