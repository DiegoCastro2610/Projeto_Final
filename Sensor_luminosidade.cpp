#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

int numero[20];

#define ldr 32

// controle joystick
#define JOY_X_PIN 35
#define JOY_Y_PIN 34

// Controle botoes
#define botaoA 14
#define botaoB 12
#define botaoC 13
#define botaoD 33
#define botaoE 25
#define botaoF 26

// Chave
byte chave[] = {B01010, B10101, B01010, B00100, B00100, B00110, B00110, B00100};

void setup()
{
    pinMode(ldr, INPUT);
    pinMode(botaoA, INPUT_PULLUP);
    pinMode(botaoB, INPUT_PULLUP);
    pinMode(botaoC, INPUT_PULLUP);
    pinMode(botaoD, INPUT_PULLUP);
    pinMode(botaoE, INPUT_PULLUP);
    pinMode(botaoF, INPUT_PULLUP);

    Serial.begin(9600);
    lcd.init();
    lcd.backlight();
    lcd.createChar(0, chave);
    lcd.home();
    lcd.write(0);

    for (int linha = 0; linha < 20; linha++)
    {
        lcd.setCursor(linha, 3);
        lcd.write((uint8_t)255);
        lcd.setCursor(linha, 0);
        lcd.write((uint8_t)255);
    }
}

void loop()
{

    int valorLDR = analogRead(ldr);

    int movimentacaox = analogRead(JOY_X_PIN);
    int movimentacaoy = analogRead(JOY_Y_PIN);

    static bool estadoBotaoA = digitalRead(botaoA);
    static bool estadoBotaoB = digitalRead(botaoB);
    static bool estadoBotaoC = digitalRead(botaoC);
    static bool estadoBotaoD = digitalRead(botaoD);
    static bool estadoBotaoE = digitalRead(botaoE);
    static bool estadoBotaoF = digitalRead(botaoF);

    static int counter = 10;
    static bool possivelAndar = true;
    static int valorExagerado = -9999;

    // zona morta joystick
    if (movimentacaox > 1000 && movimentacaox < 3000)
    {
        possivelAndar = true;
    }

    // movimentação do contador com joystick
    if (possivelAndar)
    {
        if (movimentacaox >= 4090 && counter >= 19)
        {
            lcd.setCursor(counter, 2);
            lcd.print(" ");
            counter = 0;
            possivelAndar = false;
        }

        else if (movimentacaox <= 10 && counter == 0)
        {
            lcd.setCursor(counter, 2);
            lcd.print(" ");
            counter = 19;
            possivelAndar = false;
        }

        else if ((movimentacaox >= 4090) && counter < 19)
        {
            lcd.setCursor(counter, 2);
            lcd.print(" ");
            counter++;
            possivelAndar = false;
            lcd.setCursor(counter, 2);
            lcd.print("X");
        }

        else if ((movimentacaox <= 10) && counter > 0)
        {
            lcd.setCursor(counter, 2);
            lcd.print(" ");
            counter--;
            possivelAndar = false;
            lcd.setCursor(counter, 2);
            lcd.print("X");
        }
    
        else if (counter == 14 && estadoBotaoB == false)
        {
            lcd.setCursor(3,2);
            lcd.write((uint8_t)255);
        }
    }

    delay(50); // Delay pequeno para evitar repetições rápidas


    // if (valorLDR > 3000)
    // {
    //     lcd.noBacklight();
    //     for (int linha = 0; linha < 20; linha++)
    //     {
    //         lcd.setCursor(linha, 3);
    //         lcd.print(" ");
    //         lcd.setCursor(linha, 0);
    //         lcd.print(" ");
    //     }
    // }

    // else
    // {
    //     lcd.backlight();
    //     for (int linha = 0; linha < 20; linha++)
    //     {
    //         lcd.setCursor(linha, 3);
    //         lcd.write((uint8_t)255);
    //         lcd.setCursor(linha, 0);
    //         lcd.write((uint8_t)255);
    //     }
    // }
}
