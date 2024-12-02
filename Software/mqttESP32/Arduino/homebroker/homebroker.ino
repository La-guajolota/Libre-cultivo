#include <PicoMQTT.h>



PicoMQTT::Server mqtt;

void setup() {
  Serial.begin(115200);

  //Nos conectamor a la red de la casa
  Serial.printf("Conectando %s\n",WIFI_SSID);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID,WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {delay(1000);}
  Serial.printf("Conexión exitosa! IP: %s\n",WiFi.localIP().toString().c_str());

  //Suscripción a un topic pattern colocamos un callback
  mqtt.subscribe("#",[](const char* topic, const char* payload){
    Serial.printf("Mensaje recivido en tópico; %s: %s\n", topic, payload);
  });
  mqtt.begin();
}

void loop() {
  mqtt.loop();
  if(random(1000)==0){
    mqtt.publish("picomqtt/welcome","wasaaaaaa guajoloamigo");
    delay(5000);
  }
}
