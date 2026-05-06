#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <LiquidCrystal_I2C.h>
#include <ArduinoJson.h>
#include <Bounce2.h>
#include "WiFiManager.h"
#include "MQTTManager.h"
#include "deBugManager.h"


LiquidCrystal_I2C lcd(0x27, 20, 4);


/**
 * Nome: Vinicius Atanasio de Sousa Alves
 * data: 24/04/2026
 * projeto : conexao MQTT
 * descrição: MQTT 
 * versão: 1.0
 */

uint8_t tela = 0;
String resposta;

Bounce botaoBoot = Bounce();

const int PINO_LED_RGB = 48;
const int QUANTIDADE_LEDS = 1;
const char TOPICO_COMANDO[] = "senai134/consultorio1/esp32/comando";
const int estadoPino = 12;
bool estadoLed = 0;

Adafruit_NeoPixel ledRGB(
    QUANTIDADE_LEDS,
    PINO_LED_RGB,
    NEO_GRB + NEO_KHZ800);

void tratarMensagemRecebida(const char *topico, const String &mensagem);
void configurarLedRGB();
void alterarCorLedRGB(int vermelho, int verde, int azul);
void tratarJsonComando(const String &mensagem);

void tela1();
void tela2();
void tela3();
void atualizarTela();

const char *URL_API = "https://api.weatherapi.com/v1/current.json?key=c11009a9d28749e6af4164128261604&q=Sao%20Caetano%20do%20Sul&lang=pt";


void setup()
{
  botaoBoot.attach(0, INPUT_PULLUP);
  configurarDebug();
  configurarLedRGB();
  conectarWiFi();
  configurarMQTT();
  registrarCallbackMensagem(tratarMensagemRecebida);
  conectarMQTT();
  lcd.init();
  lcd.backlight();

}

void loop()
{
  garantirWiFiConectado();
  garantirMQTTConectado();
  loopMQTT();

  unsigned long tempo = botaoBoot.previousDuration();
  unsigned long tempo2 = botaoBoot.currentDuration();

  botaoBoot.update();
  static bool estadoBotaoBoot = 1;
  bool estadoAnteriorBotaoBoot = estadoBotaoBoot;
  static bool houveTroca = false;

  if (botaoBoot.changed()) 
  {
    estadoBotaoBoot = botaoBoot.read();
  }

  if (botaoBoot.fell()) 
  {
    tela++;
    if (tela > 2)
    {
      tela = 0;
      houveTroca = true;
    }
  }
}

void tratarMensagemRecebida(const char *topico, const String &mensagem)
{
  debugInfo("===============================");
  debugInfo("Mensagem recebida na aplicação");
  debugInfo("===============================");

  if (topico == nullptr)
  {
    debugErro("Topico MQTT invalido");
    return;
  }

  debugInfo("Topico: " + String(topico));
  debugInfo("Mensagem" + mensagem);

  if (strcmp(topico, TOPICO_COMANDO) == 0)
  {
    tratarJsonComando(mensagem);
    return;
  }
  debugErro("Topico nao tratado: " + String(topico));
}

void configurarLedRGB()
{
  ledRGB.begin();
  ledRGB.setBrightness(80); 
  ledRGB.clear();
  ledRGB.show(); 
  debugInfo("LED RGB configurado no GPIO " + String(PINO_LED_RGB));
}

void alterarCorLedRGB(int vermelho, int verde, int azul)
{
  vermelho = constrain(vermelho, 0, 255); 
  verde = constrain(verde, 0, 255);
  azul = constrain(azul, 0, 255);
  ledRGB.setPixelColor(0, ledRGB.Color(vermelho, verde, azul));
  ledRGB.show();

  debugInfo("Cor aplicada no LED RGB");
  debugInfo("R: " + String(vermelho));
  debugInfo("G: " + String(verde));
  debugInfo("B: " + String(azul));
}

void tratarJsonComando(const String &mensagem)
{
  JsonDocument doc;
  DeserializationError erro = deserializeJson(doc, mensagem);

  if (erro)
  {
    debugErro("erro ao interpretar o JSON.");
    debugErro(erro.c_str());
    return;
  }

  if (doc["led"].is<JsonObject>())
  {
    if (!doc["led"]["r"].is<int>() || 
        !doc["led"]["g"].is<int>() || 
        !doc["led"]["b"].is<int>())
    {
      debugErro("JSON INVALIDO. use led.r, led.g, led.b");
      return;
    }
    else
    {
      int vermelho = doc["led"]["r"].as<int>();
      int verde = doc["led"]["g"].as<int>();
      int azul = doc["led"]["b"].as<int>();

      alterarCorLedRGB(vermelho, verde, azul);
    }
  }
}

void AtualizarTela()
{
  switch (tela)
  {
  case 0:
    tela1();
    break;
  case 1:
    tela2();
    break;
  case 2:
    tela3();
    break;
  }
}

void tela1()
{
   JsonDocument doc;
   DeserializationError erro = deserializeJson(doc, resposta);

  if(!erro)
  {
    lcd.setCursor(0,0);
    lcd.print("Consultório disponível");

    lcd.setCursor(0,2);
    lcd.print("Doutor Thiago");

    if(doc["location"]["localtime"].is<JsonVariant>())
    {
      const char *hora = doc["location"]["localtime"];
      lcd.setCursor(0,3);
      lcd.printf("%d:%d", hora);
    }
  }
}

void tela2()
{
   JsonDocument doc;
   DeserializationError erro = deserializeJson(doc, resposta);

   if(!erro)
   {
      lcd.setCursor(0,0);
      lcd.print("Consultório ocupado");

      lcd.setCursor(0,2);
      lcd.print("Doutor Thiago");

      if(doc["location"]["localtime"].is<JsonVariant>())
      {
         const char *hora = doc["location"]["localtime"];
         lcd.setCursor(0,3);
         lcd.printf("%d:%d", hora);
      }
   }
}

void tela3()
{
   JsonDocument doc;
   DeserializationError erro = deserializeJson(doc, resposta);

   if(!erro)
   {
      lcd.setCursor(0,0);
      lcd.print("Consultório fechado");

      lcd.setCursor(0,2);
      lcd.print("Sem doutor");

      if(doc["location"]["localtime"].is<JsonVariant>())
      {
         const char *hora = doc["location"]["localtime"];
         lcd.setCursor(0,3);
         lcd.printf("%d:%d", hora);
      }
   }
}
