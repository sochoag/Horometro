#include <ArduinoJson.h>
#include <SPI.h>
#include <SD.h>
#include <variables.h>

#ifndef SDFileSystem
#define SDFileSystem

const int chipSelect = D8;

void SDRead(String archivo, String header[], String values[], int sizeA);
void SDWrite(String archivo, String header[], String values[], int sizeA);

void setupSD()
{
    if (!SD.begin(chipSelect))
    {
        Serial.println("Iniciando SD");
        while (1);
    }

    if (!SD.exists("backup.log"))
    {
        Serial.println("Ejecucion del programa normal");
        banderaBackup = false;
    }
    else
    {
        banderaBackup = true;
        Serial.println("Archivos recopilados desde SD");
        SDRead("backup.log", horometro, valoresHorometro, nH);
    }

    Serial.println("");
}

void SDRead(String archivo, String header[], String values[], int sizeA)
{
    if (SD.exists(archivo))
    {
        Serial.println("Leyendo " + archivo);
        File logFile = SD.open(archivo);
        DynamicJsonDocument doc(1024);
        DeserializationError error = deserializeJson(doc, logFile);
        serializeJsonPretty(doc, Serial);
        Serial.println("");
        if (!error)
        {
            for (int i = 0; i < sizeA; i++)
            {
                values[i] = doc[header[i]].as<String>();
            }
        }
        else
        {
            Serial.println("Error al abrir el archivo" + archivo);
        }
    }
}

void SDWrite(String archivo, String header[], String values[], int sizeA)
{
    DynamicJsonDocument doc(1024);

    for (int i = 0; i < sizeA; i++)
    {
        doc[header[i]] = values[i];
    }

    File logFile = SD.open(archivo, "w");
    Serial.println("Guardado en: " + archivo);
    serializeJson(doc, logFile);
    serializeJsonPretty(doc, Serial);
    Serial.println("");
    logFile.close();
}
#endif // SDFileSystem
