#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

// === CONFIGURAÇÕES ===
#define DHTPIN 4          // Pino conectado ao OUT do sensor
#define DHTTYPE DHT22     // Sensor DHT22
DHT dht(DHTPIN, DHTTYPE);

LiquidCrystal_I2C lcd(0x27, 20, 4); // Endereço I2C e dimensões do LCD

// === CARACTERES PERSONALIZADOS ===
uint8_t pequenaArvoreChar[] = {
  B00000,
  B10110,
  B01100,
  B00101,
  B00110,
  B01100,
  B00100,
  B00100
};

uint8_t ArvoreChar[] = {
 
  B01000,
  B00101,
  B00010,
  B10100,
  B01100,
  B00100,
  B00100,
  B00100
};

uint8_t topoParedeChar[] = {
  B11111,
  B11111, 
  B01111,
  B01111,
  B01111,
  B01011,
  B00011, 
  B00001
};

uint8_t baixoParedeChar[] = {
  B01001, B01011, B01011, B01111,
  B01111, B01111, B11111, B11111
};

// === VARIÁVEIS DE CONTROLE ===
bool paredeJaSumiu = false;

// === FUNÇÕES DE DESENHO ===
void desenharParedeGelo() {
  lcd.setCursor(15, 0);
  lcd.write(2); // topo
  lcd.setCursor(16,0);
  lcd.write((uint8_t)255);
  lcd.setCursor(16,1);
  lcd.write(2);
  lcd.setCursor(15, 3);
  lcd.write(3); // base
  lcd.setCursor(16, 3);
  lcd.write((uint8_t)255);
  lcd.setCursor(16,2);
  lcd.write(3);
}

void apagarParedeGelo() {
   lcd.setCursor(15, 0);
  lcd.print(" "); // topo
  lcd.setCursor(16,0);
  lcd.print(" "); 
  lcd.setCursor(16,1);
  lcd.print(" "); 
  lcd.setCursor(15, 3);
  lcd.print(" "); 
  lcd.setCursor(16, 3);
  lcd.print(" "); 
  lcd.setCursor(16,2);
  lcd.print(" "); 
  lcd.setCursor(13, 2);
  lcd.print("!");
}

void desenharArvore() {
  lcd.setCursor(6, 3);
  lcd.write(6);
  lcd.setCursor(10, 3);
  lcd.write(6);
}

void piscarParedeGelo(int vezes, int intervalo) {
  for (int i = 0; i < vezes; i++) {
    apagarParedeGelo();
    lcd.setCursor(13, 2);
    lcd.print("!");
    delay(intervalo);
    desenharParedeGelo();
    lcd.setCursor(13, 2);
    lcd.print("!");
    delay(intervalo);
  }
  apagarParedeGelo();
  paredeJaSumiu = true;
}

// === SETUP ===
void setup() {
  Serial.begin(9600);
  dht.begin();

  lcd.init();
  lcd.backlight();

  lcd.createChar(2, topoParedeChar);
  lcd.createChar(3, baixoParedeChar);
  lcd.createChar(6, ArvoreChar);

  lcd.setCursor(0, 0);
  lcd.clear();

  desenharParedeGelo();
  desenharArvore();
}

// === LOOP PRINCIPAL ===
void loop()
 {
  float temperatura = dht.readTemperature();
  float umidade = dht.readHumidity();

  if (isnan(temperatura) || isnan(umidade)) {
    Serial.println("Erro ao ler o sensor DHT!");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Erro na leitura!");
    delay(3000);
    return;
  }

  // === SERIAL ===
  Serial.print("Temp: ");
  Serial.print(temperatura);
  Serial.print(" °C, Umidade: ");
  Serial.print(umidade);
  Serial.println(" %");




  // === Lógica de desaparecimento da parede ===
  if (temperatura > 25 && !paredeJaSumiu) {
    piscarParedeGelo(2, 500); 
    lcd.setCursor(13,2);
    lcd.print(" ");
    lcd.setCursor(15,3);
    lcd.print("__"); //oque sobrou da parede de gelo
  }

  delay(1000); // Atualiza a cada 3 segundos
}
