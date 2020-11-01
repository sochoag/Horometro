#include <ConexionWifi.h>
#include <variables.h>
#include <PubSubClient.h>


#ifndef connectMQTT
#define connectMQTT

WiFiClient HR01;
PubSubClient clientmqtt(HR01);

void reconnectMQTT();
void recibirVars();
void recibirJson(byte *payload, unsigned int length, String header[], String values[], int sizeA);

void callback(char *topic, byte *payload, unsigned int length)
{
    String incoming = "";
    //Serial.println("Mensaje recibido desde -> " + String(topic));

    for (uint8_t i = 0; i < length; i++)
    {
        incoming += (char)payload[i];
    }

    incoming.trim();

    if (strcmp(topic, "vars") == 0)
    {
        if (!banderaBackup)
        {
            Serial.println("════════════════════════════════════════════════════════════");
            Serial.println("Recopilando variables desde el Servidor MQTT...");
            //escribir_oled("Recopilando Variables",10);
            Serial.println("────────────────────────────────────────────────────────────");
            recibirJson(payload, length, horometro, valoresHorometro, nH);
            Serial.println("────────────────────────────────────────────────────────────");
            Serial.println("Variables recuperadas");
            //escribir_oled("Variables recuperadas",10);
            Serial.println("════════════════════════════════════════════════════════════");
            Serial.println("════════════════════════════════════════════════════════════");
            Serial.println("Configurando horometro...");
            //escribir_oled("Configurando horometro",10);
            Serial.println("Horometro configurado");
            //escribir_oled("Horometro configurado",10);
            Serial.println("════════════════════════════════════════════════════════════");
        }
        clientmqtt.unsubscribe("vars");
        banderaMQTT = false;
    }

    //Serial.println("Mensaje -> " + incoming);
}

void setupMQTT()
{
    Serial.println("════════════════════════════════════════════════════════════");
    Serial.println("Configurando Servidor MQTT...");
    //escribir_oled("Configurando MQTT",10);
    clientmqtt.setServer(valoresConfig[0].c_str(), valoresConfig[1].toInt());
    //clientmqtt.setKeepAlive(30);
    clientmqtt.setCallback(callback);
    while (!clientmqtt.connected())
    {
        reconnectMQTT();
    }
    Serial.println("Servido MQTT configurado");
    escribir_oled("MQTT Configurado", 10);
    Serial.println("────────────────────────────────────────────────────────────");
    Serial.println("Servidor MQTT: " + valoresConfig[0] + " Puerto MQTT: " + valoresConfig[1]);
    Serial.println("════════════════════════════════════════════════════════════");
    recibirVars();
}

void MQTTWatch()
{
    if (!clientmqtt.connected() && !banderaSD)
    {
        reconnectMQTT();
    }
    clientmqtt.loop();
}

void reconnectMQTT()
{
    while (!WiFi.isConnected() && !banderaSD)
    {
        wm.setConfigPortalTimeout(2);
        if (!wm.autoConnect("Horometro", "12345678"))
        {
            Serial.println("No se pudo conectar");
            escribir_oled("Wifi Caido - Usando SD", 10);
            banderaSD = true;
        }
    }
    int c_rec = 0;
    int c_cred = 0;
    Serial.println("Intentando conexión Mqtt...");
    while (!clientmqtt.connected() && !banderaSD)
    {
        Serial.print("■");
        //escribir_oled("Conectando MQTT",10);
        // Creamos un cliente ID
        String clientId = "HR01_";
        clientId += String(random(0xffff), HEX);
        // Intentamos conectar
        //if (clientmqtt.connect(clientId.c_str(),valoresConfig[2].c_str(),valoresConfig[3].c_str()))
        if (clientmqtt.connect(clientId.c_str(), valoresConfig[2].c_str(), valoresConfig[3].c_str()))
        {
            Serial.println("Conectado!");
            escribir_oled("Conectado", 10);
            Serial.println("────────────────────────────────────────────────────────────");
            banderaSD = false;
            // Nos suscribimos
            if (banderaMQTT)
            {
                clientmqtt.subscribe("vars");
            }
        }
        else
        {
            delay(500);
            if ((clientmqtt.state() == 4 || clientmqtt.state() == 5) && (banderaMQTT))
            {
                c_cred++;
                if (c_cred > 20)
                {
                    Serial.println("Error: " + String(clientmqtt.state()));
                    wm.resetSettings();
                    Serial.println("Por favor revise las credenciales MQTT");
                    escribir_oled("Revise Credenciales", 10);
                    delay(1000);
                    ESP.restart();
                }
                delay(500);
            }
            else
            {
                c_rec++;
                if (c_rec > 20)
                {
                    Serial.println("Error: " + String(clientmqtt.state()));
                    Serial.println("■");
                    // FSWrite("/horometro.json", horometro, valoresHorometro, nH);
                    delay(1000);
                    Serial.println("Imposible reconectar a MQTT 🡢 Usando SD");
                    escribir_oled("MQTT Caido - Usando SD", 10);
                    delay(1000);
                    banderaSD = true;
                }
            }
        }
    }
}

void publishString(String topic, String to_send)
{
    //Serial.println("Enviando " + to_send + " al topico " + topic);
    int size_to_send = to_send.length();
    to_send.toCharArray(msg, size_to_send + 1);
    clientmqtt.publish(topic.c_str(), msg, true);
}

void publishJson(String topic, String header[], String values[], int sizeA)
{
    const size_t capacity = JSON_OBJECT_SIZE(6) + 40;
    DynamicJsonDocument doc(capacity);
    for (int i = 0; i < sizeA; i++)
    {
        //Serial.println(header[i] + " ···· "+ values[i]);
        doc[header[i]] = values[i];
    }
    char buffer[256];
    size_t n = serializeJson(doc, buffer);
    clientmqtt.publish(topic.c_str(), (uint8_t *)buffer, n, true);
}

void recibirJson(byte *payload, unsigned int length, String header[], String values[], int sizeA)
{
    const size_t capacity = JSON_OBJECT_SIZE(6) + 40;
    DynamicJsonDocument doc(capacity);
    deserializeJson(doc, payload, length);
    for (int i = 0; i < sizeA; i++)
    {
        //String aux = json["sensor"].as<String>();
        values[i] = doc[header[i]].as<String>();
    }
    Serial.println("Variables recibidas");
    serializeJsonPretty(doc, Serial);
    Serial.println("");
}

void limpiarVariables()
{
    String aux[] = {"0", "0", "0", "0", "0", "0"};
    publishJson("vars", horometro, aux, nH);
    Serial.println("");
    delay(100);
}

void recibirVars()
{
    while (banderaMQTT)
    {
        clientmqtt.loop();
    }
}
#endif