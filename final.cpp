#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <MQUnifiedsensor.h>
#include <Adafruit_MMA8451.h>

int numero[20];
static int fase = 2;
int contagem = 0;

static bool estadoParede = true;
static bool porta = false;

// fase 2
#define DHTPIN 4      // Pino conectado ao OUT do sensor
#define DHTTYPE DHT22 // Sensor DHT22

// joystick
#define JOY_X_PIN 35
#define JOY_Y_PIN 34

// fase 3
#define ldr 32
#define potenciometro 2

// fase 4
#define placa "ESP"
#define volt 3.3
#define pino 36
#define sensor "MQ-135"
#define bitresolution 12

int vidaInimiga = 3;
int localPersonagen = 0;
int movimentoAtaque = 14;
float tempoAtaque = 3000;
float tempoPassado;
int i;
int AD;
int stamina = 2;
float eixoY;
float R0; // qualidade do ar
float bafo;
int lcdST = 0;
bool arte = 0;

// Controle botoes
#define botaoA 14
#define botaoB 12
#define botaoC 13
#define botaoD 33
#define botaoE 25
#define botaoF 26

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 20, 4); // Endereço I2C e dimensões do LCD
Adafruit_MMA8451 acelerometro = Adafruit_MMA8451();
MQUnifiedsensor MQ135(placa, volt, bitresolution, pino, sensor);

// === CARACTERES PERSONALIZADOS ===
uint8_t ArvoreChar[] = {

    B01000,
    B00101,
    B00010,
    B10100,
    B01100,
    B00100,
    B00100,
    B00100};

uint8_t topoParedeChar[] = {
    B11111,
    B11111,
    B01111,
    B01111,
    B01111,
    B01011,
    B00011,
    B00001};

uint8_t baixoParedeChar[] = {
    B01001, B01011, B01011, B01111,
    B01111, B01111, B11111, B11111};

// === VARIÁVEIS DE CONTROLE ===
bool paredeJaSumiu = false;

// === FUNÇÕES DE DESENHO ===
void desenharParedeGelo()
{
    lcd.setCursor(15, 0);
    lcd.write(2); // topo
    lcd.setCursor(16, 0);
    lcd.write((uint8_t)255);
    lcd.setCursor(16, 1);
    lcd.write(2);
    lcd.setCursor(15, 3);
    lcd.write(3); // base
    lcd.setCursor(16, 3);
    lcd.write((uint8_t)255);
    lcd.setCursor(16, 2);
    lcd.write(3);
}

void apagarParedeGelo()
{
    lcd.setCursor(15, 0);
    lcd.print(" "); // topo
    lcd.setCursor(16, 0);
    lcd.print(" ");
    lcd.setCursor(16, 1);
    lcd.print(" ");
    lcd.setCursor(15, 3);
    lcd.print(" ");
    lcd.setCursor(16, 3);
    lcd.print(" ");
    lcd.setCursor(16, 2);
    lcd.print(" ");
    lcd.setCursor(13, 2);
    lcd.print("!");
}

void desenharArvore()
{
    lcd.setCursor(6, 3);
    lcd.write(4);
    lcd.setCursor(10, 3);
    lcd.write(4);
}

void piscarParedeGelo(int vezes, int intervalo)
{
    for (int i = 0; i < vezes; i++)
    {
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

void trocaDeFase1();
void trocaDeFase2();
void art();
void ataqueInimigo();
void ADPersonagem();
void Vitoria();

void setup()
{
    Serial.begin(9600);

    dht.begin();
    pinMode(ldr, INPUT);
    pinMode(potenciometro, INPUT);

    acelerometro.begin();
    acelerometro.setRange(MMA8451_RANGE_4_G);

    MQ135.setRegressionMethod(1);
    MQ135.setA(110.47); // valor do gas carbonico minimo
    MQ135.setB(-2.862); // valor maximo do gas carbonico
    MQ135.init();
    MQ135.update();            // atualiza o valor do sensor
    R0 = MQ135.calibrate(3.6); // calibra o valor do ar
    MQ135.setR0(R0);

    //===controle===
    pinMode(botaoA, INPUT_PULLUP);
    pinMode(botaoB, INPUT_PULLUP);
    pinMode(botaoC, INPUT_PULLUP);
    pinMode(botaoD, INPUT_PULLUP);
    pinMode(botaoE, INPUT_PULLUP);
    pinMode(botaoF, INPUT_PULLUP);

    lcd.init();
    lcd.backlight();
    // tratamento lcd fase 2
    lcd.createChar(2, topoParedeChar);
    lcd.createChar(3, baixoParedeChar);
    lcd.createChar(4, ArvoreChar);

    lcd.setCursor(0, 0);
    lcd.clear();

    desenharParedeGelo();
    desenharArvore();
}

void loop()
{

    float temperatura = dht.readTemperature();
    float umidade = dht.readHumidity();
    int valorLDR = analogRead(ldr);

    int valorPotenciometro = analogRead(potenciometro);
    Serial.println(valorLDR);

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

    if (fase == 2)
    {

        // === SERIAL ===
        Serial.print("Temp: ");
        Serial.print(temperatura);
        Serial.print(" °C, Umidade: ");
        Serial.print(umidade);
        Serial.println(" %");

        // === Lógica de desaparecimento da parede ===
        if (temperatura > 28 && !paredeJaSumiu)
        {
            piscarParedeGelo(2, 500);
            lcd.setCursor(13, 2);
            lcd.print(" ");
            lcd.setCursor(15, 3);
            lcd.print(""); // oque sobrou da parede de gelo
            estadoParede = false;
        }

        if (estadoParede == false)
        {

            while (contagem < 3)
            {
                trocaDeFase1();
                delay(1000);
                trocaDeFase2();
                contagem++;
                fase = 3;
            }
        }
    }

    else if (fase == 3)
    {
        // zona morta joystick
        if (movimentacaox > 1000 && movimentacaox < 3000 && valorLDR >= 1500)
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

            else if (counter == 14 && estadoBotaoB == true)
            {
                lcd.setCursor(3, 2);
                lcd.write((uint8_t)255);
                porta = true;
            }

            else if (counter == 3 && valorPotenciometro > 4000 && porta == true)
            {
                while (contagem >= 3 && contagem <= 6)
                {
                    trocaDeFase2();
                    trocaDeFase1();
                    delay(1000);
                    trocaDeFase2();
                    contagem++;
                    fase = 4;
                }
            }
        }

        if (valorLDR > 1500)
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

        else
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
    }

    else if (fase == 4)
    {
        lcd.backlight();
        if (fase == 4 && arte == 0)
        {
            lcd.clear();
            art();
            arte = 1;
        }
        if (vidaInimiga == 0)
        {
            Vitoria();
        }
        delay(100);
        ADPersonagem();
        ataqueInimigo();
    }
}

void trocaDeFase1()
{
    for (int coluna = 0; coluna < 20; coluna++)
    {
        lcd.setCursor(coluna, 0);
        lcd.write((uint8_t)255);
        lcd.setCursor(coluna, 3);
        lcd.write((uint8_t)255);
        lcd.setCursor(coluna, 1);
        lcd.write((uint8_t)255);
        lcd.setCursor(coluna, 2);
        lcd.write((uint8_t)255);
    }
}

void trocaDeFase2()
{
    for (int coluna = 0; coluna < 20; coluna++)
    {
        lcd.setCursor(coluna, 0);
        lcd.print(" ");
        lcd.setCursor(coluna, 3);
        lcd.print(" ");
        lcd.setCursor(coluna, 1);
        lcd.print(" ");
        lcd.setCursor(coluna, 2);
        lcd.print(" ");
    }
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
        B01010};

    byte thiago[] = {
        B01110,
        B10001,
        B11011,
        B11111,
        B01110,
        B10101,
        B00100,
        B01010};

    byte sholdbi[] = {
        B01110,
        B10001,
        B11011,
        B10001,
        B01110,
        B10101,
        B00100,
        B01010};

    byte candelabro[] = {
        B00100,
        B00100,
        B10101,
        B10101,
        B01010,
        B00000,
        B00000,
        B00000};

    byte janela[] = {
        B00000,
        B11111,
        B10101,
        B10101,
        B11111,
        B10101,
        B10101,
        B11111};

    lcd.createChar(0, personagem);
    lcd.createChar(1, thiago);
    lcd.createChar(5, sholdbi);
    lcd.createChar(6, janela);
    lcd.setCursor(0, 2);
    lcd.write(0); // personagem principal

    lcd.setCursor(17, 2);
    lcd.write(5); // sholdibi

    lcd.setCursor(4, 1);
    lcd.write(6); // janela

    lcd.setCursor(14, 1);
    lcd.write(6); // janela

    lcd.setCursor(9, 0);
    lcd.write(1); // thiago

    lcd.setCursor(8, 0);
    lcd.print("|");

    lcd.setCursor(10, 0);
    lcd.print("|");

    lcd.setCursor(8, 1);
    lcd.print("---");

    lcd.setCursor(15, 0);
    lcd.print("ST0/2");
}

void ataqueInimigo()
{
    float tempo = millis();
    if (tempo - tempoPassado >= tempoAtaque)
    {
        for (i = 0; i <= 13; i++)
        {
            lcd.setCursor(movimentoAtaque, 2);
            lcd.print("<- ");
            movimentoAtaque -= 1;
            delay(500);
        }

        if (AD == 0)
        {
            lcd.clear();
            lcd.setCursor(6, 1);
            lcd.print("GAME OVER");
            lcd.setCursor(4, 2);
            lcd.print("Sholdibi Wins");
            while (1)
                ;
        }
        if (AD == 1)
        {
            movimentoAtaque = 14;
            lcd.setCursor(1, 2);
            lcd.print("|");
            delay(200);
            lcd.setCursor(1, 2);
            lcd.print("   ");
            stamina -= 1;
            lcdST += 1;
            lcd.setCursor(17, 0);
            lcd.print(lcdST);

            tempoPassado = tempo;
            tempoPassado += 7000;
            tempoAtaque -= 200;
        }
        if (AD == 2)
        {
            movimentoAtaque = 14;
            lcd.setCursor(0, 2);
            lcd.print("   ");
            lcd.setCursor(16, 2);
            lcd.write(0);
            delay(500);
            lcd.setCursor(16, 2);
            lcd.print(" ");
            lcd.setCursor(0, 2);
            lcd.write(0);
            stamina = 2;
            vidaInimiga -= 1;
            lcdST = 0;
            lcd.setCursor(17, 0);
            lcd.print(lcdST);

            tempoPassado = tempo;
            tempoPassado += 7000;
            tempoAtaque -= 200;
        }
    }
}

void ADPersonagem()
{
    acelerometro.read();
    eixoY = acelerometro.y_g;
    MQ135.update();
    bafo = MQ135.readSensor(); // le o valor atualizado pelo update

    if (eixoY > 0.4 && stamina > 0) // defende
    {
        AD = 1;
    }
    else if (eixoY < -0.4 && stamina == 0) // ataca
    {
        AD = 2;
    }
    else
    {
        AD = 0;
    }
}

void Vitoria()
{
    lcd.clear();
    lcd.setCursor(2, 1);
    lcd.print("matou o sholdibi");
    delay(1000);
    lcd.clear();
    lcd.setCursor(9, 0);
    lcd.write(1);
    delay(500);
    lcd.clear();
    lcd.setCursor(9, 1);
    lcd.write(1);
    delay(500);
    lcd.clear();
    lcd.setCursor(5, 0);
    lcd.print("Thiago:");
    lcd.setCursor(3, 1);
    lcd.print("muito obrigado");
    lcd.setCursor(9, 2);
    lcd.write(1);
    while (1)
        ;
}
