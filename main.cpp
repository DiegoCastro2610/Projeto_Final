#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

// === Instanciar objetos ===
LiquidCrystal_I2C lcd(0x27, 20, 4);

// === Definição dos pinos ===
#define pinBotaoEsq 25
#define pinBotaoDir 23
#define pinBotaoConfirm 3

#define botao1 4
#define botao2 5
#define botao3 18
#define botao4 19
// === Variáveis globais ===
int nivel = 0;

bool estadoBotao1 = HIGH;
bool estadoBotao2 = HIGH;
bool estadoBotao3 = HIGH;
bool estadoBotao4 = HIGH;

bool cenario1 = true;
bool cenario2 = false;
int posicao = 0;
bool puzzle = false;
int fase = 0;
// === Controle de botões ===
int leituraAnteriorBtEsq = HIGH;
int leituraAnteriorBtDir = HIGH;

// === Funções ===
void esquerda();
void direita();
void desenharCenario1();
void mudarParaCenario1();
void mudarParaCenario2();
void castelo();
void jogoDaMemoria(bool, bool, bool, bool);

void setup()
{
  Serial.begin(9600);

  pinMode(pinBotaoEsq, INPUT_PULLUP);
  pinMode(pinBotaoDir, INPUT_PULLUP);
  pinMode(pinBotaoConfirm, INPUT_PULLUP);

  pinMode(botao1, INPUT_PULLUP);
  pinMode(botao2, INPUT_PULLUP);
  pinMode(botao3, INPUT_PULLUP);
  pinMode(botao4, INPUT_PULLUP);

  lcd.init();
  lcd.backlight();

  desenharCenario1();
  posicao = 0;
  lcd.setCursor(posicao, 2);
  lcd.print("*");
}

void loop()
{
  // Leitura dos botões atualizada a cada loop
  bool leituraBt1 = digitalRead(botao1);
  bool leituraBt2 = digitalRead(botao2);
  bool leituraBt3 = digitalRead(botao3);
  bool leituraBt4 = digitalRead(botao4);

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

  // Se o puzzle estiver ativo, executa
  if (puzzle)
  {
    jogoDaMemoria(leituraBt1, leituraBt2, leituraBt3, leituraBt4);
  }
}

// ================== Funções de navegação ==================

void desenharCenario1()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" n  u u  v  e  n  ");
  lcd.setCursor(18, 0);
  lcd.write(255);
  lcd.setCursor(19, 0);
  lcd.write(255);

  lcd.setCursor(0, 1);
  lcd.print("==================");
  lcd.setCursor(18, 1);
  lcd.write(255);
  lcd.setCursor(19, 1);
  lcd.write(255);

  lcd.setCursor(0, 2);
  lcd.print("                  ||");
  lcd.setCursor(0, 3);
  lcd.print("                  ||");

  // TODO: Adicionar texto no LCD explicando o cenário 1 (ex.: "Voce esta na floresta")
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

      // TODO: Mostrar no LCD qual local está se movendo (Ex.: "Arvore", "Pedra", "Portao")
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

      // TODO: Mostrar no LCD qual local está se movendo (Ex.: "Portao do castelo")
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
  puzzle = false;
  nivel = 0;

  Serial.println("Saiu do castelo!");

  desenharCenario1();
  lcd.setCursor(posicao, 2);
  lcd.print("*");

  // TODO: Mostrar mensagem no LCD: "Saindo do castelo..."
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

  // TODO: Mostrar no LCD descrição do castelo (Ex.: "Voce entrou no castelo")
}

// ================== Funções do Castelo ==================

void castelo()
{
  if (posicao == 10 && !puzzle)
  {
    lcd.setCursor(1, 3);
    lcd.clear();
    lcd.print("Puzzle inicia!");

    // TODO: Mostrar no LCD um diálogo tipo "Uma porta trancada... Resolva o puzzle!"

    puzzle = true;
    nivel = 1;
    Serial.println("Puzzle iniciado!");
  }
}

// ================== Jogo da Memória ==================

void jogoDaMemoria(bool leituraBt1, bool leituraBt2, bool leituraBt3, bool leituraBt4)
{
  // --- Nivel 1 ---
  if (nivel == 1)
  {
    // TODO: No LCD -> "Nivel 1: Aperte o botao correto"
    if ((leituraBt1 == LOW && estadoBotao1 == HIGH) ||
        (leituraBt2 == LOW && estadoBotao2 == HIGH) ||
        (leituraBt4 == LOW && estadoBotao4 == HIGH))
    {
      Serial.println("Errou! Reiniciando puzzle...");
      lcd.clear();
      lcd.setCursor(2, 1);
      lcd.print("Errou! Reinicie!");

      delay(1000);
      nivel = 0;
      puzzle = false;
      return;
    }

    if (leituraBt3 == LOW && estadoBotao3 == HIGH)
    {
      nivel = 2;
      Serial.println("Nivel 2 iniciado (aperte Botao 2)");
      lcd.setCursor(0,0);
      lcd.print("Nivel 2 iniciado");
      // TODO: LCD -> "Nivel 2 desbloqueado!"
      delay(500);
    }
     estadoBotao1 = leituraBt1;
     estadoBotao2 = leituraBt2;
     estadoBotao3 = leituraBt3;
     estadoBotao4 = leituraBt4;
  }

  // --- Nivel 2 ---
  if (nivel == 2)
  {
    // TODO: No LCD -> "Nivel 2: Aperte o botao correto"
    if ((leituraBt1 == LOW && estadoBotao1 == HIGH) ||
        (leituraBt3 == LOW && estadoBotao3 == HIGH) ||
        (leituraBt4 == LOW && estadoBotao4 == HIGH))
    {
      Serial.println("Errou! Reiniciando puzzle...");
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("Errou! Reinicie!");

      // TODO: Mostrar no LCD -> "Errou! Tente novamente."

      delay(1000);
      nivel = 0;
      puzzle = false;
      return;
    }

    if (leituraBt2 == LOW && estadoBotao2 == HIGH)
    {
      nivel = 3;
      Serial.println("Nivel 3 iniciado (aperte Botao 4)");
      lcd.setCursor(0,0);
      lcd.print("Nivel 3 iniciado");

      // TODO: LCD -> "Nivel 3 desbloqueado!"
      delay(500);
    }
   estadoBotao1 = leituraBt1;
   estadoBotao2 = leituraBt2;
   estadoBotao3 = leituraBt3;
   estadoBotao4 = leituraBt4;
  }

  // --- Nivel 3 ---
  if (nivel == 3)
  {
    // TODO: No LCD -> "Nivel 3: Aperte o botao correto"
    if ((leituraBt1 == LOW && estadoBotao1 == HIGH) ||
        (leituraBt2 == LOW && estadoBotao2 == HIGH) ||
        (leituraBt3 == LOW && estadoBotao3 == HIGH))
    {
      Serial.println("Errou! Reiniciando puzzle...");
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("Errou! Reinicie!");

      // TODO: Mostrar no LCD -> "Errou! Tente novamente."

      delay(1000);
      nivel = 0;
      puzzle = false;
      return;
    }

    if (leituraBt4 == LOW && estadoBotao4 == HIGH)
    {
      Serial.println("Parabéns! Puzzle completo!");
      lcd.clear();
      lcd.setCursor(1, 1);
      lcd.print("PUZZLE COMPLETO!");

      // TODO: LCD -> "Parabéns! Porta abriu!"

      delay(1500);
      puzzle = false;
      fase = 2;
      posicao = 0;
    }
   estadoBotao1 = leituraBt1;
   estadoBotao2 = leituraBt2;
   estadoBotao3 = leituraBt3;
   estadoBotao4 = leituraBt4;
  }

  // Atualiza os estados anteriores dos botões
  estadoBotao1 = leituraBt1;
  estadoBotao2 = leituraBt2;
  estadoBotao3 = leituraBt3;
  estadoBotao4 = leituraBt4;
}
