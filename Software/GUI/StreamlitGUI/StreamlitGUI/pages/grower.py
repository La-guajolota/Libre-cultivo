import streamlit as st
import pandas as pd
import numpy as np
import cv2
from time import sleep

# Simulación del menú y redirección si no está logueado
# menu_with_redirect()

# Verificación de rol del usuario
if "role" in st.session_state and st.session_state.role not in ["Grower"]:
    st.warning("You do not have permission to view this page.")
    st.stop()

# Inicialización de estados de los actuadores si no están en session_state
if 'pump_states' not in st.session_state:
    st.session_state.pump_states = [False, False, False]  # Estado inicial de las bombas
if 'humidifier_states' not in st.session_state:
    st.session_state.humidifier_states = [False, False, False]  # Estado inicial de los humidificadores

# Título de la aplicación
st.title(":green[Real-time] sensors and :blue[fully-controlled] system :scientist: :material/analytics:")
st.markdown("**As a grower, you can monitor real-time sensor measurements and take action using actuators.**")

# Valores iniciales de los sensores
waterTemp = 25
waterTemp_delta = 2

# Disposición en columnas para mostrar los valores de los sensores
col1_water, col2_amb, col3_actuators = st.columns(3)

# Columna para métricas de la solución de agua
with col1_water:
    st.title("*Solution*", anchor=False)
    st.metric(label="Temperature", value=f"{waterTemp} °C", delta=f"{waterTemp_delta} °C")
    st.metric(label="pH", value=f"{waterTemp}", delta=f"{waterTemp_delta}")
    st.metric(label="EC", value=f"{waterTemp} mS/cm", delta=f"{waterTemp_delta} mS/cm")
    st.metric(label="TDS", value=f"{waterTemp} ppm", delta=f"{waterTemp_delta} ppm")

# Columna para métricas del ecosistema
with col2_amb:
    st.title("*Ecosystem*", anchor=False)
    st.metric(label="Temperature", value=f"{waterTemp} °C", delta=f"{waterTemp_delta} °C")
    st.metric(label="Humidity", value=f"{waterTemp} %", delta=f"{waterTemp_delta}")
    st.metric(label="Pressure", value=f"{waterTemp} mbar", delta=f"{waterTemp_delta} mbar")
    st.metric(label="Electric Power", value=f"{1} W", delta=f"{2} W")

# Columna para actuadores y controles

with col3_actuators:
    st.title("*Actuators*", anchor=False)
    
    # Lista desplegable para seleccionar LED y ajustar la intensidad
    led_options = ["Grow LED 1", "Grow LED 2", "Grow LED 3"]
    selected_led = st.selectbox("Select Grow LED to control:", led_options)
    led_intensity = st.slider(f"Set intensity for {selected_led}:", min_value=0, max_value=100)
    #? Actualizar valores al sistema física
    print(f"{selected_led}: {led_intensity}")

    # Lista desplegable para seleccionar bombas dosificadoras y alternar su estado
    pump_options = ["Dosing Pump ⚙️1", "Dosing Pump ⚙️2", "Dosing Pump ⚙️3"]
    selected_pump = st.selectbox("Select Dosing Pump to control:", pump_options)
    # Índice de la bomba seleccionada
    pump_index = pump_options.index(selected_pump)
    # Texto del botón según el estado de la bomba
    pump_button_label = f"{'Deactivate' if st.session_state.pump_states[pump_index] else 'Activate'} {selected_pump}"

    # Cambiar el estado de la bomba al presionar el botón (toggle)
    if st.button(pump_button_label):
        st.session_state.pump_states[pump_index] = not st.session_state.pump_states[pump_index]  # Cambia el estado

    # Lista desplegable para seleccionar humidificadores y alternar su estado
    humidifier_options = ["Humidifier 💨1", "Humidifier 💨2", "Humidifier 💨3"]
    selected_humidifier = st.selectbox("Select Humidifier to control:", humidifier_options)

    # Índice del humidificador seleccionado
    humidifier_index = humidifier_options.index(selected_humidifier)

    # Texto del botón según el estado del humidificador
    humidifier_button_label = f"{'Deactivate' if st.session_state.humidifier_states[humidifier_index] else 'Activate'} {selected_humidifier}"

    # Cambiar el estado del humidificador al presionar el botón (toggle)
    if st.button(humidifier_button_label):
        st.session_state.humidifier_states[humidifier_index] = not st.session_state.humidifier_states[humidifier_index]  # Cambia el estado

# Selector de gráficos
parameters_list = ["Water temperature", "EC", "TDS", "pH", "Environment temperature", "Pressure", "Humidity", "Electric power"]
parameter_selected = st.selectbox("Select a parameter to view its graph:", parameters_list)

# Generación de datos aleatorios para el gráfico
chart_data = pd.DataFrame(np.random.randn(100, 3), columns=["col1", "col2", "col3"])
chart_data["col1"] = pd.Series(range(len(chart_data)))  # Simulación de tiempo en los datos

# Mostrar el gráfico basado en el parámetro seleccionado
st.line_chart(chart_data.set_index("col1"), use_container_width=True)

# Captura de video en tiempo real
st.title("Real-time Video Capture")

# Configuración de captura de video
camera = cv2.VideoCapture(0)  # Ajustar el índice de la cámara según sea necesario

# Botón para iniciar la captura de video
if st.button("Start Capture"):
    st.write("Capturing...")

    # Placeholder para mostrar el video
    placeholder = st.empty()

    # Bucle para capturar imágenes en tiempo real
    while True:
        ret, frame = camera.read()
        if not ret:
            st.write("Error capturing the image.")
            break

        # Convertir la imagen de BGR a RGB
        frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)

        # Mostrar la imagen en Streamlit y actualizar el placeholder
        placeholder.image(frame, channels="RGB")

    # Liberar la cámara al finalizar la captura
    camera.release()
else:
    st.write("Press the button to start capturing.")
