// PicoADK TinyUSB configuration.
//
// =============================================================================
// Layout
// -----------------------------------------------------------------------------
// 1.  Common  — MCU/RHPort/OS knobs that the pico-sdk + TinyUSB integration
//               expect to find.
// 2.  Device  — buffer/endpoint sizes for every class we MIGHT advertise.
//               Whether a class is *actually* enumerated is decided at
//               runtime by `picoadk::Usb::configure()` building the
//               descriptor blob in `usb_descriptors.c`.
// 3.  Host    — only compiled when `PICOADK_USB_HOST_ENABLED` is set
//               (Phase 1e). Turning this on adds ~20 KB to the firmware.
// 4.  App overrides — `picoadk_tusb_overrides.h` is included if the user
//               provides one. Drop a file with that name on the include
//               path to override anything below without forking this file.
//
// All sizes assume full-speed (12 Mbit/s); high-speed RP2040/RP2350 doesn't
// exist yet so the FS bumps are unconditional.
// =============================================================================

#ifndef _TUSB_CONFIG_H_
#define _TUSB_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

// --- 1. Common ---------------------------------------------------------------
#ifndef CFG_TUSB_MCU
#  error "CFG_TUSB_MCU must be defined (pico-sdk should set this via CMake)"
#endif

#ifndef CFG_TUSB_OS
#  define CFG_TUSB_OS                       OPT_OS_FREERTOS
#endif

#ifndef BOARD_DEVICE_RHPORT_NUM
#  define BOARD_DEVICE_RHPORT_NUM           0
#endif
#ifndef BOARD_DEVICE_RHPORT_SPEED
#  define BOARD_DEVICE_RHPORT_SPEED         OPT_MODE_FULL_SPEED
#endif
#define CFG_TUSB_RHPORT0_MODE               (OPT_MODE_DEVICE | BOARD_DEVICE_RHPORT_SPEED)

#ifndef CFG_TUSB_MEM_SECTION
#  define CFG_TUSB_MEM_SECTION
#endif
#ifndef CFG_TUSB_MEM_ALIGN
#  define CFG_TUSB_MEM_ALIGN                __attribute__((aligned(4)))
#endif

// Logging — turn up to 2/3 from CMake (-DCFG_TUSB_DEBUG=2) when bringing up
// new descriptors. Default off so production builds don't spam stdout.
#ifndef CFG_TUSB_DEBUG
#  define CFG_TUSB_DEBUG                    0
#endif

// --- 2. Device classes -------------------------------------------------------
//
// CFG_TUD_<CLASS> = 1 *compiles in* the class. The descriptor builder picks
// at enumeration time which classes to advertise; disabled-at-runtime classes
// cost a few KB of compiled-but-unused code and zero RAM.
//
// To strip a class entirely from the binary, override these from CMake (e.g.
// `target_compile_definitions(picoadk_app PRIVATE CFG_TUD_MSC=0)` will save
// ~6 KB of MSC code on synth-only builds).

#ifndef CFG_TUD_ENDPOINT0_SIZE
#  define CFG_TUD_ENDPOINT0_SIZE            64
#endif

#ifndef CFG_TUD_CDC
#  define CFG_TUD_CDC                       1
#endif
#ifndef CFG_TUD_MIDI
#  define CFG_TUD_MIDI                      1
#endif
#ifndef CFG_TUD_MSC
#  define CFG_TUD_MSC                       1
#endif
#ifndef CFG_TUD_HID
#  define CFG_TUD_HID                       0
#endif
#ifndef CFG_TUD_VENDOR
#  define CFG_TUD_VENDOR                    0
#endif

#define CFG_TUD_CDC_RX_BUFSIZE              256
#define CFG_TUD_CDC_TX_BUFSIZE              256
#define CFG_TUD_CDC_EP_BUFSIZE              64

#define CFG_TUD_MIDI_RX_BUFSIZE             256
#define CFG_TUD_MIDI_TX_BUFSIZE             256
#define CFG_TUD_MIDI_EP_BUFSIZE             64

#define CFG_TUD_MSC_EP_BUFSIZE              512

// --- 3. Host stack (Phase 1e) ------------------------------------------------
//
// Compiled in only when the build defines PICOADK_USB_HOST_ENABLED — leaving
// it off saves the ~20 KB the host stack costs.

#ifdef PICOADK_USB_HOST_ENABLED
#  define CFG_TUH_ENABLED                   1
#  define CFG_TUH_MAX_SPEED                 OPT_MODE_FULL_SPEED
#  define CFG_TUH_RPI_PIO_USB               1     // run host on PIO so device port stays up
#  define CFG_TUH_HUB                       1     // controllers often sit behind a hub
#  define CFG_TUH_DEVICE_MAX                (CFG_TUH_HUB ? 4 : 1)
#  define CFG_TUH_ENUMERATION_BUFSIZE       256

#  define CFG_TUH_MIDI                      1
#  define CFG_TUH_HID                       0
#  define CFG_TUH_CDC                       0
#  define CFG_TUH_MSC                       0
#endif

// --- 4. App overrides --------------------------------------------------------
#if __has_include("picoadk_tusb_overrides.h")
#  include "picoadk_tusb_overrides.h"
#endif

#ifdef __cplusplus
}
#endif
#endif  // _TUSB_CONFIG_H_
