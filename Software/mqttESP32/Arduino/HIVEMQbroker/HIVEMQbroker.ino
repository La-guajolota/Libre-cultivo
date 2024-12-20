#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>

// Configuración Wi-Fi
const char* ssid = "Mega_2.4G_3512";           // Tu red Wi-Fi
const char* password = "Ph5YcEUs";              // Tu contraseña Wi-Fi

// Configuración de HiveMQ Broker
const char* mqtt_server = "06b1f4b4bb38490380fee42cee1fd651.s1.eu.hivemq.cloud";  // Cluster URL
const int mqtt_port = 8883;             // Puerto para conexión segura (TLS)

// Tópicos de suscripción y publicación
const char* subscribe_topic = "test/receive";  // Tópico de suscripción
const char* publish_topic = "test/send";      // Tópico de publicación

WiFiClientSecure espClient;  // Cliente MQTT sobre SSL/TLS
PubSubClient client(espClient);

// Función de callback
void callback(char* topic, byte* payload, unsigned int length) {
  String message = "";
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.println("Mensaje recibido: " + message);
}

void setup() {
  // Inicializar comunicación serial
  Serial.begin(115200);
  
  // Conectar a Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando...");
  }
  Serial.println("Conexión exitosa!");

  // Configurar el cliente MQTT con el certificado raíz (si es necesario)
  // Para simplificar este ejemplo, no estamos usando un certificado.
  // Si HiveMQ requiere un certificado SSL, deberás agregar el certificado raíz como se mostró anteriormente.
  
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);  // Establecer la función de callback

  // Intentar conectar al broker
  connectToMQTT();
}

void connectToMQTT() {
  while (!client.connected()) {
    Serial.print("Conectando a MQTT...");
    if (client.connect("ESP32Client")) {  // Conectar sin autenticación
      Serial.println("Conectado a MQTT!");
      client.subscribe(subscribe_topic);  // Suscribirse al tópico
    } else {
      Serial.print("Error, rc=");
      Serial.print(client.state());
      delay(5000);
    }
  }
}

void loop() {
  // Mantener la conexión MQTT y procesar los mensajes recibidos
  client.loop();

  // Publicar un mensaje cada 5 segundos
  static long lastMsgTime = 0;
  long now = millis();
  if (now - lastMsgTime > 5000) {  // Publicar cada 5 segundos
    lastMsgTime = now;
    String msg = "Mensaje de prueba desde ESP32";
    Serial.println("Publicando mensaje...");
    client.publish(publish_topic, msg.c_str());
  }
}

