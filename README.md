# **Libre Cultivo 🌾**  
## 🌱 **Huerto Urbano de Precisión**  

Este repositorio contiene el desarrollo completo de un **sistema inteligente de monitoreo y control** para un huerto urbano basado en tecnologías de precisión.  
El objetivo principal es optimizar el crecimiento de cultivos hidropónicos mediante sensores, actuadores y software de automatización.  

---

## 📺 **Automatización Básica: Interfaz de Usuario con Node-Red y Registro de Datos en Google Sheets para Cultivos Urbanos**  

### **Descripción**  
¡Bienvenidos a Libre Cultivo! 🌱 En este proyecto exploramos cómo integrar una interfaz de usuario básica con **Node-Red** y un sistema de registro histórico en **Google Sheets** para un cultivo urbano automatizado.  

Podrás controlar actuadores esenciales como:  
- **Luces LED de crecimiento**  
- **Bombas de agua**  
- **Humidificadores**  
- **Ventiladores**  

Todo desde una interfaz gráfica intuitiva. Además, almacenamos datos históricos en Google Sheets, lo que permite un monitoreo remoto de variables clave sin costos adicionales, gracias a Google Cloud.  

Este proyecto es una **prueba de concepto** que sienta las bases para un futuro cultivo urbano inteligente, eficiente y accesible para todos. Aprende con nosotros cómo la tecnología puede transformar la agricultura urbana.  

🔗 **[Ver playlist del funcionamiento del prototipo en YouTube](https://youtu.be/g-AVNDlKydo?si=g54WuJxGeLFtIjIJ)**  

---

### **Estructura del Sistema**  

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

---

## 🚀 **Funcionalidades**  

- **Monitoreo en tiempo real** de parámetros clave:
  - pH del agua
  - Temperatura del agua y del ambiente
  - Conductividad eléctrica (EC) *(AÚN NO AGREGADO)*
  - Sólidos disueltos totales (TDS) *(AÚN NO AGREGADO)*
  - Humedad relativa
- **Control automatizado** de:
  - Bombas de agua
  - Dosificadores de nutrientes 
  - Sistemas de iluminación LED para crecimiento
  - Ventiladores y humidificador
- **Interfaz gráfica** en Node-Red para visualización y configuración del sistema.
- **Almacenamiento de históricos**: Datos registrados en Google Sheets.
- **Control móvil**: Comunicación con el cultivo desde la aplicación gratuita MQTT Cliente.  

---

## 🛠️ **Tecnologías Utilizadas**  

### **Hardware**
- **Sensores**: pH, DHT22, BMP280
- **Microcontroladores**: ESP32 / STM32
- **Actuadores**: Bombas peristálticas, relés, ventiladores
- **Circuitos**: Diseño y fabricación de PCBs personalizados  

### **Software**
- **Lado embebido**:
  - Lenguajes: C/C++
  - Frameworks: FreeRTOS, PlatformIO, Arduino, STM32, LVGL
- **Interfaz de usuario**:
  - Lenguajes: Python
  - Frameworks: Node-RED, MQTT
- **Almacenamiento**: Google Sheets para registro histórico  

---

## 📁 **Estructura del Repositorio**  

```plaintext
.
├── hardware/            # Esquemas y diseño de PCBs
├── embedded/            # Código para microcontroladores
├── dashboard/           # Aplicación Node-Red
├── data/                # Scripts para Google Sheets
├── docs/                # Documentación adicional
└── README.md            # Descripción del proyecto
