/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2023/12/26
 * Description        : Main program body.
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for 
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/

/*
 * @Note
 * Composite Keyboard and Mouse Example:
 * This example uses PB12-PB15 and PA4-PA7 to simulate keyboard key pressing and mouse
 * movement respectively, active low.
 * At the same time, it also uses USART2(PA3) to receive the specified data sent from
 * the host to simulate the pressing and releasing of the following specific keyboard
 * keys. Data is sent in hexadecimal format and 1 byte at a time.
 * 'W' -> 0x1A
 * 'A' -> 0x04
 * 'S' -> 0x16
 * 'D' -> 0x07
 */

#include <ch32x035_usbfs_device.h>
#include "debug.h"
#include "usbd_composite_km.h"
#include "nrf24l01.h"



/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    SystemCoreClockUpdate();
    Delay_Init();
    USART_Printf_Init(115200);
    printf("SystemClk:%d\r\n", SystemCoreClock);
    printf("ChipID:%08x\r\n", DBGMCU_GetCHIPID() );

    NRF24L01_Init();
    if (NRF24L01_Check() != 0) {
        printf("NRF24L01_Check failed\r\n");
    } else {
        printf("NRF24L01_Check successfully\r\n");
        NRF24L01_RX_Mode();
    }

    /* Usb Init */
    USBFS_RCC_Init( );
    USBFS_Device_Init( ENABLE , PWR_VDD_SupplyVoltage());
    // USB_Sleep_Wakeup_CFG( );
    uint8_t data[30]={0};
    while(1)
    {
        /* Determine if enumeration is complete, perform data transfer if completed */
        if(USBFS_DevEnumStatus)
        {
            if (USBFS_DevEnumStatus) {
                if (NRF24L01_RxPacket(data) == 0) {
                    printf("rx:%02x %02x", data[0], data[1]);
                    for (int8_t index = 0; index < data[1]; index++) {
                        printf(" %02x", data[index + 2]);
                    }
                    printf("\r\n");
                    uint8_t status =0;
                    switch (data[0]) {
                        case RF_PREFIX_RELATIVE_MOUSE:
                            data[1] = REPORT_ID_RELATIVE_MOUSE;
                            status  = USBFS_Endp_DataUp(DEF_UEP2, &data[1], DEF_ENDP_SIZE_RELATIVE_MS, DEF_UEP_CPY_LOAD);
                            break;
                        case RF_PREFIX_ABSOLUTE_MOUSE:
                            data[1] = REPORT_ID_ABSOLUTE_MOUSE;
                            status  = USBFS_Endp_DataUp(DEF_UEP2, &data[1], DEF_ENDP_SIZE_ABSOLUTE_MS, DEF_UEP_CPY_LOAD);
                            break;
                        case RF_PREFIX_RAW_KEYBOARD:
                            data[1] = REPORT_ID_RAW_KEYBOARD;
                            status  = USBFS_Endp_DataUp(DEF_UEP1, &data[1], DEF_ENDP_SIZE_KB, DEF_UEP_CPY_LOAD);
                            break;
                        case RF_PREFIX_MACRO_KEYBOARD:
                            data[1] = REPORT_ID_MACRO_KEYBOARD;
                            status  = USBFS_Endp_DataUp(DEF_UEP1, &data[1], DEF_ENDP_SIZE_KB, DEF_UEP_CPY_LOAD);
                            break;
                        case RF_PREFIX_MULTIMEDIA:
                            data[1] = REPORT_ID_MULTIMEDIA;
                            status  = USBFS_Endp_DataUp(DEF_UEP1, &data[1], DEF_ENDP_SIZE_MULTIMEDIA, DEF_UEP_CPY_LOAD);
                            break;
                        default:
                            break;
                    }
                    printf("status:%d\r\n", status);
                }
            }
        }
    }
}
