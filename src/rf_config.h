#ifndef __RF_CONFIG_H__
#define __RF_CONFIG_H__
#include "ch32x035.h"
#include <string.h>
#include <stdint.h>
typedef struct __attribute__((packed)) _RfConfig {
    // 前四个字节原样保留
    uint32_t reserved1[4];
    // 是否已配对，已配对为1，未配对为其它值
    uint8_t is_paired;
    // 配对后的地址
    uint8_t rf_addr[5];
} RfConfig;

RfConfig Load_Rf_Config();
/**
 * @brief 
 * @param size 长度为5
 */
void Save_Paired_Address(uint8_t *rf_addr, uint8_t size);

#endif // !__RF_CONFIG_H__
