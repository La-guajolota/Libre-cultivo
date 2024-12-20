#include <WiFi.h>
#include <PicoMQTT.h>

// Configuración Wi-Fi
const char* ssid = "Tu_SSID";          // Tu red Wi-Fi
const char* password = "Tu_Contraseña"; // Tu contraseña Wi-Fi

// Puerto para el broker MQTT
#define MQTT_PORT 1883

// Clase personalizada de servidor MQTT con autenticación
class MyMQTT : public PicoMQTT::Server {
public:
  // Constructor que establece el puerto MQTT
  MyMQTT(uint16_t port) : PicoMQTT::Server(port) {}

protected:
  // Función de autenticación
  PicoMQTT::ConnectReturnCode auth(const char *client_id, const char *username, const char *password) override {
    if (String(client_id).length() < 3) {
      return PicoMQTT::CRC_IDENTIFIER_REJECTED;
    }
    if (!username || !password) {
      return PicoMQTT::CRC_NOT_AUTHORIZED;
    }
    // Autenticación con usuario y contraseña
    if (
      ((String(username) == "adrian") && (String(password) == "librecultivo")) ||
      ((String(username) == "juan") && (String(password) == "pepexd"))
    ) {
      return PicoMQTT::CRC_ACCEPTED;
    }
    return PicoMQTT::CRC_BAD_USERNAME_OR_PASSWORD;
  }
};

// Crear una instancia del servidor MQTT
MyMQTT mqttServer(MQTT_PORT);

void setup() {
  // Inicializar comunicación serial
  Serial.begin(115200);

  // Conectar a Wi-Fi
  Serial.printf("Conectando a %s\n", ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando...");
  }
  Serial.printf("Conexión exitosa! IP: %s\n", WiFi.localIP().toString().c_str());

  // Iniciar el servidor MQTT
  mqttServer.begin();
}

void loop() {
  // Mantener el loop de MQTT
  mqttServer.loop();

  // Publicar un mensaje cada 3 segundos
  mqttServer.publish("picomqtt/welcome", "¡Hola desde el ESP32 Broker!");
  delay(3000);
}
