// Definiciones
#include <Arduino.h>
#include <FileSystem.h>
#include <Conexion.h>
#include <horometro.h>
#include <variables.h>

#define button 0

void setup()
{
  Serial.begin(115200);
  if (! rtc.begin())
  {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    abort();
  }
  Serial.println();
  pinMode(button, INPUT_PULLUP);
  
  setupFS();
  setHoro(valoresHorometro);
  //setupAP();

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
    DateTime now = rtc.now();

    t1 = t;
    
    valoresHorometro[0] = now.year();
    valoresHorometro[1] = now.month();
    valoresHorometro[2] = now.day();
    valoresHorometro[3] = now.hour();
    valoresHorometro[4] = now.minute();
    valoresHorometro[5] = now.second();
    TimeSpan ts1 = now - DateTime(0,0,0,0,0,0);

    uint32_t minutos = ts1.totalseconds()/3600;
    uint8_t segundos = ts1.minutes();
    FSWrite("/horometro.json",horometro,valoresHorometro,nH);

    Serial.print("Equipo: " + String(valoresConfig[0]) + "\tCodigo: " + String(valoresConfig[1])+ "\tHorometro:"); // put your main code here, to run repeatedly:
    Serial.println(String(minutos)+":"+String(segundos));
    //Serial.println("" + String(now.hour()) + ":"+ now.minute()+":"+ now.second());
  }

  /*if (!WiFi.isConnected())
  {
    reconexion();
  }*/

  if (!digitalRead(0))
  {
    resetAP();
  }
}