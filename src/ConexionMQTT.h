#include <ConexionWifi.h>
#include <variables.h>
#include <PubSubClient.h>

#ifndef connectMQTT
#define connectMQTT

WiFiClient HR01;
PubSubClient clientmqtt(HR01);

void reconnectMQTT();

void callback(char *topic, byte *payload, unsigned int length)
{
    String incoming = "";
    Serial.println("Mensaje recibido desde -> " + String(topic));

    for (uint8_t i = 0; i < length; i++)
    {
        incoming += (char)payload[i];
    }

    incoming.trim();
    
    if (strcmp(topic,"lastReg")==0)
    {
        clientmqtt.unsubscribe("lastReg");
        banderaMQTT = false;
    }

    Serial.println("Mensaje -> " + incoming);
}

void setupMQTT()
{
    Serial.print(valoresConfig[0]+" - "+valoresConfig[1]);
    clientmqtt.setServer(valoresConfig[0].c_str(), valoresConfig[1].toInt());
    //clientmqtt.setKeepAlive(30);
    clientmqtt.setCallback(callback);
}

void MQTTWatch()
{
    if (!clientmqtt.connected())
    {
        reconnectMQTT();
    }
    clientmqtt.loop();
}

void reconnectMQTT()
{
    Serial.println("Reconexion");
    while (!clientmqtt.connected())
    {
        Serial.print("Intentando conexión Mqtt...");
        // Creamos un cliente ID
        String clientId = "HR01_";
        clientId += String(random(0xffff), HEX);
        // Intentamos conectar
        Serial.println(valoresConfig[2]+" - "+valoresConfig[3]);
        //if (clientmqtt.connect(clientId.c_str(),valoresConfig[2].c_str(),valoresConfig[3].c_str()))
        if (clientmqtt.connect(clientId.c_str(),valoresConfig[2].c_str(),valoresConfig[3].c_str()))
        {
            Serial.println("Conectado!");
            // Nos suscribimos
            if(banderaMQTT)
            {
                clientmqtt.subscribe("lastReg");
                Serial.println("Suscrito a lastReg");
            }
        }
        else
        {
            Serial.print("falló :( con error -> ");
            Serial.println(clientmqtt.state());
            delay(500);
        }
    }
}

void publishString(String topic, String to_send)
{
    Serial.println("Enviando "+to_send+" al topico "+ topic);
    int size_to_send = to_send.length();
    to_send.toCharArray(msg, size_to_send+1);
    clientmqtt.publish(topic.c_str(), msg, true);
}
#endif