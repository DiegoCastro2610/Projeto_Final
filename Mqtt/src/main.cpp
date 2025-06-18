#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include <DHT.h>

// Mqtt
WiFiClient espClient;
PubSubClient client(espClient);

const char *mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;
const char *mqtt_id = "esp32-senai134-projetoFinal";
const char *mqtt_topic_sub = "senai134/grupo2/esp_inscrito";
const char *mqtt_topic_pub = "senai134/grupo2/esp_publicando";

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

// Controle botoes
#define botaoA 14
#define botaoB 12
#define botaoC 13
#define botaoD 33
#define botaoE 25
#define botaoF 26

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 20, 4); // Endereço I2C e dimensões do LCD

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
    lcd.write(6);
    lcd.setCursor(10, 3);
    lcd.write(6);
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

// Jason
void callback(char *, byte *, unsigned int);
void mqttConnect(void);

void setup()
{
    Serial.begin(9600);

    dht.begin();
    pinMode(ldr, INPUT);
    pinMode(potenciometro, INPUT);

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
    lcd.createChar(6, ArvoreChar);

    lcd.setCursor(0, 0);
    lcd.clear();

    client.setServer(mqtt_server, mqtt_port);
    client.setCallback(callback);

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

void callback(char *topic, byte *payload, unsigned int length)
{
    Serial.printf("mensagem recebida em %s: ", topic);

    String mensagem = "";
    for (unsigned int i = 0; i < length; i++)
    {
        char c = (char)payload[i];
        mensagem += c;
    }
    Serial.println(mensagem);
}

void mqttConnect()
{
    while (!client.connected())
    {
        Serial.println("Conectando ao MQTT...");

        if (client.connect(mqtt_id))
        {
            Serial.println("Conectado com sucesso");
            client.subscribe(mqtt_topic_sub);
        }

        else
        {
            Serial.print("falha, rc=");
            Serial.println(client.state());
            Serial.println("tentando novamente em 5 segundos");
            delay(5000);
        }
    }
}