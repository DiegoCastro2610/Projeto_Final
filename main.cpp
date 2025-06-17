#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

// === Instanciar LCD ===
LiquidCrystal_I2C lcd(0x27, 20, 4);

// === Definir pinos dos botões ===
#define pinBotaoEsq 15
#define pinBotaoDir 23
#define pinBotaoConfirm 3

#define pinBotao1 4
#define pinBotao2 5
#define pinBotao3 6
#define pinBotao4 7

// === Variáveis ===
bool cenario1 = true;
bool cenario2 = false;
bool puzzleResolvido = false;

// Estado dos botões
int leituraAnteriorBtEsq = HIGH;
int leituraAnteriorBtDir = HIGH;

// Controle do personagem
int posicao = 0;

// Controle do jogo da memória
int memoria[4] = {1, 2, 1, 2}; // 2 pares
bool revelado[4] = {false, false, false, false};
int primeiraEscolha = -1;
bool jogandoMemoria = false;

// === Prototipos ===
void esquerda();
void direita();
void desenharCenario1();
void mudarParaCenario1();
void mudarParaCenario2();
void castelo();
void jogoDaMemoria();
void loading();

void setup() {
  Serial.begin(9600);

  pinMode(pinBotaoEsq, INPUT_PULLUP);
  pinMode(pinBotaoDir, INPUT_PULLUP);
  pinMode(pinBotaoConfirm, INPUT_PULLUP);

  pinMode(pinBotao1, INPUT_PULLUP);
  pinMode(pinBotao2, INPUT_PULLUP);
  pinMode(pinBotao3, INPUT_PULLUP);
  pinMode(pinBotao4, INPUT_PULLUP);

  lcd.init();
  lcd.backlight();

  desenharCenario1();

  lcd.setCursor(posicao, 2);
  lcd.print("*");
}

void loop() {
  if (cenario1) {
    esquerda();
    direita();

    // Travar na porta se não completou o puzzle
    if (posicao == 19) {
      lcd.setCursor(0, 3);
      lcd.print("Resolva p/ entrar");

      if (!puzzleResolvido) {
        jogoDaMemoria();
      } else {
        int leituraConfirm = digitalRead(pinBotaoConfirm);
        if (leituraConfirm == LOW) {
          loading();
          mudarParaCenario2();
        }
      }
    }
  }

  if (cenario2) {
    esquerda();
    direita();
    castelo();

    // Voltar para o cenário 1
    int leituraEsq = digitalRead(pinBotaoEsq);
    if (posicao == 0 && leituraEsq == LOW) {
      loading();
      mudarParaCenario1();
    }
  }
}

// === Cenário 1 com porta 3D ===
void desenharCenario1() {
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print(" n  u u  v  e  n   ██"); // Topo da porta

  lcd.setCursor(0, 1);
  lcd.print("================== ██"); // Parede + porta

  lcd.setCursor(0, 2);
  lcd.print("                   ||"); // Porta com maçaneta

  lcd.setCursor(0, 3);
  lcd.print("                   ||"); // Porta até o chão
}

// === Movimento Esquerda ===
void esquerda() {
  int leitura = digitalRead(pinBotaoEsq);
  if (leituraAnteriorBtEsq != leitura) {
    if (leitura == LOW) {
      if (posicao > 0) {
        lcd.setCursor(posicao, 2);
        lcd.print(" ");
        posicao--;
        lcd.setCursor(posicao, 2);
        lcd.print("*");
        Serial.println(posicao);
      }
    }
    leituraAnteriorBtEsq = leitura;
  }
}

// === Movimento Direita ===
void direita() {
  int leitura = digitalRead(pinBotaoDir);
  if (leituraAnteriorBtDir != leitura) {
    if (leitura == LOW) {
      if (posicao < 19) {
        lcd.setCursor(posicao, 2);
        lcd.print(" ");
        posicao++;
        lcd.setCursor(posicao, 2);
        lcd.print("*");
        Serial.println(posicao);
      }
    }
    leituraAnteriorBtDir = leitura;
  }
}

// === Voltar para Cenário 1 ===
void mudarParaCenario1() {
  posicao = 19;
  cenario1 = true;
  cenario2 = false;
  desenharCenario1();
  lcd.setCursor(posicao, 2);
  lcd.print("*");
  puzzleResolvido = false;
  primeiraEscolha = -1;
  for (int i = 0; i < 4; i++) revelado[i] = false;
}

// === Entrar no castelo ===
void mudarParaCenario2() {
  lcd.clear();
  posicao = 0;
  cenario1 = false;
  cenario2 = true;

  lcd.setCursor(3, 0);
  lcd.print(">>> CASTELO <<<");

  lcd.setCursor(0, 1);
  lcd.print("====================");

  lcd.setCursor(0, 2);
  lcd.print("                    ");

  lcd.setCursor(posicao, 2);
  lcd.print("*");
}

// === Tela de Loading ===
void loading() {
  lcd.clear();
  lcd.setCursor(5, 1);
  lcd.print("Carregando...");
  delay(500);
  for (int i = 0; i < 16; i++) {
    lcd.setCursor(i, 2);
    lcd.print((char)255); // Bloco cheio
    delay(100);
  }
  delay(500);
}

// === Funcionalidade dentro do castelo ===
void castelo() {
  lcd.setCursor(3, 3);
  lcd.print("Bem vindo!");
}

// === Jogo da Memória ===
void jogoDaMemoria() {
  jogandoMemoria = true;
  lcd.setCursor(0, 0);
  lcd.print(" Jogo da Memoria ");

  // Mostrar estado dos pares
  lcd.setCursor(0, 1);
  for (int i = 0; i < 4; i++) {
    if (revelado[i]) {
      lcd.print(memoria[i]);
    } else {
      lcd.print("[ ]");
    }
    lcd.print(" ");
  }

  // Ler botões
  int botoes[4] = {
    digitalRead(pinBotao1),
    digitalRead(pinBotao2),
    digitalRead(pinBotao3),
    digitalRead(pinBotao4)
  };

  for (int i = 0; i < 4; i++) {
    if (botoes[i] == LOW) {
      if (primeiraEscolha == -1) {
        primeiraEscolha = i;
      } else if (i != primeiraEscolha) {
        lcd.setCursor(0, 2);
        lcd.print("Escolheu ");
        lcd.print(primeiraEscolha + 1);
        lcd.print(" e ");
        lcd.print(i + 1);
        delay(1000);

        if (memoria[primeiraEscolha] == memoria[i]) {
          revelado[primeiraEscolha] = true;
          revelado[i] = true;
          lcd.setCursor(0, 3);
          lcd.print("Acertou!        ");
        } else {
          lcd.setCursor(0, 3);
          lcd.print("Errou!          ");
        }

        delay(1000);
        primeiraEscolha = -1;
      }
    }
  }

  // Verificar se terminou
  bool completo = true;
  for (int i = 0; i < 4; i++) {
    if (!revelado[i]) completo = false;
  }

  if (completo) {
    lcd.clear();
    lcd.setCursor(3, 1);
    lcd.print("PUZZLE OK!");
    delay(1500);
    puzzleResolvido = true;
    jogandoMemoria = false;
    lcd.clear();
    desenharCenario1();
    lcd.setCursor(posicao, 2);
    lcd.print("*");
  }
}
