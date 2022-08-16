/*
    ESP8266 library to control/monitor Midea air conditioner via XYE (RS485) bus
*/

#ifndef _esp8266_midea_rs485_h
    #define _esp8266_midea_rs485_h
    #include "Arduino.h"
    
class ESP8266_Midea_RS485Class {
  public:
    ESP8266_Midea_RS485Class(uint8_t re_de_pin, uint8_t master_id, uint8_t slave_id, uint8_t command_time, uint8_t response_timeout);
    
    virtual void begin(uint8_t re_de_pin, uint8_t master_id, uint8_t slave_id, uint8_t command_time, uint8_t response_timeout);
    void EnableTX();
    void EnableRX();
    void Update();
    MideaACPropertiesType State;
    uint8_t SentData[16];
    uint8_t ReceivedData[32];
    uint8_t state;
  private:
    uint8_t ComControlPin;
    uint8_t SlaveId;
    uint8_t MasterId;
    uint8_t Master_Send_Time;
    uint8_t Slave_Resp_Time;
  protected:
    uint8_t CalculateCRC();
    void ClearResponseBuffer();
}; 

extern ESP8266_Midea_RS485Class ESP8266_Midea_RS485;  

#endif