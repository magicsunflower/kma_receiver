/********************************** (C) COPYRIGHT *******************************
 * File Name          : usbd_descriptor.h
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2023/04/06
 * Description        : All descriptors for the keyboard and mouse composite device.
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/


#ifndef __USBD_DESC_H
#define __USBD_DESC_H

/*******************************************************************************/
/* Header File */
#include "stdint.h"

/*******************************************************************************/
/* Macro Definition */

/* File Version */
#define DEF_FILE_VERSION              0x01

/* USB Device Info */
#define DEF_USB_VID                   0x1A86
#define DEF_USB_PID                   0xFE00

/* USB Device Descriptor, Device Serial Number(bcdDevice) */
#define DEF_IC_PRG_VER                DEF_FILE_VERSION

/* USB Device Endpoint Size */
#define DEF_USBD_UEP0_SIZE            64     /* usb hs/fs device end-point 0 size */
/* HS */
#define DEF_USBD_HS_PACK_SIZE         512    /* usb hs device max bluk/int pack size */
#define DEF_USBD_HS_ISO_PACK_SIZE     1024   /* usb hs device max iso pack size */
/* FS */
#define DEF_USBD_FS_PACK_SIZE         64     /* usb fs device max bluk/int pack size */
#define DEF_USBD_FS_ISO_PACK_SIZE     1023   /* usb fs device max iso pack size */
/* LS */
#define DEf_USBD_LS_UEP0_SIZE         8      /* usb ls device end-point 0 size */
#define DEF_USBD_LS_PACK_SIZE         64     /* usb ls device max int pack size */

/* USB Device Endpoint1-6 Size */
#define DEF_USB_EP1_FS_SIZE           DEF_USBD_FS_PACK_SIZE
#define DEF_USB_EP2_FS_SIZE           DEF_USBD_FS_PACK_SIZE
#define DEF_USB_EP3_FS_SIZE           DEF_USBD_FS_PACK_SIZE
#define DEF_USB_EP4_FS_SIZE           DEF_USBD_FS_PACK_SIZE
#define DEF_USB_EP5_FS_SIZE           DEF_USBD_FS_PACK_SIZE
#define DEF_USB_EP6_FS_SIZE           DEF_USBD_FS_PACK_SIZE
/* LS */
/* ... */

/******************************************************************************/
/* usb device Descriptor length, length of usb descriptors, if one descriptor not
 * exists , set the length to 0  */
#define USBD_SIZE_DEVICE_DESC             18
#define USBD_SIZE_CONFIG_DESC             59
#define USBD_SIZE_REPORT_DESC_KB          153
#define USBD_SIZE_REPORT_DESC_MS          104

#define DEF_USBD_UEP0_SIZE                64
#define DEF_ENDP_SIZE_KB                  9
#define DEF_ENDP_SIZE_RELATIVE_MS         7
#define DEF_ENDP_SIZE_ABSOLUTE_MS         5
#define DEF_ENDP_SIZE_MULTIMEDIA          3

#define DEF_USBD_LANG_DESC_LEN   0x04
#define DEF_USBD_MANU_DESC_LEN   0x16
#define DEF_USBD_PROD_DESC_LEN   0x1A
#define DEF_USBD_SN_DESC_LEN     0x16

#define REPORT_ID_RELATIVE_MOUSE          1
#define REPORT_ID_ABSOLUTE_MOUSE          2
#define REPORT_ID_RAW_KEYBOARD            1
#define REPORT_ID_MACRO_KEYBOARD          2
#define REPORT_ID_MULTIMEDIA              3

// 指令格式，指令类型（1Byte）+ 指令长度（1Byte）+ 指令内容（由指令长度内容得出）
#define RF_CMD_RELATIVE_MOUSE          0x01
#define RF_CMD_ABSOLUTE_MOUSE          0x02
#define RF_CMD_RAW_KEYBOARD            0x03
#define RF_CMD_MACRO_KEYBOARD          0x04
#define RF_CMD_MULTIMEDIA              0x05
// 配对指令
#define RF_CMD_PAIR_ADDR 0x10
// 跳频指令
#define RF_CMD_HOP_CHANNEL 0x12

/******************************************************************************/
/* external variables */
extern const uint8_t USBD_DeviceDescriptor[USBD_SIZE_DEVICE_DESC];
extern const uint8_t USBD_ConfigDescriptor[USBD_SIZE_CONFIG_DESC];

extern const uint8_t USBD_StringLangDesc[DEF_USBD_LANG_DESC_LEN];
extern const uint8_t USBD_ManuInfo[DEF_USBD_MANU_DESC_LEN];
extern const uint8_t USBD_StringProduct[DEF_USBD_PROD_DESC_LEN];
extern const uint8_t USBD_StringSerialNum[DEF_USBD_SN_DESC_LEN];

extern const uint8_t USBD_KeyRepDesc[USBD_SIZE_REPORT_DESC_KB];
extern const uint8_t USBD_MouseRepDesc[USBD_SIZE_REPORT_DESC_MS];


#endif /* USER_USB_DESC_H1_ */
