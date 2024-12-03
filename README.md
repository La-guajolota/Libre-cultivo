# Libre  cultivo🌾 
# 🌱 Huerto Urbano de Precisión

Este repositorio contiene el desarrollo completo de un **sistema inteligente de monitoreo y control** para un huerto urbano basado en tecnologías de precisión. El objetivo principal es optimizar el crecimiento de cultivos hidroponónicos mediante el uso de sensores, actuadores y software de automatización.

### Estructura del Sistema

<div align="center">
    <img src="https://github.com/user-attachments/assets/2d077aaa-3d0d-493a-9d68-fc58760e8271" alt="Diagrama eléctrico del sistema" width="600">
</div>

El diagrama eléctrico muestra la configuración de los componentes principales del sistema:  

- **Actuadores (6 en total):**
  - **Bombas (3):**  
    - Dos bombas dosifican nutrientes al tanque con la solución general.  
    - La tercera bomba extrae esta solución para regar las plantas.  
  - **Ventiladores (2):** Regulan la temperatura dentro del rack.  
  - **Luces LED (1):** Proveen iluminación adecuada para el crecimiento de las plantas.

- **Sensores (3 en total):**  
  - **BME680:** Monitorea la temperatura y humedad en el ambiente del rack.  
  - **Sensor de temperatura:** Mide la temperatura del líquido en el tanque de la solución general.  
  - **Sensor de pH:** Verifica el nivel de pH de la solución en el tanque.

Esta configuración permite un control preciso del microclima y los parámetros hídricos necesarios para el óptimo desarrollo de las plantas.

## 🚀 Funcionalidades

- **Monitoreo en tiempo real** de parámetros clave:
  - pH del agua
  - Temperatura del agua y del ambiente
  - Conductividad eléctrica (EC) (AÚN NO AGREGADO)
  - Sólidos disueltos totales (TDS) (AÚN NO AGREGADO)
  - Humedad relativa
- **Control automatizado** de:
  - Bomba de agua
  - Dosificadores de nutrientes 
  - Sistemas de iluminación LED para crecimiento
  - Ventiladores y humidificador
- **Interfaz gráfica** en Node-Red para visualización y configuración del sistema.
- **Almacenamiento de histporicos** en google sheets almacenamos las lecturas.
- **Control mobil** desde la aplicación gratuita (MQTT cliente) nos comunicamos con el cultivo. 

## 🛠️ Tecnologías Utilizadas

### Hardware
- **Sensores**: pH, DHT22, BMP280
- **Microcontroladores**: ESP32 / STM32
- **Actuadores**: Bombas peristálticas, relés, ventiladores
- **Circuitos**: Diseño y fabricación de PCBs personalizado 

### Software
- **Lado embebido**:
  - Lenguajes: C/C++
  - Frameworks: FreeRTOS, Platformio, Arduino, STM32, LVGL
- **Interfaz de usuario**:
  - Lenguajes: Python
  - Frameworks: Streamlit, MQTT, Node-RED

## 📁 Estructura del Repositorio

```plaintext
.
├── hardware/            # Esquemas y diseño de PCBs
├── embedded/            # Código para microcontroladores
├── dashboard/           # Aplicación Streamlit
├── models/              # Redes neuronales y scripts de ML
├── docs/                # Documentación adicional
└── README.md            # Descripción del proyecto
