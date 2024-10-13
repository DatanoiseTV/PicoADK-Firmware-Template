/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach (tinyusb.org)
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
 */

#include "bsp/board_api.h"
#include "tusb.h"
#include "get_serial.h"

// Define the PID to reflect both MIDI and Ethernet interfaces
#define _PID_MAP(itf, n)  ( (CFG_TUD_##itf) << (n) )
#define USB_PID           (0x4000 | _PID_MAP(CDC, 0) | _PID_MAP(MSC, 1) | _PID_MAP(HID, 2) | \
                           _PID_MAP(MIDI, 3) | _PID_MAP(VENDOR, 4) | _PID_MAP(ECM_RNDIS, 5) | _PID_MAP(NCM, 5))

// String Descriptor Index
enum
{
  STRID_LANGID = 0,
  STRID_MANUFACTURER,
  STRID_PRODUCT,
  STRID_SERIAL,
  STRID_INTERFACE,
  STRID_MAC
};

enum
{
  ITF_NUM_MIDI = 0,
  ITF_NUM_MIDI_STREAMING,
  ITF_NUM_NET,  // Network (ECM or RNDIS)
  ITF_NUM_TOTAL
};

#define CONFIG_TOTAL_LEN (TUD_CONFIG_DESC_LEN + TUD_MIDI_DESC_LEN + TUD_CDC_ECM_DESC_LEN)

#define EPNUM_MIDI   0x01  // Endpoint for MIDI
#define EPNUM_NET_NOTIF   0x81  // Notification endpoint for Ethernet
#define EPNUM_NET_OUT     0x02  // OUT endpoint for Ethernet
#define EPNUM_NET_IN      0x83  // IN endpoint for Ethernet
//--------------------------------------------------------------------+
// Device Descriptors
//--------------------------------------------------------------------+
tusb_desc_device_t const desc_device =
{
    .bLength            = sizeof(tusb_desc_device_t),
    .bDescriptorType    = TUSB_DESC_DEVICE,
    .bcdUSB             = 0x0200,
    .bDeviceClass       = TUSB_CLASS_MISC,
    .bDeviceSubClass    = MISC_SUBCLASS_COMMON,
    .bDeviceProtocol    = MISC_PROTOCOL_IAD,
    .bMaxPacketSize0    = CFG_TUD_ENDPOINT0_SIZE,

    .idVendor           = 0x2E8A,
    .idProduct          = USB_PID,
    .bcdDevice          = 0x0101,

    .iManufacturer      = STRID_MANUFACTURER,
    .iProduct           = STRID_PRODUCT,
    .iSerialNumber      = STRID_SERIAL,

    .bNumConfigurations = 0x01
};

// Invoked when received GET DEVICE DESCRIPTOR
uint8_t const * tud_descriptor_device_cb(void)
{
  return (uint8_t const *) &desc_device;
}

//--------------------------------------------------------------------+
// Configuration Descriptors
//--------------------------------------------------------------------+
uint8_t const desc_fs_configuration[] =
{
  // Config number, interface count, string index, total length, attribute, power in mA
 // TUD_CONFIG_DESCRIPTOR(1, ITF_NUM_TOTAL, 0, CONFIG_TOTAL_LEN, 0x00, 100),
TUD_CONFIG_DESCRIPTOR(1, ITF_NUM_TOTAL, 0, CONFIG_TOTAL_LEN, 0x00, 100),
  // MIDI Descriptor: Interface number, string index, EP Out & EP In address, EP size
  TUD_MIDI_DESCRIPTOR(ITF_NUM_MIDI, 0, EPNUM_MIDI, 0x80 | EPNUM_MIDI, 64),

  // Ethernet Descriptor (ECM or RNDIS): Interface number, description, MAC index, EP notification & data
  TUD_CDC_ECM_DESCRIPTOR(ITF_NUM_NET, STRID_INTERFACE, STRID_MAC, EPNUM_NET_NOTIF, 64, EPNUM_NET_OUT, EPNUM_NET_IN, CFG_TUD_NET_ENDPOINT_SIZE, CFG_TUD_NET_MTU)
};

#if TUD_OPT_HIGH_SPEED
uint8_t const desc_hs_configuration[] =
{
  // Config number, interface count, string index, total length, attribute, power in mA
  TUD_CONFIG_DESCRIPTOR(1, ITF_NUM_TOTAL, 0, CONFIG_TOTAL_LEN, 0x00, 100),

  // MIDI Descriptor for High-Speed: Interface number, string index, EP Out & EP In address, EP size
  TUD_MIDI_DESCRIPTOR(ITF_NUM_MIDI, 0, EPNUM_MIDI, 0x80 | EPNUM_MIDI, 512),

  // Ethernet Descriptor for High-Speed: Interface number, description, MAC index, EP notification & data
  TUD_CDC_ECM_DESCRIPTOR(ITF_NUM_NET, STRID_INTERFACE, STRID_MAC, EPNUM_NET_NOTIF, 512, EPNUM_NET_OUT, EPNUM_NET_IN, CFG_TUD_NET_ENDPOINT_SIZE, CFG_TUD_NET_MTU)
};
#endif

// Invoked when received GET CONFIGURATION DESCRIPTOR
uint8_t const * tud_descriptor_configuration_cb(uint8_t index)
{
  (void) index; // for multiple configurations

#if TUD_OPT_HIGH_SPEED
  // Although we are highspeed, host may be fullspeed.
  return (tud_speed_get() == TUSB_SPEED_HIGH) ?  desc_hs_configuration : desc_fs_configuration;
#else
  return desc_fs_configuration;
#endif
}

//--------------------------------------------------------------------+
// String Descriptors
//--------------------------------------------------------------------+
char const* string_desc_arr[] =
{
  (const char[]) { 0x09, 0x04 }, // 0: supported language is English (0x0409)
  "Datanoise",                   // 1: Manufacturer
  "PicoADK v2",                  // 2: Product
  usb_serial,                    // 3: Serial number (chip ID)
  "MIDI and Ethernet Interface", // 4: Interface description
  NULL                           // 5: MAC address placeholder (handled separately)
};

static uint16_t _desc_str[32];

// Invoked when received GET STRING DESCRIPTOR request
uint16_t const* tud_descriptor_string_cb(uint8_t index, uint16_t langid)
{
  (void) langid;

  uint8_t chr_count;

  if ( index == 0)
  {
    memcpy(&_desc_str[1], string_desc_arr[0], 2);
    chr_count = 1;
  }
  else if ( index == STRID_SERIAL )
  {
    chr_count = board_usb_get_serial(_desc_str + 1, 32);
  }
  else if ( index == STRID_MAC )
  {
    // Convert MAC address into UTF-16
    for (unsigned i = 0; i < sizeof(tud_network_mac_address); i++) {
      _desc_str[1 + chr_count++] = "0123456789ABCDEF"[(tud_network_mac_address[i] >> 4) & 0xf];
      _desc_str[1 + chr_count++] = "0123456789ABCDEF"[(tud_network_mac_address[i] >> 0) & 0xf];
    }
  }
  else
  {
    if ( !(index < sizeof(string_desc_arr)/sizeof(string_desc_arr[0])) ) return NULL;

    const char* str = string_desc_arr[index];

    // Cap at max char
    chr_count = strlen(str);
    if ( chr_count > 31 ) chr_count = 31;

    // Convert ASCII string into UTF-16
    for(uint8_t i = 0; i < chr_count; i++)
    {
      _desc_str[1 + i] = str[i];
    }
  }

  // first byte is length (including header), second byte is string type
  _desc_str[0] = (TUSB_DESC_STRING << 8 ) | (2*chr_count + 2);

  return _desc_str;
}
