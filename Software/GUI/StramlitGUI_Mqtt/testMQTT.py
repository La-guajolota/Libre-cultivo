import streamlit as st
from paho.mqtt import client as mqtt

# Callback para la conexión exitosa
def on_connect(client, userdata, flags, rc):
    if rc == 0:
        st.success("Conectado exitosamente al broker MQTT")
    else:
        st.error("Error al conectar, código de retorno: " + str(rc))

# Callback para la desconexión
def on_disconnect(client, userdata, flags, rc):
    st.write("Desconectado del broker")

# Callback para la publicación
def on_publish(client, userdata, mid):
    st.write(f"Mensaje publicado con ID: {mid}")

# Callback para el mensaje recibido
def on_message(client, userdata, msg):
    # Agregar el mensaje recibido al estado de sesión de Streamlit
    if 'messages' not in st.session_state:
        st.session_state.messages = []
    st.session_state.messages.append(f"{msg.topic}: {str(msg.payload.decode())}")
    st.rerun()  # Esto actualiza la interfaz para mostrar el nuevo mensaje

# Función para crear el cliente MQTT
def get_mqtt_client(broker, port, username, password):
    client = mqtt.Client()
    client.username_pw_set(username, password)
    client.on_connect = on_connect
    client.on_publish = on_publish
    client.on_message = on_message
    return client

# Interfaz de Streamlit
st.title("MQTT Client - Conexión a ESP32")

col1, col2, col3 = st.columns(3)

# Información del broker y cliente
with col1:
    broker = st.text_input("Broker MQTT (IP ESP32)", "192.168.1.100")
    port = st.text_input("Puerto MQTT", "1883")
    client_id = st.text_input("Client ID", "ESP32_Client")
    username = st.text_input("Usuario", "juan")
    password = st.text_input("Contraseña", "pepexd", type="password")

    if st.button('Conectar'):
        # Conectar al broker con las credenciales
        try:
            client = get_mqtt_client(broker, int(port), username, password)
            client.connect(broker, int(port), 60)
            client.loop_start()
            st.success(f"Conectado al broker {broker}:{port} con éxito")
        except Exception as e:
            st.error(f"No se pudo conectar: {e}")

# Publicación de mensaje
with col2:
    publish_topic = st.text_input("Topic para publicar")
    message = st.text_input("Mensaje para publicar")
    if st.button('Publicar'):
        try:
            client = get_mqtt_client(broker, int(port), username, password)
            client.connect(broker, int(port), 60)
            client.publish(publish_topic, message)
            st.write(f"Mensaje publicado en {publish_topic}: {message}")
        except Exception as e:
            st.error(f"No se pudo publicar el mensaje: {e}")

# Suscripción a un topic
with col3:
    subscribe_topic = st.text_input("Topic para suscribirse")
    if st.button('Suscribirse'):
        try:
            client = get_mqtt_client(broker, int(port), username, password)
            client.connect(broker, int(port), 60)
            client.subscribe(subscribe_topic)
            client.loop_start()
            st.write(f"Suscrito a {subscribe_topic}")
        except Exception as e:
            st.error(f"No se pudo suscribir: {e}")

# Mostrar los mensajes recibidos
if 'messages' in st.session_state:
    st.subheader("Mensajes Recibidos:")
    for message in st.session_state.messages:
        st.write(message)
