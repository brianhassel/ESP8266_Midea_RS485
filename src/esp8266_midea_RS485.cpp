#include "esp8266_midea_RS485.h"

#define MIDEA_BAUDRATE 4800

#define DEFAULT_SERIAL_COM_CONTROL_PIN 4
#define DEFAULT_SERIAL_COM_MASTER_ID 0
#define DEFAULT_SERIAL_COM_SLAVE_ID 0xFF
#define DEFAULT_SERIAL_COM_MASTER_SEND_TIME 30
#define DEFAULT_SERIAL_COM_SLAVE_TIMEOUT_TIME 100

// Master command structure
#define PREAMBLE 0XAA
#define PROLOGUE 0X55

#define MASTER_COMMAND_QUERY 0xC0

#define FROM_MASTER 0x80

ESP8266_Midea_RS485Class::ESP8266_Midea_RS485Class(uint8_t re_de_pin, uint8_t master_id, uint8_t slave_id, uint8_t command_time, uint8_t response_timeout) : ComControlPin(re_de_pin),
                                                                                                                                                             SlaveId(slave_id),
                                                                                                                                                             MasterId(master_id),
                                                                                                                                                             Master_Send_Time(command_time),
                                                                                                                                                             Slave_Resp_Time(response_timeout)
{
}

void ESP8266_Midea_RS485Class::begin(uint8_t re_de_pin, uint8_t master_id, uint8_t slave_id, uint8_t command_time, uint8_t response_timeout)
{
  ComControlPin = re_de_pin;
  pinMode(ComControlPin, OUTPUT);
  EnableRX();
  Serial.begin(MIDEA_BAUDRATE);
  SlaveId = slave_id;
  MasterId = master_id;
  Master_Send_Time = command_time;
  Slave_Resp_Time = response_timeout;
  Update();
}

void ESP8266_Midea_RS485Class::EnableTX()
{
  digitalWrite(ComControlPin, HIGH);
}

void ESP8266_Midea_RS485Class::EnableRX()
{
  digitalWrite(ComControlPin, LOW);
}

void ESP8266_Midea_RS485Class::Update()
{
  uint8_t i, resp;

  // construct query command
  SentData[0] = PREAMBLE;
  SentData[1] = MASTER_COMMAND_QUERY;
  SentData[2] = SlaveId;
  SentData[3] = MasterId;
  SentData[4] = FROM_MASTER;
  SentData[5] = MasterId;
  SentData[6] = 0;
  SentData[7] = 0;
  SentData[8] = 0;
  SentData[9] = 0;
  SentData[10] = 0;
  SentData[11] = 0;
  SentData[12] = 0;
  SentData[13] = 0xFF - SentData[1];
  SentData[15] = PROLOGUE;
  SentData[14] = CalculateCRC();

  EnableTX();
  Serial.write(SentData, 16);
  delay(Master_Send_Time);
  EnableRX();
  delay(Slave_Resp_Time);

  if (Serial.available() > 0)
  {
    for (i = 0; i < 32; i++)
    {
      ReceivedData[i] = Serial.read();
    }
    state = 1;
  }else{
    state=0;
  }
}

uint8_t ESP8266_Midea_RS485Class::CalculateCRC()
{
  uint8_t i = 0;
  uint32_t crc = 0;
  for (i = 0; i < 16; i++)
  {
    if (i != 14)
    {
      crc += SentData[i];
    }
  }
  return 0xFF - (crc & 0xFF);
}

void ESP8266_Midea_RS485Class::ClearResponseBuffer()
{
  uint8_t i = 0;
  for (i = 0; i < 32; i++)
  {
    ReceivedData[i] = 0;
  }
}

ESP8266_Midea_RS485Class ESP8266_Midea_RS485(DEFAULT_SERIAL_COM_CONTROL_PIN, DEFAULT_SERIAL_COM_MASTER_ID, DEFAULT_SERIAL_COM_SLAVE_ID, DEFAULT_SERIAL_COM_MASTER_SEND_TIME, DEFAULT_SERIAL_COM_SLAVE_TIMEOUT_TIME);