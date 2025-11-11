#include <ch32x035_usbfs_device.h>
#include "debug.h"
#include "usbd_composite_km.h"
#include "nrf24l01.h"
#include "rf_config.h"


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
    printf("SystemClk:%u\r\n", (unsigned int)SystemCoreClock);
    printf("ChipID:%08x\r\n", (unsigned int)DBGMCU_GetCHIPID() );
   
    NRF24L01_Init();
    Delay_Ms(10); // 必须加延时，否则检查会失败
    if (NRF24L01_Check() != 0) {
        printf("NRF24L01_Check failed\r\n");
        return 0;
    } else {
        RfConfig config = Load_Rf_Config();
        if (config.is_paired) {
            NRF24L01_RX_Mode_By_Address(config.rf_addr, sizeof(config.rf_addr));
            printf("Paired address:");
            for (int index = 0; index < sizeof(config.rf_addr); index++) {
                printf("%02x ", config.rf_addr[index]);
            }
            printf("\r\n");
        } else {
            NRF24L01_Wait_Paired();
            printf("Waiting paired\r\n");
        }
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
                    // for (int8_t index = 0; index < data[1]; index++) {
                    //     printf(" %02x", data[index + 2]);
                    // }
                    printf("\r\n");
                    uint8_t status =0;
                    switch (data[0]) {
                        case RF_CMD_RELATIVE_MOUSE:
                            if (data[1] != DEF_ENDP_SIZE_RELATIVE_MS - 1) {
                                break;
                            }
                            data[1] = REPORT_ID_RELATIVE_MOUSE;
                            status  = USBFS_Endp_DataUp(DEF_UEP2, &data[1], DEF_ENDP_SIZE_RELATIVE_MS, DEF_UEP_CPY_LOAD);
                            break;
                        case RF_CMD_ABSOLUTE_MOUSE:
                            if (data[1] != DEF_ENDP_SIZE_ABSOLUTE_MS - 1) {
                                break;
                            }
                            data[1] = REPORT_ID_ABSOLUTE_MOUSE;
                            status  = USBFS_Endp_DataUp(DEF_UEP2, &data[1], DEF_ENDP_SIZE_ABSOLUTE_MS, DEF_UEP_CPY_LOAD);
                            break;
                        case RF_CMD_RAW_KEYBOARD:
                            if (data[1] != DEF_ENDP_SIZE_KB - 1) {
                                break;
                            }
                            data[1] = REPORT_ID_RAW_KEYBOARD;
                            status  = USBFS_Endp_DataUp(DEF_UEP1, &data[1], DEF_ENDP_SIZE_KB, DEF_UEP_CPY_LOAD);
                            break;
                        case RF_CMD_MACRO_KEYBOARD:
                            if (data[1] != DEF_ENDP_SIZE_KB - 1) {
                                break;
                            }
                            data[1] = REPORT_ID_MACRO_KEYBOARD;
                            status  = USBFS_Endp_DataUp(DEF_UEP1, &data[1], DEF_ENDP_SIZE_KB, DEF_UEP_CPY_LOAD);
                            break;
                        case RF_CMD_MULTIMEDIA:
                            if (data[1] != DEF_ENDP_SIZE_MULTIMEDIA - 1) {
                                break;
                            }
                            data[1] = REPORT_ID_MULTIMEDIA;
                            status  = USBFS_Endp_DataUp(DEF_UEP1, &data[1], DEF_ENDP_SIZE_MULTIMEDIA, DEF_UEP_CPY_LOAD);
                            break;
                        case RF_CMD_PAIR_ADDR:
                            printf("Received paired addr:");
                            if (data[1] != 5) {
                                break;
                            }
                            RfConfig config = Load_Rf_Config();
                            if (config.is_paired) {
                                break;
                            }
                            for (int index = 0; index < 5; index++) {
                                printf("%02x ", data[2 + index]);
                            }
                            printf("\r\n");
                            Save_Paired_Address(&data[2], 5);
                            NRF24L01_RX_Mode_By_Address(&data[2], 5);
                            // NVIC_SystemReset();
                            break;
                        case RF_CMD_HOP_CHANNEL:
                            if (data[1] != 1) {
                                break;
                            }
                            NRF24L01_Hop_Channel(data[2]);
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
