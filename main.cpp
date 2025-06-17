#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

// === Instanciar objetos ===
LiquidCrystal_I2C lcd(0x27, 20, 4);

// === Definição dos pinos ===
#define pinBotaoEsq 15
#define pinBotaoDir 23
#define botao1 5
#define botao2 18
#define botao3 15
#define botao4 23
#define pinBotaoConfirm 3

int nivel;
// === LED RGB ===
bool estadoBotao1;
bool estadoBotao2;
bool estadoBotao3;
bool estadoBotao4;

// === Variáveis globais ===
bool cenario1 = true;
bool cenario2 = false;
int posicao = 0;
bool puzzle = false;

// === Controle de botões ===
int leituraAnteriorBtEsq = HIGH;
int leituraAnteriorBtDir = HIGH;

// === Controle de cores ===
float COR1[4];
float COR2[4];
float INC[4];

// === Funções dos botões ===
void esquerda();
void direita();

// === Funções dos cenários ===
void desenharCenario1();
void mudarParaCenario1();
void mudarParaCenario2();
void castelo();
void jogoDaMemoria();

void setup()
{
  Serial.begin(9600);

  pinMode(pinBotaoEsq, INPUT_PULLUP);
  pinMode(pinBotaoDir, INPUT_PULLUP);
  pinMode(pinBotaoConfirm, INPUT_PULLUP);
  pinMode(botao1, INPUT_PULLUP);
  pinMode(botao2, INPUT);
  pinMode(botao3, INPUT);
  pinMode(botao4, INPUT);

 

  lcd.init();
  lcd.backlight();

  desenharCenario1();

  posicao = 0;
  lcd.setCursor(posicao, 2);
  lcd.print("*");
}

void loop()
{
  estadoBotao1 = digitalRead(botao1);
  estadoBotao2 = digitalRead(botao2);
  estadoBotao3 = digitalRead(botao3);
  estadoBotao4 = digitalRead(botao4);

  Serial.println(estadoBotao1);

  if (cenario1)
  {
    esquerda();
    direita();

    if (posicao == 19 && digitalRead(pinBotaoDir) == LOW)
    {
      mudarParaCenario2();
    }
  }

  if (cenario2)
  {
    esquerda();
    direita();
    castelo();

    if (posicao == 0 && digitalRead(pinBotaoEsq) == LOW)
    {
      mudarParaCenario1();
    }
  }
}

void desenharCenario1()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" n  u u  v  e  n  ");
  lcd.setCursor(18, 0);
  lcd.write((uint8_t)255);
  lcd.setCursor(19, 0);
  lcd.write((uint8_t)255);

  lcd.setCursor(0, 1);
  lcd.print("==================");
  lcd.setCursor(18, 1);
  lcd.write((uint8_t)255);
  lcd.setCursor(19, 1);
  lcd.write((uint8_t)255);

  lcd.setCursor(0, 2);
  lcd.print("                  ||");
  lcd.setCursor(0, 3);
  lcd.print("                  ||");
}

void esquerda()
{
  int leitura = digitalRead(pinBotaoEsq);
  if (leituraAnteriorBtEsq != leitura)
  {
    if (leitura == LOW)
    {
      lcd.setCursor(posicao, 2);
      lcd.print(" ");
      posicao = (posicao == 0) ? 19 : posicao - 1;
      lcd.setCursor(posicao, 2);
      lcd.print("*");
      Serial.println(posicao);
    }
    leituraAnteriorBtEsq = leitura;
  }
}

void direita()
{
  int leitura = digitalRead(pinBotaoDir);
  if (leituraAnteriorBtDir != leitura)
  {
    if (leitura == LOW)
    {
      lcd.setCursor(posicao, 2);
      lcd.print(" ");
      posicao = (posicao == 19) ? 0 : posicao + 1;
      lcd.setCursor(posicao, 2);
      lcd.print("*");
      Serial.println(posicao);
    }
    leituraAnteriorBtDir = leitura;
  }
}

void mudarParaCenario1()
{
  lcd.clear();
  posicao = 19;
  cenario1 = true;
  cenario2 = false;
  Serial.println("Saiu do castelo!");

  desenharCenario1();
  lcd.setCursor(posicao, 2);
  lcd.print("*");
}

void mudarParaCenario2()
{
  lcd.clear();
  posicao = 0;
  cenario1 = false;
  cenario2 = true;
  Serial.println("Entrou no castelo!");

  lcd.setCursor(3, 0);
  lcd.print(">>> CASTELO <<<");
  lcd.setCursor(0, 1);
  lcd.print("====================");
  lcd.setCursor(0, 2);
  lcd.print("                    ");
  lcd.setCursor(posicao, 2);
  lcd.print("*");
}

void castelo()
{
  if (posicao == 10 && !puzzle)
  {
    lcd.setCursor(3, 3);
    lcd.print("Puzzle inicia!");
    puzzle = true;
    jogoDaMemoria();
  }
}

void jogoDaMemoria()
{
  if( estadoBotao1 == 0 && estadoBotao2 == 1 && estadoBotao3 == 1 && estadoBotao4 == 1)
  {
    nivel = 1;
  }
  
}
