#ifndef __NRF2401L__
#include <ch32x035.h>
#include <stdint.h>

/* --- 使用你上传的原有引脚定义（如需修改，请在这里调整） --- */
/* 如果你的工程里已经定义了这些宏，可以直接使用原来的 nrf2401.h 中的定义 */
#define NRF24L01_SPIx                                 SPI1
/* SPI2 时钟开启（CH32 风格）在实现里会调用 RCC_APB1PeriphClockCmd */
#define NRF24L01_SPIx_RCC_CLK                       RCC_APB2Periph_SPI1

/* GPIO 引脚（保留原来文件中的定义）*/
#define NRF24L01_SPI_GPIO_PORT                        GPIOA
#define NRF24L01_SPI_SCK_PIN                          GPIO_Pin_5
#define NRF24L01_SPI_MISO_PIN                         GPIO_Pin_6
#define NRF24L01_SPI_MOSI_PIN                         GPIO_Pin_7

#define NRF24L01_SPI_CS_PORT                          GPIOA
#define NRF24L01_SPI_CS_PIN                           GPIO_Pin_4
#define NRF24L01_SPI_CS_ENABLE()                      GPIO_ResetBits(NRF24L01_SPI_CS_PORT, NRF24L01_SPI_CS_PIN)
#define NRF24L01_SPI_CS_DISABLE()                     GPIO_SetBits(NRF24L01_SPI_CS_PORT, NRF24L01_SPI_CS_PIN)

#define NRF24L01_CE_PORT                              GPIOA
#define NRF24L01_CE_PIN                               GPIO_Pin_1
#define NRF24L01_CE_LOW()                             GPIO_ResetBits(NRF24L01_CE_PORT, NRF24L01_CE_PIN)
#define NRF24L01_CE_HIGH()                            GPIO_SetBits(NRF24L01_CE_PORT, NRF24L01_CE_PIN)

#define NRF24L01_IRQ_PORT                             GPIOA
#define NRF24L01_IRQ_PIN                              GPIO_Pin_3
#define NRF24L01_IRQ_PIN_READ()                       GPIO_ReadInputDataBit(NRF24L01_IRQ_PORT, NRF24L01_IRQ_PIN)

/* widths */
#define TX_ADR_WIDTH    5
#define RX_ADR_WIDTH    5
#define TX_PLOAD_WIDTH  32
#define RX_PLOAD_WIDTH  32

/* commands / regs (和你原来的一致) */
#define NRF_READ_REG      0x00
#define NRF_WRITE_REG     0x20
#define RD_RX_PLOAD       0x61
#define WR_TX_PLOAD       0xA0
#define FLUSH_TX          0xE1
#define FLUSH_RX          0xE2
#define REUSE_TX_PL       0xE3
#define NOP               0xFF

#define CONFIG            0x00
#define EN_AA             0x01
#define EN_RXADDR         0x02
#define SETUP_AW          0x03
#define SETUP_RETR        0x04
#define RF_CH             0x05
#define RF_SETUP          0x06
#define STATUS            0x07
#define OBSERVE_TX        0x08
#define CD                0x09
#define RX_ADDR_P0        0x0A
#define TX_ADDR           0x10
#define RX_PW_P0          0x11
#define NRF_FIFO_STATUS   0x17

#define MAX_TX            0x10
#define TX_OK             0x20
#define RX_OK             0x40

// 对内接口
void NRF24L01_SPI_Init(void);
uint8_t NRF24L01_Write_Buf(uint8_t reg, uint8_t *pBuf, uint8_t len);
uint8_t NRF24L01_Read_Buf(uint8_t reg, uint8_t *pBuf, uint8_t len);
uint8_t NRF24L01_Read_Reg(uint8_t reg);
uint8_t NRF24L01_Write_Reg(uint8_t reg, uint8_t value);

/* API */
void NRF24L01_Init(void);
void NRF24L01_RX_Mode(void);
void NRF24L01_TX_Mode(void);
uint8_t NRF24L01_Check(void);
uint8_t NRF24L01_TxPacket(uint8_t *txbuf);
uint8_t NRF24L01_RxPacket(uint8_t *rxbuf);
void NRF_LowPower_Mode(void);

#endif // !__NRF2401L__
