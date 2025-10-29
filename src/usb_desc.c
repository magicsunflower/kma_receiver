/********************************** (C) COPYRIGHT *******************************
 * File Name          : composite_km_desc.h
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2023/04/06
 * Description        : All descriptors for the keyboard and mouse composite device.
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/


/*******************************************************************************/
/* Header File */
#include "usb_desc.h"

/*******************************************************************************/
/* Device Descriptor */
const uint8_t  USBD_DeviceDescriptor[USBD_SIZE_DEVICE_DESC] =
{
    0x12,                                               // bLength
    0x01,                                               // bDescriptorType (Device)
    0x10, 0x01,                                         // bcdUSB 1.10
    0x00,                                               // bDeviceClass (Use class information in the Interface Descriptors)
    0x00,                                               // bDeviceSubClass
    0x00,                                               // bDeviceProtocol
    DEF_USBD_UEP0_SIZE,                                 // bMaxPacketSize0
    (uint8_t)DEF_USB_VID, (uint8_t)(DEF_USB_VID >> 8),  // idVendor 
    (uint8_t)DEF_USB_PID, (uint8_t)(DEF_USB_PID >> 8),  // idProduct 
    0x00, DEF_IC_PRG_VER,                               // bcdDevice 1.00
    0x01,                                               // iManufacturer (String Index)
    0x02,                                               // iProduct (String Index)
    0x03,                                               // iSerialNumber (String Index)
    0x01,                                               // bNumConfigurations 1
};


/* USB Configration Descriptors */
const uint8_t  USBD_ConfigDescriptor[USBD_SIZE_CONFIG_DESC] = { 
    /* Configuration Descriptor */
    0x09,                           // bLength
    0x02,                           // bDescriptorType
    USBD_SIZE_CONFIG_DESC & 0xFF, USBD_SIZE_CONFIG_DESC >> 8, // wTotalLength
    0x02,                           // bNumInterfaces 接口总数
    0x01,                           // bConfigurationValue
    0x00,                           // iConfiguration (String Index)
    0xA0,                           // bmAttributes Remote Wakeup
    0xFA,                           // bMaxPower 500mA

	/* Interface Descriptor (Keyboard) */
    0x09,                           // bLength
    0x04,                           // bDescriptorType
    0x00,                           // bInterfaceNumber 接口的索引
    0x00,                           // bAlternateSetting
    0x01,                           // bNumEndpoints
    0x03,                           // bInterfaceClass
    0x01,                           // bInterfaceSubClass
    0x01,                           // bInterfaceProtocol: Keyboard
    0x00,                           // iInterface

    /* HID Descriptor (Keyboard) */
    0x09,                           // bLength
    0x21,                           // bDescriptorType
    0x11, 0x01,                     // bcdHID
    0x00,                           // bCountryCode
    0x01,                           // bNumDescriptors
    0x22,                           // bDescriptorType
    USBD_SIZE_REPORT_DESC_KB & 0xFF, USBD_SIZE_REPORT_DESC_KB >> 8, // wDescriptorLength

    /* Endpoint Descriptor (Keyboard) */
    0x07,                           // bLength
    0x05,                           // bDescriptorType
    0x81,                           // bEndpointAddress: IN Endpoint 1 端点地址
    0x03,                           // bmAttributes
    DEF_ENDP_SIZE_KB & 0xFF, DEF_ENDP_SIZE_KB >> 8, // wMaxPacketSize
    0x0A,

	/* Interface Descriptor (Mouse) */
    0x09,                           // bLength
    0x04,                           // bDescriptorType
    0x01,                           // bInterfaceNumber 接口索引
    0x00,                           // bAlternateSetting
    0x01,                           // bNumEndpoints 端点个数
    0x03,                           // bInterfaceClass
    0x01,                           // bInterfaceSubClass
    0x02,                           // bInterfaceProtocol: Mouse
    0x00,                           // iInterface

    /* HID Descriptor (Mouse) */
    0x09,                           // bLength
    0x21,                           // bDescriptorType
    0x10, 0x01,                     // bcdHID
    0x00,                           // bCountryCode
    0x01,                           // bNumDescriptors
    0x22,                           // bDescriptorType
    USBD_SIZE_REPORT_DESC_MS & 0xFF, USBD_SIZE_REPORT_DESC_MS >> 8, // wItemLength

    /* Endpoint Descriptor (Mouse) */
    0x07,                           // bLength
    0x05,                           // bDescriptorType
    0x82,                           // bEndpointAddress: IN Endpoint 2
    0x03,                           // bmAttributes
    DEF_ENDP_SIZE_RELATIVE_MS & 0xFF, DEF_ENDP_SIZE_RELATIVE_MS >> 8, // wMaxPacketSize
    0x01,                             // bInterval: 1mS               /* Endpoint Descriptor */
};


/* HID Report Descriptor */
const uint8_t USBD_KeyRepDesc[USBD_SIZE_REPORT_DESC_KB] =
{
    0x05, 0x01,                     // Usage Page (Generic Desktop)
    0x09, 0x06,                     // Usage (Keyboard)
    0xA1, 0x01,                     // Collection (Application)
    0x85, REPORT_ID_RAW_KEYBOARD,   // Report ID (1)
    // RAW Keyboard
    0x05, 0x07,                     // Usage Page (Key Codes)
    0x19, 0xE0,                     // Usage Minimum (224)
    0x29, 0xE7,                     // Usage Maximum (231)
    0x15, 0x00,                     // Logical Minimum (0)
    0x25, 0x01,                     // Logical Maximum (1)
    0x75, 0x01,                     // Report Size (1)
    0x95, 0x08,                     // Report Count (8)
    0x81, 0x02,                     // Input (Data,Variable,Absolute)
    0x95, 0x01,                     // Report Count (1)
    0x75, 0x08,                     // Report Size (8)
    0x81, 0x01,                     // Input (Constant)

    // LEDs (Caps Lock, Num Lock, Scroll Lock, Compose, Kana)
    0x05, 0x08,                     // Usage Page (LEDs)
    0x95, 0x03,                     // Report Count (3)
    0x75, 0x01,                     // Report Size (1)
    0x19, 0x01,                     // Usage Minimum (1)
    0x29, 0x03,                     // Usage Maximum (3)    
    0x91, 0x02,                     // Output (Data,Variable,Absolute)
    0x95, 0x05,                     // Report Count (5)
    0x75, 0x01,                     // Report Size (1)
    0x91, 0x01,                     // Output (Constant,Array,Absolute)

    0x05, 0x07,                     // Usage Page (Key Codes)
    0x19, 0x00,                     // Usage Minimum (0)
    0x29, 0x91,                     // Usage Maximum (145)
    0x26, 0xFF, 0x00,               // Logical Maximum (255)
    0x95, 0x06,                     // Report Count (6)
    0x75, 0x08,                     // Report Size (8)
    0x81, 0x00,                     // Input(Data,Array,Absolute)
    0xC0,                           // End Collection

    // MACRO KEYBOARD
    0x05, 0x01,                     // Usage Page (Generic Desktop)
    0x09, 0x06,                     // Usage (Keyboard)
    0xA1, 0x01,                     // Collection (Application)
    0x85, REPORT_ID_MACRO_KEYBOARD, // Report ID (2)
    0x05, 0x07,                     // Usage Page (Key Codes)
    0x19, 0xE0,                     // Usage Minimum (224)
    0x29, 0xE7,                     // Usage Maximum (231)
    0x15, 0x00,                     // Logical Minimum (0)
    0x25, 0x01,                     // Logical Maximum (1)
    0x75, 0x01,                     // Report Size (1)
    0x95, 0x08,                     // Report Count (8)
    0x81, 0x02,                     // Input (Data,Variable,Absolute)

    0x95, 0x01,                     // Report Count (1)
    0x75, 0x08,                     // Report Size (8)
    0x81, 0x01,                     // Input (Constant)
    
    // LEDs (Caps Lock, Num Lock, Scroll Lock, Compose, Kana)
    0x95, 0x05,       // Report Count (5)
    0x75, 0x01,       // Report Size (1)
    0x05, 0x08,       // Usage Page (LEDs)
    0x19, 0x01,       // Usage Minimum (1)
    0x29, 0x05,       // Usage Maximum (5)
    0x91, 0x02,       // Output (Data,Variable,Absolute)
    0x95, 0x01,       // Report Count (1)
    0x75, 0x03,       // Report Size (3)
    0x91, 0x01,       // Output (Constant)
    0x95, 0x06,       // Report Count (6)
    0x75, 0x08,       // Report Size (8)
    0x26, 0xFF, 0x00, // Logical Maximum (255)

    0x05, 0x07, // Usage Page (Key Codes)
    0x19, 0x00, // Usage Minimum (0)
    0x29, 0x91, // Usage Maximum (145)
    0x81, 0x00, // Input(Data,Array,Absolute)
    0xC0,       // End Collection

    // Multimedia Controls
    0x05, 0x0C,                 // Usage Page (Consumer Devices)
    0x09, 0x01,                 // Usage (Consumer Control)
    0xA1, 0x01,                 // Collection (Application)
    0x85, REPORT_ID_MULTIMEDIA, // Report ID (3)

    0x19, 0x00,        //   Usage Minimum (Unassigned)
    0x2A, 0xFF, 0x03,  //   Usage Maximum (0x03FF)
    0x15, 0x00,        //   Logical Minimum (0)
    0x26, 0xFF, 0x03,  //   Logical Maximum (1023)
    0x95, 0x01,        //   Report Count (1)
    0x75, 0x10,        //   Report Size (16)
    0x81, 0x00,        //   Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)

    0xC0 // End Collection (Multimedia Controls)
};
const uint8_t USBD_MouseRepDesc[USBD_SIZE_REPORT_DESC_MS] =
{
   // Relative Mouse Report Descriptor with Report ID 1 (modified Report ID)
    0x05, 0x01,                     // Usage Page (Generic Desktop)
    0x09, 0x02,                     // Usage (Mouse)
    0xA1, 0x01,                     // Collection (Application)
    0x85, REPORT_ID_RELATIVE_MOUSE, // Report ID (1)

    // Button
    0x05, 0x09, // Usage Page (Button)
    0x19, 0x01, // Usage Minimum (Button 1)
    0x29, 0x08, // Usage Maximum (Button 8)
    0x15, 0x00, // Logical Minimum (0)
    0x25, 0x01, // Logical Maximum (1)
    0x75, 0x01, // Report Size (1)
    0x95, 0x08, // Report Count (8)
    0x81, 0x02, // Input (Constant,Array,Absolute)

    // Movement
    0x05, 0x01,        //     Usage Page (Generic Desktop Ctrls)
    0x09, 0x30,        //     Usage (X)
    0x09, 0x31,        //     Usage (Y)
    0x16, 0x00, 0x80,  //     Logical Minimum (-32768)
    0x26, 0xFF, 0x7F,  //     Logical Maximum (32767)
    0x75, 0x10,        //     Report Size (16)
    0x95, 0x02,        //     Report Count (2)
    0x81, 0x06,        //     Input (Data,Var,Rel,No Wrap,Linear,Preferred State,No Null Position)
    0x09, 0x38,        //     Usage (Wheel)
    0x15, 0x81,        //     Logical Minimum (-127)
    0x25, 0x7F,        //     Logical Maximum (127)
    0x75, 0x08,        //     Report Size (8)
    0x95, 0x01,        //     Report Count (1)
    0x81, 0x06,        //     Input (Data,Var,Rel,No Wrap,Linear,Preferred State,No Null Position)
    0xC0, // End Collection

    // Absolute Mouse Report Descriptor with Report ID 2 (modified Report ID)
    0x05, 0x01,                     // Usage Page (Generic Desktop)
    0x09, 0x02,                     // Usage (Mouse)
    0xA1, 0x01,                     // Collection (Application)
    0x85, REPORT_ID_ABSOLUTE_MOUSE, // Report ID (2) - Different from Report ID 1

    // X and Y
    0x05, 0x01,       // Usage Page (Generic Desktop)
    0x09, 0x30,        //     Usage (X)
    0x15, 0x00,        //     Logical Minimum (0)
    0x26, 0xFF, 0x0F,  //     Logical Maximum (4095)
    0x75, 0x0D,        //     Report Size (13)
    0x95, 0x01,        //     Report Count (1)
    0x81, 0x02,        //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x75, 0x03,        //     Report Size (3)
    0x95, 0x01,        //     Report Count (1)
    0x81, 0x03,        //     Input (Const,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x09, 0x31,        //     Usage (Y)
    0x15, 0x00,        //     Logical Minimum (0)
    0x26, 0xFF, 0x0F,  //     Logical Maximum (4095)
    0x75, 0x0D,        //     Report Size (13)
    0x95, 0x01,        //     Report Count (1)
    0x81, 0x02,        //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x75, 0x03,        //     Report Size (3)
    0x95, 0x01,        //     Report Count (1)
    0x81, 0x03,        //     Input (Const,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)

    0xC0, // End Collection

};

/* Language Descriptor */
const uint8_t  USBD_StringLangDesc[DEF_USBD_LANG_DESC_LEN] =
{
    DEF_USBD_LANG_DESC_LEN, 0x03, 0x09, 0x04
};

/* Manufacturer Descriptor */
const uint8_t  USBD_ManuInfo[DEF_USBD_MANU_DESC_LEN] =
{
    DEF_USBD_MANU_DESC_LEN, 0x03, 'M', 0, 'i', 0, 'n', 0, 'd', 0, 'F', 0, 'i', 0, 'n', 0, 'g', 0, 'e', 0, 'r', 0
};

/* Product Information */
const uint8_t USBD_StringProduct[DEF_USBD_PROD_DESC_LEN] =
{
    DEF_USBD_PROD_DESC_LEN, 0x03, 'K', 0, 'M', 0, 'A', 0, '_', 0, 'R', 0, 'E', 0, 'C', 0, 'E', 0, 'I', 0, 'V', 0, 'E', 0, 'R', 0
};

/* Serial Number Information */
const uint8_t USBD_StringSerialNum[DEF_USBD_SN_DESC_LEN] =
{
    DEF_USBD_SN_DESC_LEN, 0x03, '1', 0, '2', 0, '3', 0, '4', 0, '5', 0, '6', 0, '7', 0, '8', 0, '9', 0, '0', 0
};
