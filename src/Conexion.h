#include <WiFiManager.h>
#include <variables.h>
#include <FileSystem.h>
#include <horometro.h>
// Variables de libreria
bool shouldSaveConfig = false; //flag for saving data
// Instancias
WiFiManager wm;
// Funciones
void setupAP();
void saveConfigCallback();
void resetAP();
void reconexion();

void setupAP()
{

    wm.setSaveConfigCallback(saveConfigCallback);
    wm.setConfigPortalTimeout(30); // Tiempo para saltar AP en segundos
    // Custom Parameters
    WiFiManagerParameter custom_equipo("eq", "Equipo","", 7);
    WiFiManagerParameter custom_codigo("code", "Codigo", "", 7);
    wm.addParameter(&custom_equipo);
    wm.addParameter(&custom_codigo);
    // Configuracion Access Point
    if (!wm.autoConnect("Horometro", "12345678"))
    {
        Serial.println("No se pudo conectar");
        delay(3000);
        ESP.restart();
        delay(5000);
    }
    // always start configportal for a little while
    // wm.setConfigPortalTimeout(60);
    // wm.startConfigPortal("AutoConnectAP","password");
    //if you get here you have connected to the WiFi
    Serial.println("Conectado");
    //read updated parameters
    //save the custom parameters to FS
    if (shouldSaveConfig)
    {
        valoresConfig[0] = custom_equipo.getValue(); // Se aloja el valor introducido en el AP
        valoresConfig[1] = custom_codigo.getValue(); // Se aloja el valor introducido en el AP
        FSWrite("/config.json", configuraciones, valoresConfig, n);
        //end save
        shouldSaveConfig = false;
    }
}

void resetAP()
{
    Serial.println("Limpiando Registro de Horometro");
    resetHoro();
    Serial.println("Limpiando FS");
    //LittleFS.format();
    delay(1000);
    Serial.println("Limpiando Wifi Settings");
    //wm.resetSettings();
    delay(1000);
    Serial.println("Reiniciando");
    ESP.restart();
    delay(1000);
}

void saveConfigCallback()
{
  Serial.println("Should save config");
  shouldSaveConfig = true;
}

void reconexion()
{
    int c_rec = 0;
    Serial.println("Desconexion");
    WiFi.reconnect();
    Serial.print("Tratando de reconectar: ");
    while (!WiFi.isConnected())
    {
      Serial.print("■");
      delay(500);
      c_rec++;
      if (c_rec > 20)
      {
        Serial.println("Imposible reconectar 🡢 REINICIANDO");
        ESP.restart();
        delay(1000);
      }
    }
    Serial.println("");
}