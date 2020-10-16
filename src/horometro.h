#include <variables.h>
#include <FileSystem.h>
#include <RTClib.h>

#ifndef HR
#define HR
RTC_DS3231 rtc;

void setHoro(String fecha[6])
{
    int Y = fecha[0].toInt();
    int M = fecha[1].toInt();
    int D = fecha[2].toInt();
    int H = fecha[3].toInt();
    int Min = fecha[4].toInt();
    int S = fecha[5].toInt();
    Serial.println("Fecha seteada:" + String(Y) + ":" + String(M) + ":" + String(D) + ":" + String(H) + ":" + String(Min) + ":" + String(S));
    rtc.adjust(DateTime(Y,M,D,H,Min,S));
}
void resetHoro()
{
    String aux[]= {"0","0","0","0","0","0"};
    FSWrite("/horometro.json", horometro, aux, nH);
}
#endif
