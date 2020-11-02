#include <WiFiManager.h>
#include <variables.h>
#include <FileSystem.h>
#include <horometro.h>

#ifndef connectWifi
#define connectWifi
// Variables de libreria
bool shouldSaveConfig = false; //flag for saving data
int attempToSkip = 2;
// Instancias
WiFiManager wm;
// Funciones
void setupAP();
void saveConfigCallback();
void configModeCallback(WiFiManager *myWiFiManager);
void resetAP();
//void reconexion();

void setupAP()
{
  SDRead("attemps.log",attemps,valAttemps,nAtemps);
  valAttemps[0] = String(valAttemps[0].toInt() + 1);
  SDWrite("attemps.log",attemps,valAttemps,nAtemps);
  Serial.println("════════════════════════════════════════════════════════════");
  Serial.println("Configurando Wifi \t Intento:"+valAttemps[0]);
  escribir_oled("Configurando Wifi...",10);
  wm.setAPCallback(configModeCallback);
  wm.setSaveConfigCallback(saveConfigCallback);
  wm.setConfigPortalTimeout(15); // Tiempo para saltar AP en segundos
  wm.setDebugOutput(false);

  // MQTT Server
  WiFiManagerParameter custom_mqtt_server("MQTT Server", "Servidor MQTT", "techsolutionsec.ga", 30);
  WiFiManagerParameter custom_mqtt_port("MQTT Puerto", "Puerto MQTT", "1883", 5);
  WiFiManagerParameter custom_mqtt_user("MQTT User", "Usuario MQTT", "HoroIta01", 30);
  WiFiManagerParameter custom_mqtt_password("MQTT Password", "Pass MQTT", "", 30, "type= password");

  // Custon Parametros
  WiFiManagerParameter custom_equipo("eq", "Equipo", "", 30);
  WiFiManagerParameter custom_codigo("code", "Codigo", "", 30);

  wm.addParameter(&custom_mqtt_server);
  wm.addParameter(&custom_mqtt_port);
  wm.addParameter(&custom_mqtt_user);
  wm.addParameter(&custom_mqtt_password);

  wm.addParameter(&custom_equipo);
  wm.addParameter(&custom_codigo);
  if(valAttemps[0].toInt() <= attempToSkip)
  {
    if (!wm.autoConnect("Horometro", "12345678"))
    {
      Serial.println("No se pudo conectar");
      delay(3000);
      ESP.restart();
      delay(5000);
    }
    Serial.println("Wifi Configurado");
    escribir_oled("Wifi Configurado",10);
    Serial.println("────────────────────────────────────────────────────────────");
    Serial.println("Conectado a:" + WiFi.SSID());
    Serial.println("IP Local");
    Serial.println(WiFi.localIP());
    Serial.println(WiFi.gatewayIP());
    Serial.println(WiFi.subnetMask());
    Serial.println("════════════════════════════════════════════════════════════");

    if (shouldSaveConfig)
    {
      valoresConfig[0] = custom_mqtt_server.getValue();
      valoresConfig[1] = custom_mqtt_port.getValue();
      valoresConfig[2] = custom_mqtt_user.getValue();
      valoresConfig[3] = custom_mqtt_password.getValue();

      valoresConfig[4] = custom_equipo.getValue(); // Se aloja el valor introducido en el AP
      valoresConfig[5] = custom_codigo.getValue(); // Se aloja el valor introducido en el AP

      FSWrite("/config.json", configuraciones, valoresConfig, n);
      //end save
      shouldSaveConfig = false;
    }
    banderaWifi = true;
  }
  else
  {
    escribir_oled("Sistema Offline",10);
    banderaWifi = false;
    SD.remove("attemps.log");
  }
}

void configModeCallback(WiFiManager *myWiFiManager)
{
  Serial.println("────────────────────────────────────────────────────────────");
  Serial.println("Por favor ingrese en el AP...");
  escribir_oled("Configure AP",10);
  Serial.println("────────────────────────────────────────────────────────────");
}

void resetAP()
{
  Serial.println("════════════════════════════════════════════════════════════");
  Serial.println("Reseteando");
  escribir_oled("Reseteando",10);
  Serial.println("────────────────────────────────────────────────────────────");
  Serial.println("Limpiando Registro de Horometro");
  escribir_oled("Limpiando Registro",10);
  resetHoro();
  Serial.println("────────────────────────────────────────────────────────────");
  Serial.println("Limpiando FS");
  escribir_oled("Limpiando FS",10);
  LittleFS.format();
  Serial.println("────────────────────────────────────────────────────────────");
  delay(1000);
  Serial.println("Limpiando Wifi Settings");
  escribir_oled("Limpiando Wifi Settings",10);
  wm.resetSettings();
  Serial.println("────────────────────────────────────────────────────────────");
  delay(1000);
  Serial.println("Reiniciando Dispositivo");
  escribir_oled("Reiniciando Dispositivo",10);
  Serial.println("════════════════════════════════════════════════════════════");
  delay(3000);
  ESP.restart();
  delay(1000);
}

void saveConfigCallback()
{
  Serial.println("Should save config");
  shouldSaveConfig = true;
}

#endif