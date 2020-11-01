#include <variables.h>
#include <SDFileSystem.h>
#include <RTClib.h>

#ifndef HR
#define HR

RTC_DS3231 rtc;

DateTime now;
DateTime last = DateTime(0,0,0,0,0,0);

String readHoro();
void setHoro(String fecha[6]);

void rtcSetup()
{
    while(!rtc.begin())
    {
        ESP.restart();
    }
    setHoro(valoresHorometro);
}

void setHoro(String fecha[6])
{
    int Y = fecha[0].toInt();
    int M = fecha[1].toInt();
    int D = fecha[2].toInt();
    int H = fecha[3].toInt();
    int Min = fecha[4].toInt();
    int S = fecha[5].toInt();
    //Serial.println("Fecha seteada:" + String(Y) + ":" + String(M) + ":" + String(D) + ":" + String(H) + ":" + String(Min) + ":" + String(S));
    rtc.adjust(DateTime(Y,M,D,H,Min,S));
    now = rtc.now();
    Serial.println("Horometro: " + readHoro());
}
void resetHoro()
{
    String aux[]= {"0","0","0","0","0","0"};
    SDWrite("backup.log", horometro, aux, nH);
}

String readHoro()
{   
    TimeSpan ts1 = now - DateTime(0,0,0,0,0,0);

    uint32_t horas = ts1.totalseconds()/3600;
    uint8_t minutos = ts1.minutes();
    String lectura = String(horas)+":"+String(minutos);
    return lectura;
}

boolean changeHoro()
{
    now = rtc.now();

    valoresHorometro[0] = now.year();
    valoresHorometro[1] = now.month();
    valoresHorometro[2] = now.day();
    valoresHorometro[3] = now.hour();
    valoresHorometro[4] = now.minute();
    valoresHorometro[5] = now.second();
    
    if(now.minute() != last.minute())
    {
        last = now;
        return true;
    }
    return false;
}


#endif
