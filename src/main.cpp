// Definiciones
#include <Arduino.h>
#include <FileSystem.h>
#include <Conexion.h>
#include <variables.h>

#define button 0

void setup()
{
  iniciarPuntero();
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

  if (t - t1 > 1000)
  {
    t1 = t;
    String buffH[1] = {"horo"};
    String buffV[1] = {String(t)};
    FSWrite("/data.json",buffH,buffV,1);
    Serial.println("Equipo: " + String(valores[0]) + "\tCodigo: " + String(valores[1])+ "\tHorometro: " + buffV[0]); // put your main code here, to run repeatedly:
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