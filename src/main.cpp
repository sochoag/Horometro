// Definiciones
#include <Arduino.h>
#include <variables.h>
// #include <FS.h>
#include <LittleFS.h>    // this needs to be first, or it all crashes and burns...
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson
#define button 0

//Variables
bool shouldSaveConfig = false; //flag for saving data

// Funciones
void saveConfigCallback ();
void setupFS();

// Instanciaciones
WiFiManager wm;

void setup()
{
  Serial.begin(115200);
  Serial.println();
  pinMode(button,INPUT_PULLUP);
  setupFS();
  wm.setSaveConfigCallback(saveConfigCallback);
  wm.setConfigPortalTimeout(20);
  // Custom Parameters
  WiFiManagerParameter custom_equipo("eq", "Equipo", equipo, 7);
  WiFiManagerParameter custom_codigo("code", "Codigo", codigo, 7);
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
  strcpy(equipo, custom_equipo.getValue());
  strcpy(codigo, custom_codigo.getValue());

  //save the custom parameters to FS
  if (shouldSaveConfig)
  {
    Serial.println("Guardando configuraciones");
    DynamicJsonBuffer jsonBuffer;
    JsonObject& json = jsonBuffer.createObject();
    json["equipo"] = equipo;
    json["codigo"] = codigo;

    File configFile = LittleFS.open("/config.json", "w");
    if (!configFile)
    {
      Serial.println("Error al guardar las configuraciones");
    }

    json.prettyPrintTo(Serial);
    json.printTo(configFile);
    configFile.close();
    //end save
    shouldSaveConfig = false;
  }

  Serial.println("IP Local");
  Serial.println(WiFi.localIP());
  Serial.println(WiFi.gatewayIP());
  Serial.println(WiFi.subnetMask());
}

void loop()
{
  double t = millis();

  if (t - t1 > 5000)
  {
    t1 = t;
    Serial.println("Equipo: " + String(equipo) + "\tCodigo: " + String(codigo)); // put your main code here, to run repeatedly:
  }

  if(!WiFi.isConnected() )
  {
    int c_rec = 0;
    Serial.println("Deconexion");
    WiFi.reconnect();
    Serial.print("Tratando de reconectar: ");
    while(!WiFi.isConnected())
    {
      Serial.print(".");
      delay(500);
      c_rec ++;
      if(c_rec>20)
      {
        ESP.restart();
        delay(1000);
      }
    }
    Serial.println("");
  }
  

  if(!digitalRead(0))
  {
    Serial.println("Limpiando FS");
    LittleFS.format();
    delay(1000);
    Serial.println("Limpiando Wifi Settings");
    wm.resetSettings();
    delay(1000);
    Serial.println("Reiniciando");
    ESP.restart();
    delay(1000);
  } 
}

void setupFS() 
{
  //clean FS, for testing
  //LittleFS.format();

  //read configuration from FS json
  Serial.println("mounting FS...");

  if (LittleFS.begin())
  {
    Serial.println("mounted file system");
    if (LittleFS.exists("/config.json"))
    {
      //file exists, reading and loading
      Serial.println("reading config file");
      File configFile = LittleFS.open("/config.json", "r");
      if (configFile)
      {
        Serial.println("opened config file");
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);
        configFile.readBytes(buf.get(), size);
        DynamicJsonBuffer jsonBuffer;
        JsonObject& json = jsonBuffer.parseObject(buf.get());
        json.printTo(Serial);

        if (json.success())
        {
          Serial.println("\nparsed json");
          strcpy(equipo, json["equipo"]);
          strcpy(codigo, json["codigo"]);
        }

        else
        {
          Serial.println("failed to load json config");
        }

      }
    }
  }
  else
  {
    Serial.println("failed to mount FS");
  } //end read
}

void saveConfigCallback () 
{
  Serial.println("Should save config");
  shouldSaveConfig = true;
}