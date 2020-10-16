#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <FileSystem.h>
#include <variables.h>

#ifndef BOD
#define BOD

ADC_MODE(ADC_VCC);

void BODinit()
{
    volt_ini = ESP.getVcc() - 70;
}

void BODWatch()
{
  float volt = ESP.getVcc();
  //Serial.println("Volt Ini:"+String(volt_ini)+"Watching:"+String(volt));
  if (volt < volt_ini)
  {
    if(banBOD)
    {
      FSWrite("/horometro.json",horometro,valoresHorometro,nH);
      Serial.println("Guardado exitoso");
      banBOD = !banBOD;
    }
  }
}
#endif