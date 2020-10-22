#include <LittleFS.h>
#include <ArduinoJson.h>
#include <variables.h>
#include <OLED.h>

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
  Serial.println("════════════════════════════════════════════════════════════");
  escribir_oled("Configurando FS...",10);
  Serial.println("Configurando FS...");
  LittleFS.begin();
  FSRead("/config.json", configuraciones, valoresConfig, n);
  escribir_oled("FS configurado",10);
  Serial.println("FS configurado");
  Serial.println("════════════════════════════════════════════════════════════");
  //FSRead("/horometro.json", horometro,valoresHorometro, nH);
  //FSRead("/prueba.json",prueba,valoresPrueba,nP);
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

        DynamicJsonDocument doc(1024);
        DeserializationError error = deserializeJson(doc, buf.get());

        serializeJsonPretty(doc, Serial);
        Serial.println("");

        if (!error)
        {
          for(int i=0; i<sizeA ; i++)
          {
            //String aux = json["sensor"].as<String>();
            values[i] =  doc[header[i]].as<String>();
          }
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
    DynamicJsonDocument doc(1024);

    for(int i=0; i<sizeA ; i++)
    {
      //Serial.println(header[i] + " ···· "+ values[i]);
      doc[header[i]] = values[i];
    }

    File configFile = LittleFS.open(archivo, "w");
    if (!configFile)
    {
      //Serial.println("Error al guardar las configuraciones");
    }
    //Serial.println("Guardado en"+archivo);
    //json.prettyPrintTo(Serial);
    serializeJson(doc, configFile);
    serializeJsonPretty(doc, Serial);
    Serial.println("");
    configFile.close();
    //Serial.println("Guardado en"+archivo);
}

#endif