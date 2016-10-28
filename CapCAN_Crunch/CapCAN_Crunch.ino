#include <FlexCAN.h>
#include <kinetis_flexcan.h>

FlexCAN can(1000000);
static CAN_message_t rxmsg;
void setup() {
  pinMode(13, OUTPUT);
  can.begin();
  Serial.begin(115200);
  while(!Serial.available());
}

bool on = false;
int bytesSent = 0;
char buffer[50];

void loop() {
  if(can.read(rxmsg)){
    digitalWrite(13, on = !on);
    
    sprintf(buffer, "%04lx %02x | ", rxmsg.id, rxmsg.ext);
    Serial.print(buffer);
    for(int i = 0; i < rxmsg.len; ++i){
      sprintf(buffer, "%02x ", rxmsg.buf[i]);
      Serial.print(buffer);
      bytesSent++;
    }
    Serial.println();
  }
  
}
