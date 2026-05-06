//! WiFiManager.cpp
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <WiFiClient.h>
#include "WiFiManager.h"
#include "secrets.h"
#include "deBugManager.h"

bool wifiEstaConectado ()
{
    return WiFi.status() == WL_CONNECTED;
}

void conectarWiFi()
 {
  debugInfo("==================");
  debugInfo("Iniciando Conexão WiFi...");
  debugInfo("==================");

  //configura o ESP32 como station, ou seja, ele vai se conectar a um roteador existente
  WiFi.mode(WIFI_STA); //

  //inicia a conexao com SSID e senha
  WiFi.begin(WIFI_SSID, WIFI_SENHA);

  debugInfo("conectando");

  int tentativasWiFi = 0;
  const int maxTentativasWiFi = 30;

  // aguarda a conexão por até 30 tentativas
  while(WiFi.status() != WL_CONNECTED && tentativasWiFi < maxTentativasWiFi)
  {
    delay(500);
    debugInfoSemLinha(".");
    tentativasWiFi++;
  }

  debugInfo(" ");

  if(WiFi.status() == WL_CONNECTED)
  {
    debugInfo("WiFi conectado com sucesso!");
    debugInfoSemLinha("[INFO] endereço IP: ");
    debugInfoSemLinha( WiFi.localIP() .toString());
    debugInfoSemLinha("\n\r");
  }
  else
  {
    debugErro("falha ao conectar no WiFi");
    debugErro("Verifique SSID, senha e sinal de rede.");
  }
 }

 void garantirWiFiConectado()
 {
  if(WiFi.status() != WL_CONNECTED)
  {
    debugErro("WiFi desconectado. tentando reconectar...");
    conectarWiFi();
  }

  if(WiFi.status() !=WL_CONNECTED)
  {
    debugErro("Não foi possivel reconectar ao WiFi.");
  }
 }