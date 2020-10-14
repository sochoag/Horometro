#include <LittleFS.h>
#include <ArduinoJson.h>
#include <variables.h>

#ifndef FileSystem
#define FileSystem
// Funciones
void FSRead(String);
void FSWrite(String);
void setupFS();

void setupFS() 
{
  //clean FS, for testing
  //LittleFS.format();

  //Lectura archivo de configuración
  Serial.println("Configurando FS...");
  LittleFS.begin();
  FSRead("config.json");
}

void FSRead(String archivo)
{
    if (LittleFS.exists(archivo))
    {
      //file exists, reading and loading
      Serial.println("Leyendo "+archivo);
      File configFile = LittleFS.open(archivo, "r");
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
          Serial.println("\nParsed json");
          strcpy(equipo, json["equipo"]);
          strcpy(codigo, json["codigo"]);
        }

        else
        {
          Serial.println("Error al abrir el archivo" + archivo);
        }

      }
    }
}

void FSWrite(String archivo)
{
    Serial.println("Guardando configuraciones");
    DynamicJsonBuffer jsonBuffer;
    JsonObject& json = jsonBuffer.createObject();
    json["equipo"] = equipo;
    json["codigo"] = codigo;

    File configFile = LittleFS.open(archivo, "w");
    if (!configFile)
    {
      Serial.println("Error al guardar las configuraciones");
    }

    json.prettyPrintTo(Serial);
    json.printTo(configFile);
    configFile.close();
}

#endif