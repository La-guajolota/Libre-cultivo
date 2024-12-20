import paho.mqtt.client as mqtt
import time

# Dirección IP de la ESP32 que está actuando como broker
BROKER_IP = "192.168.1.100"  # Sustituye con la IP real de tu ESP32
BROKER_PORT = 1883  # Puerto por defecto de MQTT

# El topic al que te quieres suscribir
SUBSCRIBE_TOPIC = "#"  # Se suscribe a todos los tópicos
PUBLISH_TOPIC = "rack0/sens/ambient/temperature"  # El tópico en el que vas a publicar

# Credenciales para la autenticación
USERNAME = "adrian"  # Cambia al usuario que usas
PASSWORD = "librecultivo"  # Cambia a la contraseña que usas

# Tiempo entre publicaciones en segundos
PUBLISH_INTERVAL = 5  # Publicar cada 5 segundos

# Función de callback que se ejecuta cuando el cliente recibe un mensaje
def on_message(client, userdata, msg):
    print(f"Mensaje recibido en el tópico {msg.topic}: {msg.payload.decode()}")

# Función de callback que se ejecuta cuando el cliente se conecta al broker
def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Conexión exitosa al broker MQTT")
        # Suscribirse al tópico
        client.subscribe(SUBSCRIBE_TOPIC)
    else:
        print(f"Error al conectar con el broker: {rc}")

# Función de callback que se ejecuta cuando el cliente se desconecta del broker
def on_disconnect(client, userdata, rc):
    if rc != 0:
        print(f"Desconexión inesperada, código de error: {rc}")
    else:
        print("Desconexión exitosa")

# Crear un nuevo cliente MQTT
client = mqtt.Client()

# Establecer las credenciales de autenticación
client.username_pw_set(USERNAME, PASSWORD)

# Establecer la función de callback de conexión
client.on_connect = on_connect
# Establecer la función de callback para mensajes
client.on_message = on_message
# Establecer la función de callback para desconexión
client.on_disconnect = on_disconnect

# Intentar conectar al broker con reintentos en caso de fallo
def connect_to_broker():
    try:
        client.connect(BROKER_IP, BROKER_PORT, 60)
    except Exception as e:
        print(f"Error al intentar conectar: {e}")
        time.sleep(5)  # Esperar 5 segundos antes de intentar nuevamente
        connect_to_broker()  # Reintentar la conexión

# Conectarse al broker MQTT
connect_to_broker()

# Iniciar el bucle de espera de mensajes
client.loop_start()  # Cambiar a loop_start() para que el cliente siga ejecutándose en paralelo

# Publicar un mensaje periódicamente
n=0
try:
    while True:
        #message = f"¡Hola desde Python! Este es un mensaje periódico. numero {n}"
        message = f"{n}"
        n = n + 1
        client.publish(PUBLISH_TOPIC, message)
        print(f"Publicado en el tópico {PUBLISH_TOPIC}: {message}")
        time.sleep(PUBLISH_INTERVAL)  # Espera antes de publicar el siguiente mensaje
except KeyboardInterrupt:
    print("Interrumpido por el usuario")

# Detener el bucle cuando termine
client.loop_stop()  # Detiene el bucle cuando se interrumpe
