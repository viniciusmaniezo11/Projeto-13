#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <ArduinoJson.h>
#include "WiFiManager.h"
#include "MQTTManager.h"
#include "deBugManager.h"
/**
 * Nome: Vinicius Atanasio de Sousa Alves
 * data: 24/04/2026
 * projeto : conexao MQTT
 * descrição: MQTT 
 * versão: 1.0
 */

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

void setup()
{
  configurarDebug();
  configurarLedRGB();
  conectarWiFi();
  configurarMQTT();
  registrarCallbackMensagem(tratarMensagemRecebida);
  conectarMQTT();
}

void loop()
{
  garantirWiFiConectado();
  garantirMQTTConectado();
  loopMQTT();
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
  ledRGB.setBrightness(80); // colocamos a qnt de brilho para o led de 0 a 255
  ledRGB.clear();
  ledRGB.show(); // atualiza o estado led (mudar de cor, etc)
  debugInfo("LED RGB configurado no GPIO " + String(PINO_LED_RGB));
}

void alterarCorLedRGB(int vermelho, int verde, int azul)
{
  vermelho = constrain(vermelho, 0, 255); // constrain() limita o valor do vermelho para no minimo 0 e no maximo 255
  verde = constrain(verde, 0, 255);
  azul = constrain(azul, 0, 255);
  ledRGB.setPixelColor(0, ledRGB.Color(vermelho, verde, azul)); // primeiro coloca o led que queremos mecher (0), e dps as cores
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
    if (!doc["led"]["r"].is<int>() || !doc["led"]["g"].is<int>() || !doc["led"]["b"].is<int>())
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
  if(doc["lampada"].is<JsonObject>())
  {
    if(!doc["lampada"].is<String>())
    {
      debugErro("JSON INVALIDO. use true ou false");
      return;
    }
    else
    {
     estadoLed = doc["lampada"].as<bool>();
    }
  }
}