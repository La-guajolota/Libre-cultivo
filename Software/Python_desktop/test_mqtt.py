import paho.mqtt.client as mqtt
import time

# Dirección IP de la ESP32 que está actuando como broker
BROKER_IP = "192.168.1.100"  # Sustituye con la IP real de tu ESP32
BROKER_PORT = 1883  # Puerto por defecto de MQTT

# El topic al que te quieres suscribir
SUBSCRIBE_TOPIC = "#"  # Se suscribe a todos los tópicos
PUBLISH_TOPIC = "rack0/sens/ambient/temperature"  # El tópico en el que vas a publicar

# Tiempo entre publicaciones en segundos
PUBLISH_INTERVAL = 5  # Publicar cada 5 segundos

# Función de callback que se ejecuta cuando el cliente recibe un mensaje
def on_message(client, userdata, msg):
    print(f"Mensaje recibido en el tópico {msg.topic}: {msg.payload.decode()}")

# Crear un nuevo cliente MQTT
client = mqtt.Client()

# Definir el comportamiento cuando el cliente se conecta al broker
def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Conexión exitosa al broker MQTT")
        # Suscribirse al tópico
        client.subscribe(SUBSCRIBE_TOPIC)
    else:
        print(f"Error al conectar con el broker: {rc}")

# Establecer la función de callback de conexión
client.on_connect = on_connect
# Establecer la función de callback para mensajes
client.on_message = on_message

# Conectarse al broker MQTT
client.connect(BROKER_IP, BROKER_PORT, 60)

# Iniciar el bucle de espera de mensajes
client.loop_start()  # Cambiar a loop_start() para que el cliente siga ejecutándose en paralelo

# Publicar un mensaje periódicamente
try:
    while True:
        message = "¡Hola desde Python! Este es un mensaje periódico."
        client.publish(PUBLISH_TOPIC, message)
        print(f"Publicado en el tópico {PUBLISH_TOPIC}: {message}")
        time.sleep(PUBLISH_INTERVAL)  # Espera antes de publicar el siguiente mensaje
except KeyboardInterrupt:
    print("Interrumpido por el usuario")

# Detener el bucle cuando termine
client.loop_stop()  # Detiene el bucle cuando se interrumpe
