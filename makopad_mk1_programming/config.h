#pragma once

// ==========================================================================
// MakoPad Display / SPI Configuration
// ==========================================================================
// RP2040 SPI0
// KB2040 wiring:
//
// GP18 -> Display SCLK
// GP19 -> Display MOSI
// --------------------------------------------------------------------------

#define SPI_DRIVER SPID0

#define SPI_SCK_PIN  GP18
#define SPI_MOSI_PIN GP19
#define SPI_MISO_PIN NO_PIN


// ==========================================================================
// DFR0928 / ST7735
// ==========================================================================

#define MAKOPAD_DISPLAY_CS_PIN  GP7
#define MAKOPAD_DISPLAY_RST_PIN GP8
#define MAKOPAD_DISPLAY_DC_PIN  GP9

#define MAKOPAD_DISPLAY_WIDTH  128
#define MAKOPAD_DISPLAY_HEIGHT 160

// SPI divisor.
// Start conservatively at 8.
// I can increase the speed once the hardware arrives.
#define MAKOPAD_DISPLAY_SPI_DIVISOR 8

#define MAKOPAD_DISPLAY_SPI_MODE 0


// ==========================================================================
// Quantum Painter
// ==========================================================================

// Keep the display on while I'm developing.
#define QUANTUM_PAINTER_DISPLAY_TIMEOUT 0

// One display only.
#define ST7735_NUM_DEVICES 1