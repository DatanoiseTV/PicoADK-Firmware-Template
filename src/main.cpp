#include <stdio.h>
#include "project_config.h"

// Reverse compatibility with old PicoSDK.
#if __has_include("bsp/board_api.h")
#include "bsp/board_api.h"
#else
#include "bsp/board.h"
#endif

#include "midi_input_usb.h"
#include "audio_subsystem.h"
#include "picoadk_hw.h"


#include "FreeRTOS.h"
#include <task.h>
#include <queue.h>

#include "arduino_compat.h"

#include <dhserver.h>
#include <dnserver.h>
#include <httpd.h>
#include <lwip/ethip6.h>
#include <lwip/init.h>
#include <lwip/timeouts.h>


#ifdef INCLUDE_IPERF
  #include "lwip/apps/lwiperf.h"
#endif

// Audio Buffer (Size is set in lib/audio/include/audio_subsystem.h)
audio_buffer_pool_t *audio_pool;

#include "Heavy_prog.hpp"
Heavy_prog pd_prog(SAMPLE_RATE);

float smp[2];


#if (USE_USB_MIDI_HOST == 1)
static uint8_t midi_dev_addr = 0;
#endif

MIDIInputUSB usbMIDI;
TaskHandle_t usb_task_handle;

// TODO : move macros out of here

#define HV_HASH_NOTEIN          0x67E37CA3
#define HV_HASH_CTLIN           0x41BE0f9C
#define HV_HASH_POLYTOUCHIN     0xBC530F59
#define HV_HASH_PGMIN           0x2E1EA03D
#define HV_HASH_TOUCHIN         0x553925BD
#define HV_HASH_BENDIN          0x3083F0F7
#define HV_HASH_MIDIIN          0x149631bE
#define HV_HASH_MIDIREALTIMEIN  0x6FFF0BCF

#define HV_HASH_NOTEOUT         0xD1D4AC2
#define HV_HASH_CTLOUT          0xE5e2A040
#define HV_HASH_POLYTOUCHOUT    0xD5ACA9D1
#define HV_HASH_PGMOUT          0x8753E39E
#define HV_HASH_TOUCHOUT        0x476D4387
#define HV_HASH_BENDOUT         0xE8458013
#define HV_HASH_MIDIOUT         0x6511DE55
#define HV_HASH_MIDIOUTPORT     0x165707E4

#define MIDI_RT_CLOCK           0xF8
#define MIDI_RT_START           0xFA
#define MIDI_RT_CONTINUE        0xFB
#define MIDI_RT_STOP            0xFC
#define MIDI_RT_ACTIVESENSE     0xFE
#define MIDI_RT_RESET           0xFF

#ifdef __cplusplus
extern "C" {
#endif


    // IP



#define INIT_IP4(a, b, c, d) \
  { PP_HTONL(LWIP_MAKEU32(a, b, c, d)) }

/* lwip context */
static struct netif netif_data;

/* shared between tud_network_recv_cb() and service_traffic() */
static struct pbuf *received_frame;

/* this is used by this code, ./class/net/net_driver.c, and usb_descriptors.c */
/* ideally speaking, this should be generated from the hardware's unique ID (if available) */
/* it is suggested that the first byte is 0x02 to indicate a link-local address */
uint8_t tud_network_mac_address[6] = {0x02, 0x02, 0x84, 0x6A, 0x96, 0x00};

/* network parameters of this MCU */
static const ip4_addr_t ipaddr = INIT_IP4(192, 168, 7, 1);
static const ip4_addr_t netmask = INIT_IP4(255, 255, 255, 0);
static const ip4_addr_t gateway = INIT_IP4(0, 0, 0, 0);

/* database IP addresses that can be offered to the host; this must be in RAM to store assigned MAC addresses */
static dhcp_entry_t entries[] = {
    /* mac ip address               lease time */
    {{0}, INIT_IP4(192, 168, 7, 2), 24 * 60 * 60},
    {{0}, INIT_IP4(192, 168, 7, 3), 24 * 60 * 60},
    {{0}, INIT_IP4(192, 168, 7, 4), 24 * 60 * 60},
};

static const dhcp_config_t dhcp_config = {
    .router = INIT_IP4(0, 0, 0, 0),  /* router address (if any) */
    .port = 67,                      /* listen port */
    .dns = INIT_IP4(192, 168, 7, 1), /* dns server (if any) */
    "usb",                           /* dns suffix */
    TU_ARRAY_SIZE(entries),          /* num entry */
    entries                          /* entries */
};

static err_t linkoutput_fn(struct netif *netif, struct pbuf *p) {
  (void) netif;

  for (;;) {
    /* if TinyUSB isn't ready, we must signal back to lwip that there is nothing we can do */
    if (!tud_ready())
      return ERR_USE;

    /* if the network driver can accept another packet, we make it happen */
    if (tud_network_can_xmit(p->tot_len)) {
      tud_network_xmit(p, 0 /* unused for this example */);
      return ERR_OK;
    }

    /* transfer execution to TinyUSB in the hopes that it will finish transmitting the prior packet */
    tud_task();
  }
}

static err_t ip4_output_fn(struct netif *netif, struct pbuf *p, const ip4_addr_t *addr) {
  return etharp_output(netif, p, addr);
}

#if LWIP_IPV6
static err_t ip6_output_fn(struct netif *netif, struct pbuf *p, const ip6_addr_t *addr) {
  return ethip6_output(netif, p, addr);
}
#endif

static err_t netif_init_cb(struct netif *netif) {
  LWIP_ASSERT("netif != NULL", (netif != NULL));
  netif->mtu = CFG_TUD_NET_MTU;
  netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_LINK_UP | NETIF_FLAG_UP;
  netif->state = NULL;
  netif->name[0] = 'E';
  netif->name[1] = 'X';
  netif->linkoutput = linkoutput_fn;
  netif->output = ip4_output_fn;
#if LWIP_IPV6
  netif->output_ip6 = ip6_output_fn;
#endif
  return ERR_OK;
}

static void init_lwip(void) {
  struct netif *netif = &netif_data;

  lwip_init();

  /* the lwip virtual MAC address must be different from the host's; to ensure this, we toggle the LSbit */
  netif->hwaddr_len = sizeof(tud_network_mac_address);
  memcpy(netif->hwaddr, tud_network_mac_address, sizeof(tud_network_mac_address));
  netif->hwaddr[5] ^= 0x01;

  netif = netif_add(netif, &ipaddr, &netmask, &gateway, NULL, netif_init_cb, ip_input);
#if LWIP_IPV6
  netif_create_ip6_linklocal_address(netif, 1);
#endif
  netif_set_default(netif);
}

/* handle any DNS requests from dns-server */
bool dns_query_proc(const char *name, ip4_addr_t *addr) {
  if (0 == strcmp(name, "tiny.usb")) {
    *addr = ipaddr;
    return true;
  }
  return false;
}

bool tud_network_recv_cb(const uint8_t *src, uint16_t size) {
  /* this shouldn't happen, but if we get another packet before
  parsing the previous, we must signal our inability to accept it */
  if (received_frame) return false;

  if (size) {
    struct pbuf *p = pbuf_alloc(PBUF_RAW, size, PBUF_POOL);

    if (p) {
      /* pbuf_alloc() has already initialized struct; all we need to do is copy the data */
      memcpy(p->payload, src, size);

      /* store away the pointer for service_traffic() to later handle */
      received_frame = p;
    }
  }

  return true;
}

uint16_t tud_network_xmit_cb(uint8_t *dst, void *ref, uint16_t arg) {
  struct pbuf *p = (struct pbuf *) ref;

  (void) arg; /* unused for this example */

  return pbuf_copy_partial(p, dst, p->tot_len, 0);
}

static void service_traffic(void) {
  /* handle any packet received by tud_network_recv_cb() */
  if (received_frame) {
    ethernet_input(received_frame, &netif_data);
    pbuf_free(received_frame);
    received_frame = NULL;
    tud_network_recv_renew();
  }

  sys_check_timeouts();
}

void tud_network_init_cb(void) {
  /* if the network is re-initializing and we have a leftover packet, we must do a cleanup */
  if (received_frame) {
    pbuf_free(received_frame);
    received_frame = NULL;
  }
}


    // IP END

    /**
     * Task to handle USB MIDI input processing.
     *
     * @param pvParameters Unused task parameters
     */
    void usb_midi_task(void *pvParameters)
    {
        // Setup MIDI Callbacks using lambdas
        usbMIDI.setCCCallback([](uint8_t cc, uint8_t value, uint8_t channel) {
            // Handle Control Change (CC) event
            pd_prog.sendMessageToReceiverV(HV_HASH_CTLIN, 0, "fff", (float)value, (float)cc, (float)channel);
        });

        usbMIDI.setNoteOnCallback([](uint8_t note, uint8_t velocity, uint8_t channel) {
            if (velocity > 0)
            {
                pd_prog.sendMessageToReceiverV(HV_HASH_NOTEIN, 0, "fff", (float)note, (float)velocity, (float)channel);
                // Handle Note On event
            }
            else
            {
                pd_prog.sendMessageToReceiverV(HV_HASH_NOTEIN, 0, "fff", (float)note, (float)velocity, (float)channel);
                // Treat zero velocity as Note Off
            }
        });

        usbMIDI.setNoteOffCallback([](uint8_t note, uint8_t velocity, uint8_t channel) {
            pd_prog.sendMessageToReceiverV(HV_HASH_NOTEIN, 0, "fff", (float)note, (float)velocity, (float)channel);
            // Handle Note Off event
        });

        usbMIDI.setClockCallback([]() {
            // Handle MIDI Clock event
            pd_prog.sendMessageToReceiverV(HV_HASH_MIDIREALTIMEIN, 0, "f", (float)MIDI_RT_CLOCK);
        });

        while (1)
        {
            // TinyUSB Device Task
            #if (USE_USB_MIDI_HOST == 1)
            tuh_task();
            #else
            tud_task();
            #endif
            usbMIDI.process();
        }
    }

    /**
     * Task to blink an LED on GPIO pin 2.
     *
     * @param pvParameters Unused task parameters
     */
    void blinker_task(void *pvParameters)
    {
        gpio_init(2);
        gpio_set_dir(2, GPIO_OUT);

        while (1)
        {
            gpio_put(2, 1);
            vTaskDelay(pdMS_TO_TICKS(100)); // Delay for 100ms
            gpio_put(2, 0);
            vTaskDelay(pdMS_TO_TICKS(100)); // Delay for 100ms
        }
    }

    /**
     * Main entry point.
     */
    int main(void)
    {
        // Initialize hardware
        picoadk_init();

        // Initialize DSP engine (if needed)
        

        // Initialize the audio subsystem
        audio_pool = init_audio();

        // Create FreeRTOS tasks for MIDI handling and LED blinking
        xTaskCreate(usb_midi_task, "USB_MIDI_Task", 4096, NULL, configMAX_PRIORITIES, NULL);
        xTaskCreate(blinker_task, "Blinker_Task", 128, NULL, configMAX_PRIORITIES - 1, &usb_task_handle);
        vTaskCoreAffinitySet(usb_task_handle, (1 << 1));

        // Start the FreeRTOS scheduler
        vTaskStartScheduler();

 /* initialize lwip, dhcp-server, dns-server, and http */
  init_lwip();
  while (!netif_is_up(&netif_data));
  while (dhserv_init(&dhcp_config) != ERR_OK);
  while (dnserv_init(IP_ADDR_ANY, 53, dns_query_proc) != ERR_OK);
  httpd_init();

#ifdef INCLUDE_IPERF
  // test with: iperf -c 192.168.7.1 -e -i 1 -M 5000 -l 8192 -r
  lwiperf_start_tcp_server_default(NULL, NULL);
#endif

        // Idle loop (this is fine for Cortex-M33)
        while (1)
        {
            service_traffic();
            __wfi();
        }
    }

    /**
     * I2S audio callback for filling the audio buffer with samples.
     *
     * This function is called at a fixed rate determined by the audio subsystem
     * and must return within the interval between calls to avoid audio glitches.
     */
    void __not_in_flash_func(i2s_callback_func())
    {
        audio_buffer_t *buffer = take_audio_buffer(audio_pool, false);
        if (buffer == NULL)
        {
            return;
        }

        int32_t *samples = (int32_t *)buffer->buffer->bytes;

        // Fill buffer with 32-bit samples (stereo, 2 channels)
        for (uint i = 0; i < buffer->max_sample_count; i++)
        {
            pd_prog.processInlineInterleaved(smp, smp, 1);

            samples[i * 2 + 0] = float_to_int32(smp[0]);   // Left channel sample
            samples[i * 2 + 1] = float_to_int32(smp[1]);   // Right channel sample
            // Use your DSP function here for generating the audio samples
        }

        buffer->sample_count = buffer->max_sample_count;
        give_audio_buffer(audio_pool, buffer);
    }

#ifdef __cplusplus
}
#endif
