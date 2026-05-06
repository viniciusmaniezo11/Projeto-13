//! secrets

#ifndef secrets_h
#define secrets_h

// configurações do WiFi
extern const char *WIFI_SSID;
extern const char *WIFI_SENHA;

// ===================================
// MQTT
extern const bool USAR_AWS_IOT;

extern const char *MQTT_BROKER;
extern const int MQTT_PORTA;

extern const char *MQTT_CLIENT_ID;

extern const char *MQTT_USUARIO;
extern const char *MQTT_SENHA;

extern const bool MQTT_USAR_TLS; // modo de conexao sem segurança

extern const char MQTT_CERTIFICADO_CA[];

extern const char *TOPICOS_PUBLICAR[];

extern const int TOTAL_TOPICOS_PUBLICAR;

extern const char *TOPICOS_RECEBER[];

extern const int TOTAL_TOPICOS_RECEBER;

// ===================================
// DEBUG

// 0 = SEM MENSAGENS
// 1 = APENAS ERROS
// 2 = TODAS AS MENSAGENS
extern const int DEBUG_NIVEL_INICIAL;

// PINO USADO PARA FORÇAR TODAS AS MENSAGENS NA INICIALIZAÇÃO
extern const int DEBUG_HABILITA_DEBUG_COMPLETO;

#endif