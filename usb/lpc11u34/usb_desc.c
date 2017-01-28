/**
 * @file    cdc_desc.h
 * @author  Giedrius Medzevicius <giedrius@8devices.com>
 *
 * @section LICENSE
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2013 UAB 8devices
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * @section DESCRIPTION
 *
 * USB descriptors
 *
 */

#include <string.h>
#include "mw_usbd.h"
#include "mw_usbd_rom_api.h"
#include "app_usbd_cfg.h"

/* USB Standard Device Descriptor */
__attribute__ ((aligned(4))) const uint8_t DeviceDescriptor[] = {
    USB_DEVICE_DESC_SIZE,               /* bLength */
    USB_DEVICE_DESCRIPTOR_TYPE,         /* bDescriptorType */
    WBVAL(0x0200), /* 2.0 */            /* bcdUSB */
    0xEF,                               /* bDeviceClass Misc */
    0x02,                               /* bDeviceSubClass */
    0x01,                               /* bDeviceProtocol - Interface Association Descriptor */
    USB_MAX_PACKET0,                    /* bMaxPacketSize0 */
    WBVAL(0x1209),                      /* idVendor */
    WBVAL(0x2552),                      /* idProduct */
    WBVAL(0x0100), /* 1.00 */           /* bcdDevice */
    0x01,                               /* iManufacturer */
    0x02,                               /* iProduct */
    0x03,                               /* iSerialNumber */
    0x01                                /* bNumConfigurations: one possible configuration*/
};

/* USB Standard Device Qualifier Descriptor */
__attribute__ ((aligned(4))) const uint8_t DeviceQualifierDescriptor[] = {
    USB_DEVICE_QUALI_SIZE,              /* bLength */
    USB_DEVICE_QUALIFIER_DESCRIPTOR_TYPE, /* bDescriptorType */
    WBVAL(0x0200),                      /* bcdUSB */
    0xEF,                               /* bDeviceClass Misc */
    0x02,                               /* bDeviceSubClass */
    0x01,                               /* bDeviceProtocol - Interface Association Descriptor */
    USB_MAX_PACKET0,                    /* bMaxPacketSize0 */
    0x01,                               /* bNumConfigurations: one possible configuration*/
    0x00                                /* bReserved */
};

/* USB Configuration Descriptor */
/*   All Descriptors (Configuration, Interface, Endpoint, Class, Vendor */
__attribute__ ((aligned(4))) const uint8_t ConfigDescriptor[] = {
    /* Configuration 1 */
    USB_CONFIGUARTION_DESC_SIZE,        /* bLength */
    USB_CONFIGURATION_DESCRIPTOR_TYPE,  /* bDescriptorType */
    WBVAL(                              /* wTotalLength */
            1*USB_CONFIGUARTION_DESC_SIZE +

            1*0x08 +                    /* IAD size */
            1*USB_INTERFACE_DESC_SIZE + /* communication interface */
            0x0013 +                    /* CDC functions */
            1*USB_ENDPOINT_DESC_SIZE +  /* interrupt endpoint */

            1*USB_INTERFACE_DESC_SIZE + /* data0 interface */
            2*USB_ENDPOINT_DESC_SIZE    /* bulk endpoints */
         ),
    0x02,                               /* bNumInterfaces */
    0x01,                               /* bConfigurationValue: 0x01 is used to select this configuration */
    0x00,                               /* iConfiguration: no string to describe this configuration */
    USB_CONFIG_BUS_POWERED,             /* bmAttributes */
    USB_CONFIG_POWER_MA(50),            /* bMaxPower, device power consumption is 100 mA */


    /* IAD 1 Descriptor */
    0x08,                               /* sizeof(USB_IAD_DSC) */
    0x0B,                               /* Interface association descriptor type */
    USB_CDC_CMDIF_CIF_NUM,              /* The first associated interface */
    0x02,                               /* Number of contiguous associated interfaces */
    CDC_COMMUNICATION_INTERFACE_CLASS,  /* bInterfaceClass: Communication Interface Class */
    CDC_ABSTRACT_CONTROL_MODEL,         /* bInterfaceSubClass: Abstract Control Model */
    0x00,                               /* bInterfaceProtocol: no protocol used */
    0x04,                               /* iInterface: */

    /* Interface 0, Alternate Setting 0, Communication class interface descriptor */
    USB_INTERFACE_DESC_SIZE,            /* bLength */
    USB_INTERFACE_DESCRIPTOR_TYPE,      /* bDescriptorType */
    USB_CDC_CMDIF_CIF_NUM,              /* bInterfaceNumber: Number of Interface */
    0x00,                               /* bAlternateSetting: Alternate setting */
    0x01,                               /* bNumEndpoints: One endpoint used */
    CDC_COMMUNICATION_INTERFACE_CLASS,  /* bInterfaceClass: Communication Interface Class */
    CDC_ABSTRACT_CONTROL_MODEL,         /* bInterfaceSubClass: Abstract Control Model */
    0x00,                               /* bInterfaceProtocol: no protocol used */
    0x04,                               /* iInterface: */
    /*Header Functional Descriptor*/
    0x05,                               /* bLength: Endpoint Descriptor size */
    CDC_CS_INTERFACE,                   /* bDescriptorType: CS_INTERFACE */
    CDC_HEADER,                         /* bDescriptorSubtype: Header Func Desc */
    WBVAL(CDC_V1_10), /* 1.10 */        /* bcdCDC */
    /*Call Management Functional Descriptor*/
    0x05,                               /* bFunctionLength */
    CDC_CS_INTERFACE,                   /* bDescriptorType: CS_INTERFACE */
    CDC_CALL_MANAGEMENT,                /* bDescriptorSubtype: Call Management Func Desc */
    0x00,                               /* bmCapabilities: device handles call management */
    USB_CDC_CMDIF_DIF_NUM,              /* bDataInterface: CDC data IF ID */
    /*Abstract Control Management Functional Descriptor*/
    0x04,                               /* bFunctionLength */
    CDC_CS_INTERFACE,                   /* bDescriptorType: CS_INTERFACE */
    CDC_ABSTRACT_CONTROL_MANAGEMENT,    /* bDescriptorSubtype: Abstract Control Management desc */
    0x02,                               /* bmCapabilities: SET_LINE_CODING, GET_LINE_CODING, SET_CONTROL_LINE_STATE supported */
    /*Union Functional Descriptor*/
    0x05,                               /* bFunctionLength */
    CDC_CS_INTERFACE,                   /* bDescriptorType: CS_INTERFACE */
    CDC_UNION,                          /* bDescriptorSubtype: Union func desc */
    USB_CDC_CMDIF_CIF_NUM,              /* bMasterInterface: Communication class interface is master */
    USB_CDC_CMDIF_DIF_NUM,              /* bSlaveInterface0: Data class interface is slave 0 */
    /*Endpoint 1 Descriptor*/
    USB_ENDPOINT_DESC_SIZE,             /* bLength */
    USB_ENDPOINT_DESCRIPTOR_TYPE,       /* bDescriptorType */
    USB_CDC_CMDIF_EP_INT_IN,            /* bEndpointAddress */
    USB_ENDPOINT_TYPE_INTERRUPT,        /* bmAttributes */
    WBVAL(0x0010),                      /* wMaxPacketSize */
    0x02,          /* 2ms */            /* bInterval */

    /* Interface 1, Alternate Setting 0, Data class interface descriptor*/
    USB_INTERFACE_DESC_SIZE,            /* bLength */
    USB_INTERFACE_DESCRIPTOR_TYPE,      /* bDescriptorType */
    USB_CDC_CMDIF_DIF_NUM,              /* bInterfaceNumber: Number of Interface */
    0x00,                               /* bAlternateSetting: no alternate setting */
    0x02,                               /* bNumEndpoints: two endpoints used */
    CDC_DATA_INTERFACE_CLASS,           /* bInterfaceClass: Data Interface Class */
    0x00,                               /* bInterfaceSubClass: no subclass available */
    0x00,                               /* bInterfaceProtocol: no protocol used */
    0x04,                               /* iInterface: */
    /* Endpoint, EP2 Bulk Out */
    USB_ENDPOINT_DESC_SIZE,             /* bLength */
    USB_ENDPOINT_DESCRIPTOR_TYPE,       /* bDescriptorType */
    USB_CDC_CMDIF_EP_BULK_OUT,          /* bEndpointAddress */
    USB_ENDPOINT_TYPE_BULK,             /* bmAttributes */
    WBVAL(USB_HS_MAX_BULK_PACKET),      /* wMaxPacketSize */
    0x00,                               /* bInterval: ignore for Bulk transfer */
    /* Endpoint, EP2 Bulk In */
    USB_ENDPOINT_DESC_SIZE,             /* bLength */
    USB_ENDPOINT_DESCRIPTOR_TYPE,       /* bDescriptorType */
    USB_CDC_CMDIF_EP_BULK_IN,           /* bEndpointAddress */
    USB_ENDPOINT_TYPE_BULK,             /* bmAttributes */
    WBVAL(USB_HS_MAX_BULK_PACKET),      /* wMaxPacketSize */
    0x00,                               /* bInterval: ignore for Bulk transfer */
    /* Terminator */
    0                                   /* bLength */
};

/* USB String Descriptor (optional) */
__attribute__ ((aligned(4))) const uint8_t StringDescriptor[] = {
    /* Index 0x00: LANGID Codes */
    0x04,                              /* bLength */
    USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
    WBVAL(0x0409), /* US English */    /* wLANGID */
    /* Index 0x01: Manufacturer */
    (4*2 + 2),                         /* bLength (8 Char + Type + length) */
    USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
    'W', 0,
    'e', 0,
    'I', 0,
    'O', 0,
    /* Index 0x02: Product */
    (15*2 + 2),                        /* bLength (21 Char + Type + length) */
    USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
    'W', 0,
    'e', 0,
    'I', 0,
    'O', 0,
    ' ', 0,
    'c', 0,
    'o', 0,
    'p', 0,
    'o', 0,
    'c', 0,
    'e', 0,
    's', 0,
    's', 0,
    'o', 0,
    'r', 0,
    /* Index 0x03: Serial Number */
    (0*2 + 2),                         /* bLength (0 Char + Type + length) */
    USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
    /* Index 0x04: Interface 0, Alternate Setting 0 */
    ( 9*2 + 2),                        /* bLength (8 Char + Type + length) */
    USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
    'C', 0,
    'o', 0,
    'm', 0,
    'm', 0,
    'a', 0,
    'n', 0,
    'd', 0,
    'I', 0,
    'F', 0,
};

__attribute__ ((aligned(4))) uint8_t USBSerialStringDescriptor[] = {
    (2 + 2*27),                        /* bLength */
    USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
    '#', 0,
    '#', 0,
    '#', 0,
    '#', 0,
    '#', 0,
    '#', 0,
    '-', 0,
    '#', 0,
    '#', 0,
    '#', 0,
    '#', 0,
    '#', 0,
    '#', 0,
    '-', 0,
    '#', 0,
    '#', 0,
    '#', 0,
    '#', 0,
    '#', 0,
    '#', 0,
    '-', 0,
    '#', 0,
    '#', 0,
    '#', 0,
    '#', 0,
    '#', 0,
    '#', 0,
};
