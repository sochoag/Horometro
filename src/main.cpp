// Definiciones
#include <Arduino.h>
#include <horometro.h>
#include <variables.h>
#include <FileSystem.h>
#include <ConexionWifi.h>
#include <ConexionMQTT.h>
#include <BOD.h>
#include <OLED.h>
#include <SDFileSystem.h>

#define button 2

void setup()
{
  Serial.begin(115200);
  //BODinit();
  setupOled();
  Serial.println();
  pinMode(button, INPUT);
  
  setupFS();
  setupSD();

  setupAP();
  setupMQTT();

  rtcSetup(); 

  String msghoro = readHoro();
  escribir_oled(msghoro,14);
  Serial.println("════════════════════════════════════════════════════════════");
  escribir_oled("Sistema Listo",10);
  delay(1000);
  Serial.println("Sistema Listo");
  Serial.println("════════════════════════════════════════════════════════════");
  if(SD.exists("backup.log"))
  {
    SD.remove("backup.log");
  }
}

void loop()
{
  //BODWatch();
  MQTTWatch();
  if (changeHoro())
  {
    String msgHoro = readHoro();
    Serial.println("Equipo: " + String(valoresConfig[4]) + "\tCodigo: " + String(valoresConfig[5]) + "\tHorometro:" + msgHoro);
    if(!banderaSD)
    {
      Serial.println("Publicando en MQTT");
      publishString("lastReg",msgHoro);
      publishJson("vars",horometro,valoresHorometro,nH);
      estado = "ONLINE";
      if(SD.exists("backup.log"))
      {
        Serial.println("Eliminando archivo BackUp");
        SD.remove("backup.log");
      }
    }
    else
    {
      Serial.println("Guardando en SD");
      SDWrite("backup.log",horometro, valoresHorometro, nH);
      estado = "OFFLINE";
      banderaSD = false;
    }
    escribir_oled(msgHoro,14);
  }


  if (!digitalRead(button))
  {
    limpiarVariables();
    resetHoro();
    resetAP();
    delay(500);
  }
}