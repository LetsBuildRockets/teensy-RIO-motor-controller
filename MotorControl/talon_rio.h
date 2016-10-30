#define PART_DEVID 0x0000003F // 63 device ids on the bus
#define PART_PACKTYPE (~PART_DEVID) // close enough

// send
#define PACKET_STARTUP 0x02041500
#define PACKET_STARTUP_LEN 6
void packetStartupVersion(uint8_t buf[8], byte fwMaj, byte fwMin);

// listen
#define PACKET_SPEEDCHG 0x02040000
float packetSpeedchgSpeed(uint8_t buf[8]);
