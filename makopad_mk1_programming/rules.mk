BOOTLOADER = rp2040

ENCODER_ENABLE = yes
EXTRAKEY_ENABLE = yes

LTO_ENABLE = yes

# Quantum painter / TFT LCD Display

QUANTUM_PAINTER_ENABLE = yes
QUANTUM_PAINTER_DRIVERS += st7735_spi

# Display driver source
SRC += makopad_display.c

# Allowed number of fonts
QUANTUM_PAINTER_NUM_FONTS = 1