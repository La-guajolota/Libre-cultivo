import paho.mqtt.client as mqtt
import time
import streamlit as st

# Dirección IP del ESP32 (Broker MQTT)
BROKER_IP = "192.168.1.100"  # Sustituye con la IP de tu ESP32
BROKER_PORT = 1883  # Puerto por defecto de MQTT

# Tópicos de suscripción y publicación
SUBSCRIBE_TOPIC = "#"  # Se suscribe a todos los tópicos
PUBLISH_TOPIC = "mi/topico/test"  # Tópico para publicar mensajes

# Credenciales de autenticación
USERNAME = "adrian"  # Nombre de usuario
PASSWORD = "librecultivo"  # Contraseña

# Tiempo entre publicaciones (en segundos)
PUBLISH_INTERVAL = 5  # Publicar cada 5 segundos

# Función de callback para cuando el cliente recibe un mensaje
def on_message(client, userdata, msg):
    message = msg.payload.decode()  # Decodificar el mensaje
    # Guardar el mensaje en el estado de la sesión de Streamlit
    st.session_state.messages.append(f"{msg.topic}: {message}")

# Función de callback para cuando el cliente se conecta al broker
def on_connect(client, userdata, flags, rc):
    if rc == 0:
        st.write("Conexión exitosa al broker MQTT")
        # Suscribirse al tópico
        client.subscribe(SUBSCRIBE_TOPIC)
    else:
        st.write(f"Error al conectar con el broker: {rc}")

# Función de callback para cuando el cliente se desconecta
def on_disconnect(client, userdata, rc):
    if rc != 0:
        st.write(f"Desconexión inesperada, código de error: {rc}")
    else:
        st.write("Desconexión exitosa")

# Inicialización del estado de la sesión de Streamlit
if "messages" not in st.session_state:
    st.session_state.messages = []  # Iniciar la lista de mensajes si no está definida

# Crear el cliente MQTT
client = mqtt.Client()

# Establecer las credenciales
client.username_pw_set(USERNAME, PASSWORD)

# Asignar las funciones de callback
client.on_connect = on_connect
client.on_message = on_message
client.on_disconnect = on_disconnect

# Conectar al broker MQTT
def connect_to_broker():
    try:
        client.connect(BROKER_IP, BROKER_PORT, 60)
        client.loop_start()  # Comienza el loop para recibir mensajes en segundo plano
    except Exception as e:
        st.write(f"Error al intentar conectar: {e}")
        time.sleep(5)  # Esperar 5 segundos antes de intentar nuevamente
        connect_to_broker()  # Reintentar la conexión

# Publicar mensajes periódicamente
def publish_message(message):
    client.publish(PUBLISH_TOPIC, message)  # Publicar el mensaje
    st.write(f"Publicado en el tópico {PUBLISH_TOPIC}: {message}")  # Mostrar el mensaje publicado

# Ejecutar la conexión y publicación
connect_to_broker()

# Interfaz de Streamlit
st.title("Monitor de MQTT con ESP32")

# Mostrar los mensajes recibidos
if st.session_state.messages:
    st.write("Mensajes recibidos:")
    for message in st.session_state.messages:
        st.write(message)
else:
    st.write("No se han recibido mensajes aún.")

# Formulario para enviar un mensaje manualmente
message_input = st.text_input("Escribe un mensaje para publicar:", "")
if st.button("Publicar mensaje"):
    if message_input:
        publish_message(message_input)
        st.session_state.messages.append(f"Publicado en {PUBLISH_TOPIC}: {message_input}")
    else:
        st.write("Por favor, ingresa un mensaje para publicar.")

# Publicar un mensaje periódicamente
try:
    while True:
        message = "¡Hola desde Python! Este es un mensaje periódico."
        publish_message(message)  # Publica un mensaje cada 5 segundos
        time.sleep(PUBLISH_INTERVAL)
except KeyboardInterrupt:
    st.write("Interrumpido por el usuario")

# Detener el loop cuando se termina
client.loop_stop()
