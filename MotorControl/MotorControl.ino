#include <FlexCAN.h>
#include <kinetis_flexcan.h>

#include "talon_rio.c"

FlexCAN can(1000000);
static CAN_message_t rxmsg;
void setup() {
  pinMode(13, OUTPUT);
  can.begin();
  Serial.begin(115200);
  while(!Serial.available());
}

bool on = false;
char buffer[50];

#define number_of_devices_to_emulate 4 // id from 0 to n-1

void loop() {
  if(can.read(rxmsg)){
    int id = rxmsg.id & PART_DEVID;
    if(id > number_of_devices_to_emulate) // are we responsible for this packet?
      return;
    
    digitalWrite(13, on = !on); // blink the light, we've got a packet!
    int type = rxmsg.id & PART_PACKTYPE;
    
    switch(type){
    case PACKET_SPEEDCHG:
      sprintf(buffer, "%d: %f", id, packetSpeedchgSpeed(rxmsg.buf));
      Serial.println(buffer);
      break;
    }
  }
}
