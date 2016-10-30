#include <FlexCAN.h>
#include <kinetis_flexcan.h>

#include "talon_rio.c"

#define number_of_devices_to_emulate 4 // id from 0 to n-1

FlexCAN can(1000000);
static CAN_message_t rxmsg,txmsg;
char buffer[50];
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

void cleanMessage(uint8_t buf[8]){
  for(int i = 0; i<8; i++)
    buf[i] = 0x0;
}

