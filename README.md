# Libre  cultivo🌾 
# 🌱 Huerto Urbano de Precisión

Este repositorio contiene el desarrollo completo de un **sistema inteligente de monitoreo y control** para un huerto urbano basado en tecnologías de precisión. El objetivo principal es optimizar el crecimiento de cultivos hidroponónicos mediante el uso de sensores, actuadores y software de automatización.

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
