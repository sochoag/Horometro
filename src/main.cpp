// Definiciones
#include <Arduino.h>
#include <FileSystem.h>
#include <Conexion.h>
#include <variables.h>

#define button 0

void setup()
{
  Serial.begin(115200);
  Serial.println();
  pinMode(button, INPUT_PULLUP);
  
  setupFS();
  setupAP();

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

  if (!WiFi.isConnected())
  {
    reconexion();
  }

  if (!digitalRead(0))
  {
    resetAP();
  }
}

void saveConfigCallback()
{
  Serial.println("Should save config");
  shouldSaveConfig = true;
}