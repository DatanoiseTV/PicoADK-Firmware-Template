/*
 * PicoADK v2 — linker fragment that adds .psram_bss / .psram_data sections.
 *
 * Included via target_link_options(... "-T${...}/picoadk_v2_psram.ld") on the
 * v2 build only. Pico-sdk's default linker script reserves the PSRAM region
 * but doesn't allocate sections in it; this fragment puts user-tagged data
 * there and exposes __psram_dynamic_start/end for the TLSF allocator.
 *
 * Adjust PSRAM_LENGTH if your board ships with more than 8 MB.
 */

MEMORY
{
    PSRAM (rwx) : ORIGIN = 0x11000000, LENGTH = 8M
}

SECTIONS
{
    .psram_data (NOLOAD) : ALIGN(8)
    {
        __psram_data_start = .;
        KEEP(*(.psram_data))
        KEEP(*(.psram_data.*))
        __psram_data_end = .;
    } > PSRAM

    .psram_bss (NOLOAD) : ALIGN(8)
    {
        __psram_bss_start = .;
        KEEP(*(.psram_bss))
        KEEP(*(.psram_bss.*))
        __psram_bss_end = .;
    } > PSRAM

    .psram_dynamic (NOLOAD) : ALIGN(8)
    {
        __psram_dynamic_start = .;
        . = ORIGIN(PSRAM) + LENGTH(PSRAM);
        __psram_dynamic_end = .;
    } > PSRAM
}
