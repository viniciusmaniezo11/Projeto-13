
//! MqttManager.h
#ifndef MQTTMANAGER_H
#define MQTTMANAGER_H

#include <Arduino.h>

void configurarMQTT();
void conectarMQTT();
void garantirMQTTConectado();
void loopMQTT();

bool mqttEstaConectado();

void publicarMensagem(const char *topico, const char *mensagem);
void publicarMensagemNoTopico(int indiceTopico, const char *mensagem);

// todo: criar parametro do tipo int chamado indice topico para as funcões abaixo
const char *obterTopicoPublicacao(int indiceTopico);
const char *obterTopicoRecebimento(int indiceTopico);

int obterTotaltopicosRecebimento();

typedef void (*CallbackMensageMQTT)(const char *topico, const String &mensagem);

void registrarCallbackMensagem(CallbackMensageMQTT callback);
int obterTotalTopicosRecebimento();

#endif