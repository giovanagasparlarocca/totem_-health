
#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>  // Biblioteca do DHT

// ------------------- CONFIGURAÇÕES -------------------
const char* default_SSID = "Gigalaxy"; // Nome da rede Wi-Fi
const char* default_PASSWORD = "LaroccaS2"; // Senha da rede Wi-Fi     // Senha da rede Wi-Fi
const char* default_BROKER_MQTT = "128.24.112.170"; // IP do Broker MQTT
const int default_BROKER_PORT = 1883;    // Porta do Broker MQTT
const char* default_TOPICO_SUBSCRIBE = "/TEF/lamp001/cmd"; 
const char* default_TOPICO_PUBLISH_1 = "/TEF/lamp001/attrs";
const char* default_TOPICO_PUBLISH_2 = "/TEF/lamp001/attrs/l";
const char* default_ID_MQTT = "fiware_001";
const int default_D4 = 2;  // Pino do LED onboard
const int DHTPIN = 4;      // <<< GPIO2 = D4 no NodeMCU/Wokwi
#define DHTTYPE DHT11      // Tipo do sensor alterado para DHT11

const char* topicPrefix = "lamp001";

// ------------------- VARIÁVEIS EDITÁVEIS -------------------
char* SSID = const_cast<char*>(default_SSID);
char* PASSWORD = const_cast<char*>(default_PASSWORD);
char* BROKER_MQTT = const_cast<char*>(default_BROKER_MQTT);
int BROKER_PORT = default_BROKER_PORT;
char* TOPICO_SUBSCRIBE = const_cast<char*>(default_TOPICO_SUBSCRIBE);
char* TOPICO_PUBLISH_1 = const_cast<char*>(default_TOPICO_PUBLISH_1);
char* TOPICO_PUBLISH_2 = const_cast<char*>(default_TOPICO_PUBLISH_2);
char* ID_MQTT = const_cast<char*>(default_ID_MQTT);
int D4 = default_D4;

WiFiClient espClient;
PubSubClient MQTT(espClient);
char EstadoSaida = '0';

// Criando objeto DHT
DHT dht(DHTPIN, DHTTYPE);

void initSerial() {
  Serial.begin(115200);
}

void initWiFi() {
  delay(10);
  Serial.println("------Conexao WI-FI------");
  Serial.print("Conectando-se na rede: ");
  Serial.println(SSID);
  Serial.println("Aguarde");
  reconectWiFi();
}

void initMQTT() {
  MQTT.setServer(BROKER_MQTT, BROKER_PORT);
  MQTT.setCallback(mqtt_callback);
}

void setup() {
  InitOutput();
  initSerial();
  dht.begin();  // Inicializa o DHT11
  initWiFi();
  initMQTT();
  delay(5000);
  MQTT.publish(TOPICO_PUBLISH_1, "s|on");
}

void loop() {
  VerificaConexoesWiFIEMQTT();
  EnviaEstadoOutputMQTT();
  handleLuminosity();  // Lê temperatura do DHT11
  MQTT.loop();
}

void reconectWiFi() {
  if (WiFi.status() == WL_CONNECTED)
    return;
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Conectado com sucesso na rede ");
  Serial.print(SSID);
  Serial.println("IP obtido: ");
  Serial.println(WiFi.localIP());

  digitalWrite(D4, LOW);  // Garante que LED inicie desligado
}

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  String msg;
  for (int i = 0; i < length; i++) {
    char c = (char)payload[i];
    msg += c;
  }
  Serial.print("- Mensagem recebida: ");
  Serial.println(msg);

  String onTopic = String(topicPrefix) + "@on|";
  String offTopic = String(topicPrefix) + "@off|";

  if (msg.equals(onTopic)) {
    digitalWrite(D4, HIGH);
    EstadoSaida = '1';
  }

  if (msg.equals(offTopic)) {
    digitalWrite(D4, LOW);
    EstadoSaida = '0';
  }
}

void VerificaConexoesWiFIEMQTT() {
  if (!MQTT.connected())
    reconnectMQTT();
  reconectWiFi();
}

void EnviaEstadoOutputMQTT() {
  if (EstadoSaida == '1') {
    MQTT.publish(TOPICO_PUBLISH_1, "s|on");
    Serial.println("- Led Ligado");
  }

  if (EstadoSaida == '0') {
    MQTT.publish(TOPICO_PUBLISH_1, "s|off");
    Serial.println("- Led Desligado");
  }
  Serial.println("- Estado do LED onboard enviado ao broker!");
  delay(1000);
}

void InitOutput() {
  pinMode(D4, OUTPUT);
  digitalWrite(D4, HIGH);
  boolean toggle = false;

  for (int i = 0; i <= 10; i++) {
    toggle = !toggle;
    digitalWrite(D4, toggle);
    delay(200);
  }
}

void reconnectMQTT() {
  while (!MQTT.connected()) {
    Serial.print("* Tentando se conectar ao Broker MQTT: ");
    Serial.println(BROKER_MQTT);
    if (MQTT.connect(ID_MQTT)) {
      Serial.println("Conectado com sucesso ao broker MQTT!");
      MQTT.subscribe(TOPICO_SUBSCRIBE);
    } else {
      Serial.println("Falha ao reconectar no broker.");
      Serial.println("Haverá nova tentativa de conexão em 2s");
      delay(2000);
    }
  }
}

// ------------------- LEITURA DE TEMPERATURA -------------------
void handleLuminosity() {
  float temperatura = dht.readTemperature(); // Lê temperatura em °C

  if (isnan(temperatura)) {
    Serial.println("Falha ao ler DHT11!");
    return;
  }

  int tempInt = (int)temperatura; // Converte para inteiro, como no JSON
  Serial.print("Temperatura lida: ");
  Serial.println(tempInt);

  // Publica o valor como número (inteiro)
  char buffer[10];
  itoa(tempInt, buffer, 10);
  MQTT.publish(TOPICO_PUBLISH_2, buffer);
}
