#include "esphome.h"
#include <esp8266_midea_RS485.h>

#define SERIAL_COM_CONTROL_PIN 4
#define SERIAL_COM_MASTER_ID 0
#define SERIAL_COM_SLAVE_ID 0xFF
#define SERIAL_COM_MASTER_SEND_TIME 30
#define SERIAL_COM_SLAVE_TIMEOUT_TIME 100

float DesiredTemp = 18;

class MideaACSensor : public PollingComponent, public Sensor
{
public:

    Sensor *state = new Sensor();

    MideaACSensor() : PollingComponent(10000){}

    float get_setup_priority() const override { return esphome::setup_priority::AFTER_WIFI; }

    void setup() override
    {
        ESP8266_Midea_RS485.begin(SERIAL_COM_CONTROL_PIN, SERIAL_COM_MASTER_ID, SERIAL_COM_SLAVE_ID, SERIAL_COM_MASTER_SEND_TIME, SERIAL_COM_SLAVE_TIMEOUT_TIME);
    }
    void update() override
    {

        ESP8266_Midea_RS485.Update();

        ESP_LOGD("custom", "SentData: %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x ",
                 ESP8266_Midea_RS485.SentData[0],
                 ESP8266_Midea_RS485.SentData[1],
                 ESP8266_Midea_RS485.SentData[2],
                 ESP8266_Midea_RS485.SentData[3],
                 ESP8266_Midea_RS485.SentData[4],
                 ESP8266_Midea_RS485.SentData[5],
                 ESP8266_Midea_RS485.SentData[6],
                 ESP8266_Midea_RS485.SentData[7],
                 ESP8266_Midea_RS485.SentData[8],
                 ESP8266_Midea_RS485.SentData[9],
                 ESP8266_Midea_RS485.SentData[10],
                 ESP8266_Midea_RS485.SentData[11],
                 ESP8266_Midea_RS485.SentData[12],
                 ESP8266_Midea_RS485.SentData[13],
                 ESP8266_Midea_RS485.SentData[14],
                 ESP8266_Midea_RS485.SentData[15]);
        ESP_LOGD("custom", "ReceivedData: %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x",
                 ESP8266_Midea_RS485.ReceivedData[0],
                 ESP8266_Midea_RS485.ReceivedData[1],
                 ESP8266_Midea_RS485.ReceivedData[2],
                 ESP8266_Midea_RS485.ReceivedData[3],
                 ESP8266_Midea_RS485.ReceivedData[4],
                 ESP8266_Midea_RS485.ReceivedData[5],
                 ESP8266_Midea_RS485.ReceivedData[6],
                 ESP8266_Midea_RS485.ReceivedData[7],
                 ESP8266_Midea_RS485.ReceivedData[8],
                 ESP8266_Midea_RS485.ReceivedData[9],
                 ESP8266_Midea_RS485.ReceivedData[10],
                 ESP8266_Midea_RS485.ReceivedData[11],
                 ESP8266_Midea_RS485.ReceivedData[12],
                 ESP8266_Midea_RS485.ReceivedData[13],
                 ESP8266_Midea_RS485.ReceivedData[14],
                 ESP8266_Midea_RS485.ReceivedData[15],
                 ESP8266_Midea_RS485.ReceivedData[16],
                 ESP8266_Midea_RS485.ReceivedData[17],
                 ESP8266_Midea_RS485.ReceivedData[18],
                 ESP8266_Midea_RS485.ReceivedData[19],
                 ESP8266_Midea_RS485.ReceivedData[20],
                 ESP8266_Midea_RS485.ReceivedData[21],
                 ESP8266_Midea_RS485.ReceivedData[22],
                 ESP8266_Midea_RS485.ReceivedData[23],
                 ESP8266_Midea_RS485.ReceivedData[24],
                 ESP8266_Midea_RS485.ReceivedData[25],
                 ESP8266_Midea_RS485.ReceivedData[26],
                 ESP8266_Midea_RS485.ReceivedData[27],
                 ESP8266_Midea_RS485.ReceivedData[28],
                 ESP8266_Midea_RS485.ReceivedData[29],
                 ESP8266_Midea_RS485.ReceivedData[30],
                 ESP8266_Midea_RS485.ReceivedData[31],
                 ESP8266_Midea_RS485.ReceivedData[32],
                 ESP8266_Midea_RS485.ReceivedData[33],
                 ESP8266_Midea_RS485.ReceivedData[34],
                 ESP8266_Midea_RS485.ReceivedData[35],
                 ESP8266_Midea_RS485.ReceivedData[36],
                 ESP8266_Midea_RS485.ReceivedData[37],
                 ESP8266_Midea_RS485.ReceivedData[38],
                 ESP8266_Midea_RS485.ReceivedData[39]);

        for (index = 0; index < 32; index++)
        {
            ESP8266_Midea_RS485.ReceivedData[index] = 0;
        }

        state->publish_state(ESP8266_Midea_RS485.state)
    }
};