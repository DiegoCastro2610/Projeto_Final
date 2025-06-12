#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

int numero[20];
int posicao = 0;

#define ldr 32

//Chave
byte chave[] = { B01010, B10101, B01010, B00100, B00100, B00110, B00110, B00100 };

void setup()
{
    pinMode(ldr, INPUT);
    Serial.begin(9600);
    lcd.init();
    lcd.backlight();
    lcd.createChar(0, chave);
    lcd.home();
    lcd.write(0);
    
}

void loop()
{
    int valorLDR = analogRead(ldr);
    Serial.println(valorLDR);

    if (valorLDR > 3000)
    {
        lcd.noBacklight();
        for (int linha = 0; linha < 20; linha++)
        {
            lcd.setCursor(linha, 3);
            lcd.print(" ");
            lcd.setCursor(linha, 0);
            lcd.print(" ");
        }
    }

    else
    {
        lcd.backlight();
        for (int linha = 0; linha < 20; linha++)
        {
            lcd.setCursor(linha, 3);
            lcd.write((uint8_t)255);
            lcd.setCursor(linha, 0);
            lcd.write((uint8_t)255);
        }
    }
}
