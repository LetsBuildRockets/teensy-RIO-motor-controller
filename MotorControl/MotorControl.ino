#include <FlexCAN.h>
#include <kinetis_flexcan.h>
#include <Metro.h>

#include "talon_rio.h"

#define number_of_devices_to_emulate 4 // id from 0 to n-1

FlexCAN can(1000000);
static CAN_message_t rxmsg,txmsg;
char buffer[50];
Metro heartbeat = Metro(10/*ms*/);
Metro encoderTask = Metro(100/*ms*/);
void setup() {
  pinMode(13, OUTPUT);
  can.begin();
  Serial.begin(115200);
  while(!Serial.available());

  for(int i = 0; i<number_of_devices_to_emulate; i++){
    cleanMessage(txmsg.buf);
    packetStartupVersion(txmsg.buf, 1, 13);
    txmsg.id = PACKET_STARTUP | i;
    txmsg.len = PACKET_STARTUP_LEN;
    can.write(txmsg);
    sprintf(buffer, "Started TALON #%d", i);
    Serial.println(buffer);
  }
}

bool on = false;
float throttle[number_of_devices_to_emulate] = {0.0};
void loop() {
  if(heartbeat.check())
    for(int i = 0; i<number_of_devices_to_emulate; i++){
      cleanMessage(txmsg.buf);
      packetHbeatReportedThrottle(txmsg.buf,throttle[i]);
      txmsg.id = PACKET_HBEAT | i;
      txmsg.len = PACKET_HBEAT_LEN;
      can.write(txmsg);
    }
//  if(encoderTask.check())
//    for(int i = 0; i<number_of_devices_to_emulate; i++){
//      cleanMessage(txmsg.buf);
//      packetEncoderPosition(txmsg.buf,100);
//      packetEncoderRate(txmsg.buf,100);
//      txmsg.id = PACKET_ENCODER | i;
//      txmsg.len = PACKET_ENCODER_LEN;
//      can.write(txmsg);
//    }
  if(can.read(rxmsg)){
    bool unrequited_packet = false;
    if(rxmsg.id == PACKET_DISABLE){
      Serial.println("DISABLED");
    } else if(rxmsg.id == PACKET_ENABLE){
      Serial.println("ENABLED");
    } else {
      int id = rxmsg.id & PART_DEVID;
      if(id < number_of_devices_to_emulate){ // are we responsible for this packet?
        digitalWrite(13, on = !on); // blink the light, we've got a packet!
        int type = rxmsg.id & PART_PACKTYPE;
        
        switch(type){
        case PACKET_SPEEDCHG: {
          float v = packetSpeedchgSpeed(rxmsg.buf);
          sprintf(buffer, "%d: %f", id, v);
          throttle[id] = v;
          Serial.println(buffer);
          break;
        }
        default:
          unrequited_packet = true;
          Serial.print("?TYPE ");
          break;
        }
      } else {
        unrequited_packet = true;
        Serial.print("?ADDR ");
      }
    }
    if(unrequited_packet){
      sprintf(buffer, "%04lx %02x | ", rxmsg.id, rxmsg.ext);
      Serial.print(buffer);
      for(int i = 0; i < rxmsg.len; ++i){
        sprintf(buffer, "%02x ", rxmsg.buf[i]);
        Serial.print(buffer);
      }
      Serial.println();
    }
  }
}

void cleanMessage(uint8_t buf[8]){
  for(int i = 0; i<8; i++)
    buf[i] = 0x0;
}
