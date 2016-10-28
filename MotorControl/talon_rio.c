#include <Arduino.h>
#include "talon_rio.h"

int signExtend(unsigned int val, int bits){
  int sign_bit = 1 << (bits - 1);
  return (val & (sign_bit - 1)) - (val & sign_bit);
}

//SPEEDCHG
float packetSpeedchgSpeed(uint8_t buf[8]){
  return (buf[2] << 16 | buf[3] << 8 | buf[4]) / (float)1023;
}
