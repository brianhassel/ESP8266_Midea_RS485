#include "esphome.h"
#include <esp8266_midea_RS485.h>

#define DI_PIN 16
#define RO_PIN 17 
#define DE_PIN 4
#define RE_PIN 4

#define SERIAL_COM_BUS &Serial
#define SERIAL_COM_CONTROL_PIN DE_PIN
#define SERIAL_COM_MASTER_ID 0
#define SERIAL_COM_SLAVE_ID 0
#define SERIAL_COM_MASTER_SEND_TIME 40
#define SERIAL_COM_SLAVE_TIMEOUT_TIME 100

float DesiredTemp=18;

class MideaACSensor : public PollingComponent, public Sensor {
 public:
  // constructor
  

  Sensor *ACT1Temp = new Sensor();
  Sensor *ACT2ATemp = new Sensor();
  Sensor *ACT2BTemp = new Sensor();
  Sensor *ACT3Temp = new Sensor();
  Sensor *ACNotResponding = new Sensor();
  std::string SetMode = "Unknown";
  std::string SetFanMode = "Unknown";
  uint8_t SetTemp = 18;
  bool aux_heat = 0;
  bool echo_sleep = 0;
  bool vent = 0;
  bool swing = 0;
  bool lock = 0;

  esphome::template_::TemplateSelect *_SetMode;
  esphome::template_::TemplateSelect *_SetFanMode;
  esphome::template_::TemplateNumber *_SetTemp;
  esphome::template_::TemplateSwitch *_aux_heat;
  esphome::template_::TemplateSwitch *_echo_sleep;
  esphome::template_::TemplateSwitch *_vent;
  esphome::template_::TemplateSwitch *_swing;
  esphome::template_::TemplateSwitch *_lock;

  uint8_t update_command=0;
  uint8_t update_internal=0;

  MideaACSensor(\
  esphome::template_::TemplateSelect *&_SetMode_in,\
  esphome::template_::TemplateSelect *&_SetFanMode_in,\
  esphome::template_::TemplateNumber *&_SetTemp_in, \
  esphome::template_::TemplateSwitch *&_aux_heat_in, \
  esphome::template_::TemplateSwitch *&_echo_sleep_in, \
  esphome::template_::TemplateSwitch *&_vent_in, \
  esphome::template_::TemplateSwitch *&_swing_in, \
  esphome::template_::TemplateSwitch *&_lock_in \
  ) : PollingComponent(10000)  {
  _SetMode=_SetMode_in;
  _SetFanMode = _SetFanMode_in;
  _SetTemp=_SetTemp_in;
  _aux_heat = _aux_heat_in;
  _echo_sleep = _echo_sleep_in;
  _vent = _vent_in;
  _swing = _swing_in;
  _lock = _lock_in;

    _SetMode->add_on_state_callback([this](std::string value, size_t size)
                                        { if((1 != update_internal)&&(value != SetMode)){update_command =1;} });
    _SetFanMode->add_on_state_callback([this](std::string value, size_t size)
                                        { if((1 != update_internal)&&(value != SetFanMode)){update_command =1;}});
    _SetTemp->add_on_state_callback([this](float value)
                                        { if((1 != update_internal)&&(value != SetTemp)){update_command =1;}});
    _aux_heat->add_on_state_callback([this](bool value)
                                        { if((1 != update_internal)&&(value != aux_heat)){update_command =1;}});
    _echo_sleep->add_on_state_callback([this](bool value)
                                        { if((1 != update_internal)&&(value != echo_sleep)){update_command =1;}});
    _swing->add_on_state_callback([this](bool value)
                                        { if((1 != update_internal)&&(value != swing)){update_command =1;}});
    _vent->add_on_state_callback([this](bool value)
                                        { if((1 != update_internal)&&(value != vent)){update_command =1;}});
    _lock->add_on_state_callback([this](bool value)
                                        { if(value == 1){ESP8266_Midea_RS485.Lock();}else{ESP8266_Midea_RS485.Unlock();}});

  }

  float get_setup_priority() const override { return esphome::setup_priority::AFTER_WIFI; }

  void setup() override {
    
  ESP8266_Midea_RS485.begin(SERIAL_COM_BUS,SERIAL_COM_CONTROL_PIN,SERIAL_COM_MASTER_ID,SERIAL_COM_SLAVE_ID,SERIAL_COM_MASTER_SEND_TIME,SERIAL_COM_SLAVE_TIMEOUT_TIME);
  
  
  
  }
  void update() override {

  float temp;
  uint8_t index;

  if(1==update_command)
  {
    SetMode = _SetMode->state.c_str();
  
    if(SetMode == "Auto")
    {
        ESP8266_Midea_RS485.SetMode(MIDEA_AC_OPMODE_AUTO);
    }
    if(SetMode == "Off")
    {
        ESP8266_Midea_RS485.SetMode(MIDEA_AC_OPMODE_OFF);
    }
    if(SetMode =="Cool")
    {
        ESP8266_Midea_RS485.SetMode(MIDEA_AC_OPMODE_COOL);
    }
    if(SetMode == "Heat")
    {
        ESP8266_Midea_RS485.SetMode(MIDEA_AC_OPMODE_HEAT);
    }
    if(SetMode == "Dry")
    {
        ESP8266_Midea_RS485.SetMode(MIDEA_AC_OPMODE_DRY);
    }
    if(SetMode == "Fan")
    {
        ESP8266_Midea_RS485.SetMode(MIDEA_AC_OPMODE_FAN);
    }
    
    SetFanMode = _SetFanMode->state.c_str();

    if(SetFanMode=="Auto")
    {
        ESP8266_Midea_RS485.SetFanMode(MIDEA_AC_FANMODE_AUTO);
    }
    if(SetFanMode=="High")
    {
        ESP8266_Midea_RS485.SetFanMode(MIDEA_AC_FANMODE_HIGH);
    }
    if(SetFanMode=="Medium")
    {
        ESP8266_Midea_RS485.SetFanMode(MIDEA_AC_FANMODE_MEDIUM);
    }
    if(SetFanMode=="Low")
    {
        ESP8266_Midea_RS485.SetFanMode(MIDEA_AC_FANMODE_LOW);
    }
    
    SetTemp = (_SetTemp->state)/1;
    ESP8266_Midea_RS485.SetTemp(SetTemp);

    aux_heat = _aux_heat->state;
    if(!aux_heat)
    {
        ESP8266_Midea_RS485.SetAuxHeat_Turbo(MIDEA_AC_ACTIVE);
    }else
        {
            ESP8266_Midea_RS485.SetAuxHeat_Turbo(MIDEA_AC_INACTIVE);
        }
    
    echo_sleep = _echo_sleep->state;
    if(!echo_sleep)
    {
        ESP8266_Midea_RS485.SetEcho_Sleep(MIDEA_AC_ACTIVE);
    }else
        {
            ESP8266_Midea_RS485.SetEcho_Sleep(MIDEA_AC_INACTIVE);
        }

    vent = _vent->state;
    if(!vent)
    {
        ESP8266_Midea_RS485.SetVent(MIDEA_AC_ACTIVE);
    }else
        {
            ESP8266_Midea_RS485.SetVent(MIDEA_AC_INACTIVE);
        }

    swing = _swing->state;
    if(!swing)
    {
        ESP8266_Midea_RS485.SetSwing(MIDEA_AC_ACTIVE);
    }else
        {
            ESP8266_Midea_RS485.SetSwing(MIDEA_AC_INACTIVE);
        }

    update_command = 2;
  } 

  ESP8266_Midea_RS485.Update();

  ESP_LOGD("custom","SentData: %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x ",\
  ESP8266_Midea_RS485.SentData[0],\
  ESP8266_Midea_RS485.SentData[1],\
  ESP8266_Midea_RS485.SentData[2],\
  ESP8266_Midea_RS485.SentData[3],\
  ESP8266_Midea_RS485.SentData[4],\
  ESP8266_Midea_RS485.SentData[5],\
  ESP8266_Midea_RS485.SentData[6],\
  ESP8266_Midea_RS485.SentData[7],\
  ESP8266_Midea_RS485.SentData[8],\
  ESP8266_Midea_RS485.SentData[9],\
  ESP8266_Midea_RS485.SentData[10],\
  ESP8266_Midea_RS485.SentData[11],\
  ESP8266_Midea_RS485.SentData[12],\
  ESP8266_Midea_RS485.SentData[13],\
  ESP8266_Midea_RS485.SentData[14],\
  ESP8266_Midea_RS485.SentData[15]);
  ESP_LOGD("custom","ReceivedData: %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x",\
  ESP8266_Midea_RS485.ReceivedData[0],\
  ESP8266_Midea_RS485.ReceivedData[1],\
  ESP8266_Midea_RS485.ReceivedData[2],\
  ESP8266_Midea_RS485.ReceivedData[3],\
  ESP8266_Midea_RS485.ReceivedData[4],\
  ESP8266_Midea_RS485.ReceivedData[5],\
  ESP8266_Midea_RS485.ReceivedData[6],\
  ESP8266_Midea_RS485.ReceivedData[7],\
  ESP8266_Midea_RS485.ReceivedData[8],\
  ESP8266_Midea_RS485.ReceivedData[9],\
  ESP8266_Midea_RS485.ReceivedData[10],\
  ESP8266_Midea_RS485.ReceivedData[11],\
  ESP8266_Midea_RS485.ReceivedData[12],\
  ESP8266_Midea_RS485.ReceivedData[13],\
  ESP8266_Midea_RS485.ReceivedData[14],\
  ESP8266_Midea_RS485.ReceivedData[15],\
  ESP8266_Midea_RS485.ReceivedData[16],\
  ESP8266_Midea_RS485.ReceivedData[17],\
  ESP8266_Midea_RS485.ReceivedData[18],\
  ESP8266_Midea_RS485.ReceivedData[19],\
  ESP8266_Midea_RS485.ReceivedData[20],\
  ESP8266_Midea_RS485.ReceivedData[21],\
  ESP8266_Midea_RS485.ReceivedData[22],\
  ESP8266_Midea_RS485.ReceivedData[23],\
  ESP8266_Midea_RS485.ReceivedData[24],\
  ESP8266_Midea_RS485.ReceivedData[25],\
  ESP8266_Midea_RS485.ReceivedData[26],\
  ESP8266_Midea_RS485.ReceivedData[27],\
  ESP8266_Midea_RS485.ReceivedData[28],\
  ESP8266_Midea_RS485.ReceivedData[29],\
  ESP8266_Midea_RS485.ReceivedData[30],\
  ESP8266_Midea_RS485.ReceivedData[31],\
  ESP8266_Midea_RS485.ReceivedData[32],\
  ESP8266_Midea_RS485.ReceivedData[33],\
  ESP8266_Midea_RS485.ReceivedData[34],\
  ESP8266_Midea_RS485.ReceivedData[35],\
  ESP8266_Midea_RS485.ReceivedData[36],\
  ESP8266_Midea_RS485.ReceivedData[37],\
  ESP8266_Midea_RS485.ReceivedData[38],\
  ESP8266_Midea_RS485.ReceivedData[39]);
  
  for(index=0;index<40;index++)
  {
    ESP8266_Midea_RS485.ReceivedData[index]=0;
  }
  
  if(ESP8266_Midea_RS485.State.ACNotResponding == 0)
  {
    update_internal = 1;
    
    if(ESP8266_Midea_RS485.State.OpMode == MIDEA_AC_OPMODE_AUTO)
    {
       _SetMode->publish_state("Auto");
    }else if(ESP8266_Midea_RS485.State.OpMode == MIDEA_AC_OPMODE_OFF)
    {
        _SetMode->publish_state("Off");
    }else if(ESP8266_Midea_RS485.State.OpMode ==MIDEA_AC_OPMODE_COOL)
    {
        _SetMode->publish_state("Cool");
    }else if(ESP8266_Midea_RS485.State.OpMode == MIDEA_AC_OPMODE_HEAT)
    {
        _SetMode->publish_state("Heat");
    }else if(ESP8266_Midea_RS485.State.OpMode == MIDEA_AC_OPMODE_DRY)
    {
        _SetMode->publish_state("Dry");
    }else if(ESP8266_Midea_RS485.State.OpMode == MIDEA_AC_OPMODE_FAN)
    {
        _SetMode->publish_state("Fan");
    }else
    {
        _SetMode->publish_state("Unknown");
    }

    if(ESP8266_Midea_RS485.State.FanMode == MIDEA_AC_FANMODE_AUTO)
    {
       _SetFanMode->publish_state("Auto");
    }else if(ESP8266_Midea_RS485.State.FanMode == MIDEA_AC_FANMODE_HIGH)
    {
        _SetFanMode->publish_state("High");
    }else if(ESP8266_Midea_RS485.State.FanMode ==MIDEA_AC_FANMODE_MEDIUM)
    {
        _SetFanMode->publish_state("Medium");
    }else if(ESP8266_Midea_RS485.State.FanMode == MIDEA_AC_FANMODE_LOW)
    {
        _SetFanMode->publish_state("Low");
    }else
    {
        _SetFanMode->publish_state("Unknown");
    }
    
    if(ESP8266_Midea_RS485.State.SetTemp>0)
    {
        temp = ESP8266_Midea_RS485.State.SetTemp * 1.0;
        _SetTemp->publish_state(temp);
    }else
        {
            _SetTemp->publish_state(18);
        }
    
    _aux_heat->publish_state((ESP8266_Midea_RS485.State.OperatingFlags&0x02)>0);
    _echo_sleep->publish_state((ESP8266_Midea_RS485.State.OperatingFlags&0x01)>0);
    _vent->publish_state((ESP8266_Midea_RS485.State.OperatingFlags&0x88)>0);
    _swing->publish_state((ESP8266_Midea_RS485.State.OperatingFlags&0x04)>0);

    update_internal = 0;
  }

  ACT1Temp->publish_state(ESP8266_Midea_RS485.State.T1Temp);
  ACT2ATemp->publish_state(ESP8266_Midea_RS485.State.T2ATemp);
  ACT2BTemp->publish_state(ESP8266_Midea_RS485.State.T2BTemp);
  ACT3Temp->publish_state(ESP8266_Midea_RS485.State.T3Temp);
  ACNotResponding->publish_state(ESP8266_Midea_RS485.State.ACNotResponding);

  }
};