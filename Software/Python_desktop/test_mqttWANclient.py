import paho.mqtt.client as mqtt
import time

# Dirección IP pública o dominio DDNS del router que redirige al ESP32
BROKER_IP = "2806:264:548d:12dc:78dd:7d8f:da7b:b078"  # Sustituye con tu IP pública o dominio DDNS
BROKER_PORT = 1883  # Puerto redirigido al ESP32 para MQTT

# El topic al que te quieres suscribir
SUBSCRIBE_TOPIC = "#"  # Se suscribe a todos los tópicos
PUBLISH_TOPIC = "mi/topico/test"  # El tópico en el que vas a publicar

# Credenciales para la autenticación
USERNAME = "juan"  # Cambia al usuario que usas
PASSWORD = "pepexd"  # Cambia a la contraseña que usas

# Tiempo entre publicaciones en segundos
PUBLISH_INTERVAL = 5  # Publicar cada 5 segundos

# Función de callback que se ejecuta cuando el cliente recibe un mensaje
def on_message(client, userdata, msg):
    print(f"Mensaje recibido en el tópico {msg.topic}: {msg.payload.decode()}")

# Función de callback que se ejecuta cuando el cliente se conecta al broker
def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Conexión exitosa al broker MQTT en WAN")
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

# Conectarse al broker MQTT en la WAN
connect_to_broker()

# Iniciar el bucle de espera de mensajes
client.loop_start()  # Cambiar a loop_start() para que el cliente siga ejecutándose en paralelo

# Publicar un mensaje periódicamente
try:
    while True:
        message = "¡Hola desde Python en WAN! Este es un mensaje periódico."
        client.publish(PUBLISH_TOPIC, message)
        print(f"Publicado en el tópico {PUBLISH_TOPIC}: {message}")
        time.sleep(PUBLISH_INTERVAL)  # Espera antes de publicar el siguiente mensaje
except KeyboardInterrupt:
    print("Interrumpido por el usuario")

# Detener el bucle cuando termine
client.loop_stop()  # Detiene el bucle cuando se interrumpe
