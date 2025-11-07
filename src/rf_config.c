#include "rf_config.h"

#define DATA_START_ADDRESS 0x1FFFF800
#define PAGE_SIZE          256

RfConfig Load_Rf_Config()
{
    RfConfig config;
    memcpy(&config, (void *)DATA_START_ADDRESS, sizeof(RfConfig));
    return config;
}
void Save_Paired_Address(uint8_t *rf_addr, uint8_t size)
{
    RfConfig config = Load_Rf_Config();
    config.is_paired  = 1;
    memcpy(config.rf_addr, rf_addr, size);

    FLASH_Unlock_Fast();
    FLASH_ErasePage_Fast(DATA_START_ADDRESS);

    u8 i;
    FLASH_BufReset();
    uint32_t buff[64];
    memset(buff, 0xFF, sizeof(buff));
    memcpy(buff, &config, sizeof(RfConfig));
   
    for (i = 0; i < 64; i++) {
        FLASH_BufLoad(DATA_START_ADDRESS + 4 * i, buff[i]);
    }
    FLASH_ProgramPage_Fast(DATA_START_ADDRESS);
    FLASH_Lock_Fast();
}
