#ifndef _variables
#define _variables

// Variables
double t1 = 0;

String configuraciones[] = {"mqtt_server","mqtt_port","mqtt_user","mqtt_pass","equipo","codigo"};
const int n=sizeof(configuraciones)/sizeof(configuraciones[0]);
String valoresConfig[n];

String horometro[] = {"Y","M","D","H","Min","S"};
const int nH=sizeof(horometro)/sizeof(horometro[0]);
String valoresHorometro[nH] = {"0","0","0","0","0","0"};

String prueba[] = {"msg"};
const int nP=sizeof(prueba)/sizeof(prueba[0]);
String valoresPrueba[nP];

char msg[50];
bool banderaMQTT = true;
bool banderaSD = false;
bool banderaBackup = false;
bool banderaWifi = true;


// Variables BOD
float volt_ini=0;
bool banBOD = true;

#endif