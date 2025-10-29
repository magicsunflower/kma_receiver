#include "nrf24l01.h"
#include "debug.h"

/* 发送与接收地址（示例） */
const uint8_t TX_ADDRESS[TX_ADR_WIDTH] = {0x34, 0x43, 0x10, 0x10, 0x01};
const uint8_t RX_ADDRESS[RX_ADR_WIDTH] = {0x34, 0x43, 0x10, 0x10, 0x01};

/* 本地：设置 SPI1 的速度（通过 BR[2:0]） */
static void SPIx_SetSpeed(uint16_t prescaler)
{
    // uint16_t tmp;
    // tmp = SPI2->CTLR1;
    // tmp &= ~(0x7 << 3);         // 清除原 BR[5:3] 位
    // tmp |= prescaler;           // 例如：SPI_BaudRatePrescaler_8 即 (0x2 << 3)
    // SPI1->CTLR1 = tmp;
}

/* CH32 SPI 读写一个字节函数（阻塞） */
uint8_t SPIx_ReadWriteByte(SPI_TypeDef *SPIx, uint8_t TxData)
{
    uint8_t i = 0;
    while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET) {
        i++;
        if (i > 200)
            return 0;
    }

    SPI_I2S_SendData(SPIx, TxData);
    i = 0;

    while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET) {
        i++;
        if (i > 200)
            return 0;
    }
    return SPI_I2S_ReceiveData(SPIx);
}

/* 初始化 SPI1 与用于 NRF 的 GPIO */
void NRF24L01_SPI_Init(void)
{
    SPI_InitTypeDef SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    /* 使能 GPIOA (SCK/MISO/MOSI) 与 SPI1 时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | NRF24L01_SPIx_RCC_CLK, ENABLE);

    /* SPI SCK/MOSI 输出配置 */
    GPIO_InitStructure.GPIO_Pin   = NRF24L01_SPI_SCK_PIN | NRF24L01_SPI_MOSI_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_Init(NRF24L01_SPI_GPIO_PORT, &GPIO_InitStructure);

    /* MISO 输入浮空 */
    GPIO_InitStructure.GPIO_Pin  = NRF24L01_SPI_MISO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(NRF24L01_SPI_GPIO_PORT, &GPIO_InitStructure);

    /* CS, CE as push-pull output */
    GPIO_InitStructure.GPIO_Pin  = NRF24L01_SPI_CS_PIN | NRF24L01_CE_PIN | NRF24L01_IRQ_PIN; // IRQ 设置为输入后覆盖
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(NRF24L01_SPI_CS_PORT, &GPIO_InitStructure);

    /* IRQ 引脚作为输入（覆盖上面的输出设定） */
    GPIO_InitStructure.GPIO_Pin  = NRF24L01_IRQ_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(NRF24L01_IRQ_PORT, &GPIO_InitStructure);

    /* 释放 CS，CE 低 */
    NRF24L01_SPI_CS_DISABLE();
    NRF24L01_CE_LOW();

    /* SPI1 配置：主机模式，全双工，8 位，CPOL=0, CPHA=0, MSB first */
    SPI_InitStructure.SPI_Direction         = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode              = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize          = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL              = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA              = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS               = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8; // 默认，之后可用 SPI1_SetSpeed 调整
    SPI_InitStructure.SPI_FirstBit          = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial     = 7;

    SPI_Init(NRF24L01_SPIx, &SPI_InitStructure);
    SPI_Cmd(NRF24L01_SPIx, ENABLE);
}

/* 初始化 nRF24L01（只做 SPI/GPIO 初始化） */
void NRF24L01_Init(void)
{
    NRF24L01_SPI_Init();
    NRF24L01_CE_LOW();
    NRF24L01_SPI_CS_DISABLE();
}

/* 检测 nRF24L01 是否存在 */
uint8_t NRF24L01_Check(void)
{
    uint8_t buf[5] = {0xA5, 0xA5, 0xA5, 0xA5, 0xA5};
    uint8_t readbuf[5];
    uint8_t i;

    SPIx_SetSpeed(SPI_BaudRatePrescaler_8);
    NRF24L01_Write_Buf(NRF_WRITE_REG + TX_ADDR, buf, 5);
    NRF24L01_Read_Buf(TX_ADDR, readbuf, 5);

    for (i = 0; i < 5; i++) {
        if (readbuf[i] != 0xA5) break;
    }
    if (i != 5) return 1;
    return 0;
}

/********************************************************
函数功能：写寄存器的值（单字节）
入口参数：reg:寄存器映射地址（格式：WRITE_REG｜reg）
                    value:寄存器的值
返回  值：状态寄存器的值
*********************************************************/
uint8_t NRF24L01_Write_Reg(uint8_t reg, uint8_t value)
{
    uint8_t status;
    NRF24L01_SPI_CS_ENABLE();
    status = SPIx_ReadWriteByte(NRF24L01_SPIx, reg);
    SPIx_ReadWriteByte(NRF24L01_SPIx, value);
    NRF24L01_SPI_CS_DISABLE();
    return status;
}

/********************************************************
函数功能：读取寄存器的值（单字节）
入口参数：reg:寄存器映射地址（格式：READ_REG｜reg）
返回  值：寄存器值
*********************************************************/
uint8_t NRF24L01_Read_Reg(uint8_t reg)
{
    uint8_t reg_val;
    NRF24L01_SPI_CS_ENABLE();
    SPIx_ReadWriteByte(NRF24L01_SPIx, reg);
    reg_val = SPIx_ReadWriteByte(NRF24L01_SPIx, 0xFF);
    NRF24L01_SPI_CS_DISABLE();
    return reg_val;
}

/********************************************************
函数功能：读取寄存器的值（单字节）
入口参数：reg:寄存器映射地址（格式：READ_REG｜reg）
返回  值：寄存器值
*********************************************************/
uint8_t NRF24L01_Read_Buf(uint8_t reg, uint8_t *pBuf, uint8_t len)
{
    uint8_t status;
    uint8_t i;
    NRF24L01_SPI_CS_ENABLE();
    status = SPIx_ReadWriteByte(NRF24L01_SPIx, reg);
    for (i = 0; i < len; i++) {
        pBuf[i] = SPIx_ReadWriteByte(NRF24L01_SPIx, 0xFF);
    }
    NRF24L01_SPI_CS_DISABLE();
    return status;
}

/********************************************************
函数功能：写寄存器的值（多字节）
入口参数：reg:寄存器映射地址（格式：WRITE_REG｜reg）
                    pBuf:写数据首地址
                    bytes:写数据字节数
返回  值：状态寄存器的值
*********************************************************/
uint8_t NRF24L01_Write_Buf(uint8_t reg, uint8_t *pBuf, uint8_t len)
{
    uint8_t status;
    uint8_t i;
    NRF24L01_SPI_CS_ENABLE();
    status = SPIx_ReadWriteByte(NRF24L01_SPIx, reg);
    for (i = 0; i < len; i++) {
        SPIx_ReadWriteByte(NRF24L01_SPIx, pBuf[i]);
    }
    NRF24L01_SPI_CS_DISABLE();
    return status;
}

/********************************************************
函数功能：BK2425接收模式初始化
入口参数：无
返回  值：无
*********************************************************/
void NRF24L01_RX_Mode(void)
{
    NRF24L01_CE_LOW();
    NRF24L01_Write_Buf(NRF_WRITE_REG + RX_ADDR_P0, (uint8_t *)RX_ADDRESS, RX_ADR_WIDTH); // 接收设备接收通道0使用和发送设备相同的发送地址
    NRF24L01_Write_Reg(NRF_WRITE_REG + EN_AA, 0x01);                                     // 使能接收通道0自动应答
    NRF24L01_Write_Reg(NRF_WRITE_REG + EN_RXADDR, 0x01);                                 // 使能接收通道0
    NRF24L01_Write_Reg(NRF_WRITE_REG + RF_CH, 40);                                       // 选择射频通道0x40
    NRF24L01_Write_Reg(NRF_WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH);                        // 接收通道0选择和发送通道相同有效数据宽度
    NRF24L01_Write_Reg(NRF_WRITE_REG + RF_SETUP, 0x0f);                                  // 数据传输率2Mbps，发射功率7dBm
    NRF24L01_Write_Reg(NRF_WRITE_REG + CONFIG, 0x0f);                                    // CRC使能，16位CRC校验，上电，接收模式
    NRF24L01_Write_Reg(NRF_WRITE_REG + STATUS, 0xff);                                    // 清除所有的中断标志位
    NRF24L01_CE_HIGH();
    Delay_Ms(1);
}

/********************************************************
函数功能：BK2425发送模式初始化
入口参数：无
返回  值：无
*********************************************************/
void NRF24L01_TX_Mode(void)
{
    NRF24L01_CE_LOW();
    NRF24L01_Write_Buf(NRF_WRITE_REG + TX_ADDR, (uint8_t *)TX_ADDRESS, TX_ADR_WIDTH);    // 写入发送地址
    NRF24L01_Write_Buf(NRF_WRITE_REG + RX_ADDR_P0, (uint8_t *)RX_ADDRESS, RX_ADR_WIDTH); // 为了应答接收设备，接收通道0地址和发送地址相同
    NRF24L01_Write_Reg(NRF_WRITE_REG + EN_AA, 0x01);                                     // 使能接收通道0自动应答
    NRF24L01_Write_Reg(NRF_WRITE_REG + EN_RXADDR, 0x01);                                 // 使能接收通道0
    NRF24L01_Write_Reg(NRF_WRITE_REG + SETUP_RETR, 0xff);                                //
    NRF24L01_Write_Reg(NRF_WRITE_REG + RF_CH, 40);                                       // 选择射频通道0x40
    NRF24L01_Write_Reg(NRF_WRITE_REG + RF_SETUP, 0x0f);                                  // 数据传输率2Mbps，发射功率7dBm
    NRF24L01_Write_Reg(NRF_WRITE_REG + CONFIG, 0x0e);                                    //  CRC使能，16位CRC校验，上电
    NRF24L01_CE_HIGH();
    Delay_Ms(1);
}

/********************************************************
函数功能：发送一个数据包
入口参数：txbuf:要发送的数据
返回  值：0x10:达到最大重发次数，发送失败
          0x20:发送成功
          0xff:发送失败
*********************************************************/
uint8_t NRF24L01_TxPacket(uint8_t *txbuf)
{
    uint8_t sta;
    SPIx_SetSpeed(SPI_BaudRatePrescaler_8);
    NRF24L01_CE_LOW();                                      // CE拉低，使能SI24R1配置
    NRF24L01_Write_Buf(WR_TX_PLOAD, txbuf, TX_PLOAD_WIDTH); // 写数据到TX FIFO,32个字节
    NRF24L01_CE_HIGH();                                     // CE置高，使能发送

    /* 等待 IRQ 引脚变低（外部中断或轮询） */
    while (NRF24L01_IRQ_PIN_READ() != 0); // 等待发送完成

    sta = NRF24L01_Read_Reg(STATUS);                 // 读取状态寄存器的值
    NRF24L01_Write_Reg(NRF_WRITE_REG + STATUS, sta); // 清除TX_OK或MAX_TX中断标志
    if (sta & MAX_TX) {                              // 达到最大重发次数
        NRF24L01_Write_Reg(FLUSH_TX, 0xff);          // 清除TX FIFO寄存器
        return MAX_TX;
    }
    if (sta & TX_OK) { // 发送完成
        return TX_OK;
    }
    return 0xff; // 发送失败
}

/********************************************************
函数功能：读取接收数据
入口参数：rxbuf:接收数据存放首地址
返回  值：0:接收到数据
          1:没有接收到数据
*********************************************************/
uint8_t NRF24L01_RxPacket(uint8_t *rxbuf)
{
    uint8_t sta;
    SPIx_SetSpeed(SPI_BaudRatePrescaler_8);
    sta = NRF24L01_Read_Reg(STATUS);                           // 读取状态寄存器的值
    NRF24L01_Write_Reg(NRF_WRITE_REG + STATUS, sta);           // 清除RX_DS中断标志
    if (sta & RX_OK) {                                         // 接收到数据
        NRF24L01_Read_Buf(RD_RX_PLOAD, rxbuf, RX_PLOAD_WIDTH); // 读取数据
        NRF24L01_Write_Reg(FLUSH_RX, 0xff);                    // 清除RX FIFO寄存器
        return 0;
    }
    return 1; // 没收到任何数据
}

/* 进入掉电模式 */
void NRF_LowPower_Mode(void)
{
    NRF24L01_CE_LOW();
    NRF24L01_Write_Reg(NRF_WRITE_REG + CONFIG, 0x00); // POWER DOWN
}