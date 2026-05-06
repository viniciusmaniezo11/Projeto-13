#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Bounce2.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

uint8_t tela = 0;
String resposta;

void tela1();
void tela2();
void tela3();
void atualizarTela();

const char *URL_API = "https://api.weatherapi.com/v1/current.json?key=c11009a9d28749e6af4164128261604&q=Sao%20Caetano%20do%20Sul&lang=pt";

Bounce botaoBoot = Bounce();

void setup() 
{
  botaoBoot.attach(0, INPUT_PULLUP);
  lcd.init();
  lcd.backlight();
}

void loop() 
{

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
