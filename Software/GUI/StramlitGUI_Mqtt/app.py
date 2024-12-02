import threading
import streamlit as st
import paho.mqtt.client as mqtt

# Lock to synchronize the MQTT thread and Streamlit
lock = threading.Lock()

# MQTT Configuración
BROKER_IP = "192.168.1.100"
BROKER_PORT = 1883
USERNAME = "adrian"
PASSWORD = "librecultivo"

SENSOR_TOPICS = [
    "rack0/sens/water/temperature",
    "rack0/sens/ambient/temperature",
    "rack0/sens/ambient/humidity",
    "rack0/sens/water/ph",
    "rack0/sens/water/tds",
    "rack0/sens/water/ec"
]

ACTUATOR_TOPICS = [
    "rack0/actu/watering0",
    "rack0/actu/dose_pump0",
    "rack0/actu/dose_pump1",
    "rack0/actu/dose_pump2",
    "rack0/actu/light/control",
    "rack0/actu/fan/control0",
    "rack0/actu/fan/control1",
    "rack0/actu/humidifier"
]

# Inicialización de estado de Streamlit
if "sensor_data" not in st.session_state:
    st.session_state["sensor_data"] = {topic: "N/A" for topic in SENSOR_TOPICS}

if "actuator_values" not in st.session_state:
    st.session_state["actuator_values"] = {topic: 0 for topic in ACTUATOR_TOPICS}

# Diccionario de datos de sensores que se actualizará desde el hilo MQTT
sensor_data_dict = {topic: "N/A" for topic in SENSOR_TOPICS}

# Callbacks de MQTT
def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Conectado exitosamente al broker MQTT")
        # Suscribirse a los tópicos de sensores
        for topic in SENSOR_TOPICS:
            client.subscribe(topic)
    else:
        print(f"Error al conectar con el broker, código: {rc}")

def on_message(client, userdata, msg):
    # Bloquear el acceso a la actualización de datos para evitar conflictos
    with lock:
        # Actualizar el diccionario con los datos recibidos
        if msg.topic in sensor_data_dict:
            sensor_data_dict[msg.topic] = msg.payload.decode()
            print(f"Mensaje recibido: {msg.topic} -> {msg.payload.decode()}")
        # Actualizar el estado de Streamlit
        st.session_state["sensor_data"] = sensor_data_dict
        st.rerun()  # Forzar la actualización de la UI después de recibir el mensaje


# Función para manejar el hilo MQTT
def mqtt_thread():
    client = mqtt.Client()
    client.username_pw_set(USERNAME, PASSWORD)
    client.on_connect = on_connect
    client.on_message = on_message

    # Conectarse al broker
    client.connect(BROKER_IP, BROKER_PORT, 60)

    # Bucle de MQTT
    client.loop_forever()

# Iniciar el cliente MQTT en un hilo separado
if "mqtt_thread" not in st.session_state:
    threading.Thread(target=mqtt_thread, daemon=True).start()
    st.session_state["mqtt_thread"] = True

# Interfaz de Streamlit
st.title("Dashboard de Hidroponía - MQTT")

# Mostrar los datos de sensores
st.header("Datos de Sensores")
for topic in SENSOR_TOPICS:
    sensor_value = st.session_state["sensor_data"].get(topic, "N/A")
    st.text(f"{topic}: {sensor_value}")

# Control de actuadores
st.header("Control de Actuadores")
for topic in ACTUATOR_TOPICS:
    value = st.slider(f"{topic}", 0, 100, st.session_state["actuator_values"][topic])
    st.session_state["actuator_values"][topic] = value

# Botón para enviar comandos a los actuadores
if st.button("Enviar comandos a actuadores"):
    client = mqtt.Client()
    client.username_pw_set(USERNAME, PASSWORD)
    client.connect(BROKER_IP, BROKER_PORT, 60)

    for topic, value in st.session_state["actuator_values"].items():
        client.publish(topic, value)
        print(f"Enviado: {topic} -> {value}")

    client.disconnect()
