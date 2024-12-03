# **Libre Cultivo 🌾**  
## 🌱 **Huerto Urbano de Precisión**  

Este repositorio contiene el desarrollo completo de un **sistema inteligente de monitoreo y control** para un huerto urbano basado en tecnologías de precisión.  
El objetivo principal es optimizar el crecimiento de cultivos hidropónicos mediante sensores, actuadores y software de automatización.  

---

## 📺 **Audiovisuales del funcionamiento**  

### **Descripción**  
¡Bienvenidos a Libre Cultivo! 🌱  

Esta playlist documenta el proceso y funcionamiento del sistema automatizado desarrollado para un huerto urbano de precisión. En los videos se exploran las siguientes áreas clave:  
- **Monitoreo en tiempo real** de parámetros como pH, temperatura, y humedad.  
- **Control automatizado** de actuadores esenciales: bombas de agua, luces LED, ventiladores y humidificadores.  
- **Interfaz gráfica** con Node-Red para una experiencia de usuario intuitiva.  
- **Almacenamiento de datos históricos** en Google Sheets, con acceso remoto y sin costo adicional.  

El contenido refleja los avances y resultados obtenidos, así como los retos superados durante la implementación de esta prueba de concepto. Es un punto de partida para futuras iteraciones hacia un sistema completamente inteligente, eficiente y accesible.  

🔗 **[Ver playlist del funcionamiento del prototipo en YouTube](https://www.youtube.com/watch?v=WTyhxuf8L80&t=177s)**  

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
- **Sensores**: DS1820, pH módulo, DHT22, BMP280
- **Microcontroladores**: ESP32 / STM32
- **Actuadores**: Bombas peristálticas, relés, ventiladores
- **Circuitos**: Diseño y fabricación de PCBs personalizados
- **PCB**: Kicad  

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
├── hardware/            
│   ├── Actuadores/       # Esquemas eléctricos relacionados con actuadores (bombas, ventiladores, LEDs)
│   ├── Rack0/            # Diseños de PCBs y conexiones para el rack principal
│   ├── Sensores/         # Esquemas eléctricos y configuraciones de sensores utilizados
│   └── Simulaciones/     # Simulaciones de circuitos y pruebas virtuales
├── software/            
│   ├── blackpill_sensors/  # Código específico para sensores en la Blackpill
│   ├── bluepill_actuadores/ # Código para control de actuadores con la Bluepill
│   ├── Ejemplo_externos/   # Ejemplos de integración con librerías externas
│   ├── espressif/          # Código relacionado con el ESP32
│   ├── GUI/                # Interfaz gráfica para monitoreo y control
│   ├── mqttESP32/          # Implementación de MQTT para el ESP32
│   └── Python_desktop/     # Aplicaciones de escritorio en Python para control y monitoreo
├── LICENSE/               # Licencia del proyecto
└── README.md              # Descripción del proyecto


