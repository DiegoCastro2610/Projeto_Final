#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

// === Definição dos pinos ===
#define pinBotaoEsq 15
#define pinBotaoDir 23
#define pinBotaoConfirm 3

// === Variáveis globais ===
bool cenario1 = true;
bool cenario2 = false;

int leituraAnteriorBtEsq = HIGH;
int leituraAnteriorBtDir = HIGH;

int posicao = 0;

// === Funções ===
void esquerda();
void direita();
void desenharCenario1();
void mudarParaCenario1();
void mudarParaCenario2();
void castelo();

void setup()
{
  Serial.begin(9600);

  pinMode(pinBotaoEsq, INPUT_PULLUP);
  pinMode(pinBotaoDir, INPUT_PULLUP);
  pinMode(pinBotaoConfirm, INPUT_PULLUP);

  lcd.init();
  lcd.backlight();

  // Desenha o cenário inicial
  desenharCenario1();

  lcd.setCursor(posicao, 2);
  lcd.print("*"); // Personagem
}

void loop()
{
  if (cenario1)
  {
    esquerda();
    direita();

    // Se o personagem estiver na porta do castelo (posição 19) e apertar direita
    int leituraDir = digitalRead(pinBotaoDir);
    if (posicao == 19 && leituraDir == LOW)
    {
      mudarParaCenario2();
    }
  }
  
  if (cenario2)
  {
    esquerda();
    direita();
    castelo();

    int leituraEsq = digitalRead(pinBotaoEsq);
    if (posicao == 0 && leituraEsq == LOW)
    {
      mudarParaCenario1();
    }
  }
}

// === Desenhar o cenário 1 (entrada do castelo) ===
void desenharCenario1()
{
  lcd.clear();

  // Linha superior com "nuvens" e céu
  lcd.setCursor(0, 0);
  lcd.print(" n  u u  v  e  n ");

  // Linha do castelo (muro e porta na posição 19)
  lcd.setCursor(0, 1);
  lcd.print("==================|"); // | é a porta

  // Linha onde o personagem anda (linha 2)
  lcd.setCursor(0, 2);
  lcd.print("                    "); // Linha vazia para andar
}

// === Função para mover para a esquerda ===
void esquerda()
{
  int leitura = digitalRead(pinBotaoEsq);
  if (leituraAnteriorBtEsq != leitura)
  {
    if (leitura == LOW) // Pressionado
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

// === Função para mover para a direita ===
void direita()
{
  int leitura = digitalRead(pinBotaoDir);
  if (leituraAnteriorBtDir != leitura)
  {
    if (leitura == LOW) // Pressionado
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
  Serial.println("saiu no castelo!");

  // Linha superior com "nuvens" e céu
  lcd.setCursor(0, 0);
  lcd.print(" n  u u  v  e  n ");

  // Linha do castelo (muro e porta na posição 19)
  lcd.setCursor(0, 1);
  lcd.print("==================|"); // | é a porta

  // Linha onde o personagem anda (linha 2)
  lcd.setCursor(0, 2);
  lcd.print("                    "); // Linha vazia para andar
}

// === Troca para o cenário do castelo ===
void mudarParaCenario2()
{
  lcd.clear();
  posicao = 0;
  cenario1 = false;
  cenario2 = true;
  Serial.println("Entrou no castelo!");

  // Desenha o castelo
  lcd.setCursor(3, 0);
  lcd.print(">>> CASTELO <<<");

  lcd.setCursor(0, 1);
  lcd.print("====================");

  lcd.setCursor(0, 2);
  lcd.print("                    ");

  lcd.setCursor(posicao, 2);
  lcd.print("*");
}

// === Função do castelo ===
void castelo()
{
  // Aqui podemos fazer a lógica de movimentação interna no castelo
  // e depois chamar um puzzle, como jogo da memória

  // Exemplo de detecção de porta interna para começar puzzle
  if (posicao == 10) // Suponha que porta está na coluna 10
  {
    lcd.setCursor(3, 3);
    lcd.print("Puzzle inicia!");
    // Aqui você pode colocar sua lógica para iniciar o jogo da memória
  }
}
