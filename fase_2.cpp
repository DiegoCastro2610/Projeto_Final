#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

// === CONFIGURAÇÕES ===
#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

LiquidCrystal_I2C lcd(0x27, 20, 4);

// === PINOS ===
#define JOY_X_PIN 35
#define JOY_Y_PIN 34
#define JOY_SW_PIN 27

#define BUTTON_A_PIN 14
#define BUTTON_B_PIN 12
#define BUTTON_C_PIN 13
#define BUTTON_D_PIN 33
#define BUTTON_E_PIN 25
#define BUTTON_F_PIN 26

// === VARIÁVEIS GERAIS ===
int cenario = 1;
int counter = 10;
bool parede = true;
bool paredeJaSumiu = false;
bool jaFoiPressionado = false;
bool possivelAndar = true;

int movimentacaox = 0;

// === CARACTERES PERSONALIZADOS (parede gelo) ===
uint8_t topoParedeChar[] = {
  B11111, B11111, B01111, B01111,
  B01111, B01011, B00011, B00001
};

uint8_t baixoParedeChar[] = {
  B01001, B01011, B01011, B01111,
  B01111, B01111, B11111, B11111
};

void desenharParedeGelo() {
  lcd.setCursor(15, 0); lcd.write(2);
  lcd.setCursor(16, 0); lcd.write((uint8_t)255);
  lcd.setCursor(16, 1); lcd.write(2);
  lcd.setCursor(15, 3); lcd.write(3);
  lcd.setCursor(16, 3); lcd.write((uint8_t)255);
  lcd.setCursor(16, 2); lcd.write(3);
}

void apagarParedeGelo() {
  for (int y = 0; y < 4; y++) {
    lcd.setCursor(15, y); lcd.print(" ");
    lcd.setCursor(16, y); lcd.print(" ");
  }
}

void setup() {
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);
  dht.begin();

  pinMode(BUTTON_B_PIN, INPUT_PULLUP);

  lcd.createChar(2, topoParedeChar);
  lcd.createChar(3, baixoParedeChar);

  lcd.clear();
}

void loop() {
  movimentacaox = analogRead(JOY_X_PIN);
  bool botaoPressionado = digitalRead(BUTTON_B_PIN) == LOW;

  float temperatura = dht.readTemperature();
  if (!isnan(temperatura) && temperatura > 25 && !paredeJaSumiu) {
    apagarParedeGelo();
    parede = false;
    paredeJaSumiu = true;
  }

  if (movimentacaox > 1000 && movimentacaox < 3000)
    possivelAndar = true;

  if (possivelAndar) {
    if (movimentacaox >= 4090 && counter >= 19) {
      lcd.setCursor(counter, 2); lcd.print(" ");
      if (cenario < 4) {
        cenario++; counter = 0;
      } else {
        counter = 19;
      }
    }
    else if (movimentacaox <= 10 && counter == 0 && cenario != 1) {
      lcd.setCursor(counter, 2); lcd.print(" ");
      cenario--; counter = 19;
      possivelAndar = false;
    }
    else if (movimentacaox >= 4090 && counter < 19) {
      if (!(cenario == 2 && counter + 1 >= 13 && counter + 1 <= 16 && parede)) {
        lcd.setCursor(counter, 2); lcd.print(" ");
        counter++; possivelAndar = false;
      }
    }
    else if (movimentacaox <= 10 && counter > 0) {
      lcd.setCursor(counter, 2); lcd.print(" ");
      counter--; possivelAndar = false;
    }
  }

  lcd.setCursor(counter, 2);
  lcd.print("X");

  lcd.setCursor(0, 1);
  lcd.print("Cenario ");
  lcd.print(cenario);
  lcd.print("           ");

  if (cenario == 2 && parede) {
    desenharParedeGelo();
    if (counter >= 13 && counter <= 16) {
      counter = 12;
    }
  }

  if (botaoPressionado && !jaFoiPressionado) {
    parede = false;
    jaFoiPressionado = true;
    apagarParedeGelo();
  }

}