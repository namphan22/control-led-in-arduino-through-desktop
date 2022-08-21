#include<stdint.h>
#define LedPin 13
bool sendSerial=false;
bool stringComplete=false;
String input_string="";
uint64_t count;
volatile int32_t state;

typedef enum{
    PROCESS_LED_TURN_OFF,
    PROCESS_LED_TURN_ON,
    PROCESS_LED_TOGGLE,
    PROCESS_ERROR,
}process_type_def;

process_type_def process_id =PROCESS_ERROR;
static void BlinkLed(int32_t state)
{
    switch (state)
    {
    case 0:
        digitalWrite(LedPin,LOW);
        break;
    case 1:
        digitalWrite(LedPin,HIGH);
        break;
    default:
        break;
    }
}
static int32_t process_message(String input_from_pc)
{
    if(input_from_pc.startsWith("turn off led")){return PROCESS_LED_TURN_OFF ;}
    else if(input_from_pc.startsWith("turn on led")){return PROCESS_LED_TURN_ON;}
    else if(input_from_pc.startsWith("toggle led")){return PROCESS_LED_TOGGLE;}
    else{return PROCESS_ERROR;}

}
void setup()
{
    Serial.begin(115200);
    pinMode(LedPin,OUTPUT);
    digitalWrite(LedPin,LOW);
    input_string.reserve(100);
}
void loop()
{
    if(stringComplete)
    {
        process_id = process_message(input_string);
        switch (process_id)
        {
        case PROCESS_LED_TURN_OFF :

            state = 0;
            break;
        case PROCESS_LED_TURN_ON:
            state =1;
            break;
        case PROCESS_LED_TOGGLE:
            state ==0?state=1:state =0;
            break;
        
        default:
            break;
        }
        BlinkLed(state);
        count =0;
        stringComplete =false;
        input_string="";
        sendSerial =true;
        process_id = PROCESS_ERROR;
    }
    else
    {
        // automatically toggled
        count++;
        if(count==50){
            count =0;
            state ==0?state=1:state =0;
            BlinkLed(state);
            sendSerial =true;
        }
        delay(100);
    }
    if(sendSerial)
    {
        sendSerial =false;
        Serial.print("led:");
        Serial.print(state);
        Serial.print("\r\n");
    }


}
void SerialEvent()
{
    while (Serial.available())
    {
        char buffer =(char)Serial.read();
        input_string +=buffer;
        if(buffer='\n')
        {
            stringComplete =true;
        }
    }
    

}
