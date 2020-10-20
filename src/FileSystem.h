#include <LittleFS.h>
#include <ArduinoJson.h>
#include <variables.h>

#ifndef FileSystem
#define FileSystem
// Funciones
void FSRead(String archivo, String header[], String values[], int sizeA);
void FSWrite(String archivo, String header[], String values[], int sizeA);
void setupFS();

void setupFS() 
{
  //clean FS, for testing
  //LittleFS.format();

  //Lectura archivo de configuración
  Serial.println("Configurando FS...");
  LittleFS.begin();
  FSRead("/config.json", configuraciones, valoresConfig, n);
  FSRead("/horometro.json", horometro,valoresHorometro, nH);
  FSRead("/prueba.json",prueba,valoresPrueba,nP);
}

void FSRead(String archivo, String header[], String values[],int sizeA)
{
    if (LittleFS.exists(archivo))
    {
      //file exists, reading and loading
      Serial.println("Leyendo "+archivo);
      File configFile = LittleFS.open(archivo, "r");
      if (configFile)
      {
        Serial.println("Archivo "+archivo+" abierto");
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
          Serial.print("{");
          for(int i=0; i<sizeA ; i++)
          {
            //String aux = json["sensor"].as<String>();
            values[i] =  json[header[i]].as<String>();
            Serial.print("\""+header[i] + "\":\"" + values[i]+"\",");
          }
          Serial.println("}");
        }

        else
        {
          Serial.println("Error al abrir el archivo" + archivo);
        }

      }
    }
}

void FSWrite(String archivo, String header[], String values[],int sizeA)
{
    // Serial.println(sizeA);
    //Serial.println("Guardando configuraciones");
    DynamicJsonBuffer jsonBuffer;
    JsonObject& json = jsonBuffer.createObject();
    for(int i=0; i<sizeA ; i++)
    {
      //Serial.println(header[i] + " ···· "+ values[i]);
      json[header[i]] = values[i];
    }
    File configFile = LittleFS.open(archivo, "w");
    if (!configFile)
    {
      //Serial.println("Error al guardar las configuraciones");
    }
    //Serial.println("Guardado en"+archivo);
    //json.prettyPrintTo(Serial);
    json.printTo(configFile);
    configFile.close();
    //Serial.println("Guardado en"+archivo);
}

#endif