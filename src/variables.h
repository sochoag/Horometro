#ifndef _variables
#define _variables

// Variables
double t1 = 0;
char equipo[8];
char codigo[8];

String encabezados[] = {"equipo","codigo","horo","estado"};
const int n=sizeof(encabezados)/sizeof(encabezados[0]);
String valores[n];
String *p[n];

void iniciarPuntero()
{
    for (size_t i = 0; i < n; i++)
    {
        p[i]=&valores[i]; 
    }
}
#endif