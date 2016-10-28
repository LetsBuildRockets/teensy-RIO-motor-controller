#define PART_DEVID 0x0000003F // 63 device ids on the bus
#define PART_PACKTYPE (~PART_DEVID) // close enough

#define PACKET_SPEEDCHG 0x02040000

float packetSpeedchgSpeed(uint8_t buf[8]);
