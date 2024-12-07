#include <PicoMQTT.h>

#define WIFI_SSID ""
#define WIFI_PASSWORD ""
String val = "";

// Instancia del servidor MQTT
PicoMQTT::Server mqtt;

class MyMQTT : public PicoMQTT::Server {
  // Sobrescribir la función de autenticación
  protected:
    PicoMQTT::ConnectReturnCode auth(const char *client_id, const char *username, const char *password) override {
      // Aceptamos clientes con ID mayores a 3 o más caracteres
      if (String(client_id).length() < 3) {  // Se usa length() en lugar de lenght()
        return PicoMQTT::CRC_IDENTIFIER_REJECTED;
      }

      // Aceptamos la conexión si el username y password son dados
      if (!username || !password) {  // username y password pueden ser null
        return PicoMQTT::CRC_NOT_AUTHORIZED;
      }

      // Aceptamos dos combinaciones user/password
      if (
        ((String(username) == "adrian") && (String(password) == "librecultivo")) ||
        ((String(username) == "juan") && (String(password) == "pepexd"))
      ) {
        return PicoMQTT::CRC_ACCEPTED;
      }

      // Rechazamos las demás credenciales
      return PicoMQTT::CRC_BAD_USERNAME_OR_PASSWORD;
    }
};

// Crear una instancia del servidor MQTT que usa la clase personalizada
MyMQTT mqttServer;

void setup() {
  Serial.begin(115200);

  // Nos conectamos a la red Wi-Fi
  Serial.printf("Conectando a %s\n", WIFI_SSID);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }
  Serial.printf("Conexión exitosa! IP: %s\n", WiFi.localIP().toString().c_str());

  // Suscripción a un tópico, colocamos un callback para recibir los mensajes
  mqttServer.subscribe("#", [](const char* topic, const char* payload) {
    Serial.printf("Mensaje recibido en el tópico %s: %s\n", topic, payload);
  });

  mqttServer.begin();
}

void loop() {
  mqttServer.loop();

  // Publicar un mensaje aleatorio cada cierto tiempo
  mqttServer.publish("picomqtt/welcome", "wasaaaaaa guajoloamigo SOY LA ESP32");
  val = String(random(100));
  mqttServer.publish("picomqtt/randomnum", val);
  delay(3000);  // Espera 5 segundos antes de publicar nuevamente
}
