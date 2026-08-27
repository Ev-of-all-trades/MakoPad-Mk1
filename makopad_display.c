#include "quantum.h"

#ifdef QUANTUM_PAINTER_ENABLE

#include "qp.h"
#include "qp_st7735.h"

#include "makopad_display.h"



// =========================================================================
// DISPLAY
// =========================================================================

static painter_device_t makopad_display;

static bool display_ready = false;
static bool display_dirty = false;


// ==========================================================================
// COLORS
// ==========================================================================
//
// Quantum Painter uses HSV values.
//
// Hue        0-255
// Saturation 0-255
// Value      0-255
// ==========================================================================

#define COLOR_BLACK   0,   0,   0
#define COLOR_WHITE   0,   0,   255
#define COLOR_GRAY    0,   0,   120
#define COLOR_CYAN    128, 255, 255
#define COLOR_GREEN   85,  255, 255
#define COLOR_BLUE    170, 255, 255
#define COLOR_RED     0,   255, 255
#define COLOR_YELLOW  43,  255, 255


// ==========================================================================
// LOGICAL DISPLAY SIZE
// ==========================================================================
// Physical panel: 128 x 160
//
// MakoPad assembly: LANDSCAPE
//
// Therefore the UI is designed as: 160 x 128
//
// Quantum Painter handles the physical rotation.
// ==========================================================================

#define UI_WIDTH  160
#define UI_HEIGHT 128


// ==========================================================================
// KEY AREAS
// ==========================================================================
// These coordinates correspond to the physical six-key arrangement below
// the display in the MakoPad CAD assembly.
//
//     KEY 1     KEY 2     KEY 3
//
//     KEY 4     KEY 5     KEY 6
// ==========================================================================

#define KEY_W 46
#define KEY_H 30

#define KEY_X1 4
#define KEY_X2 57
#define KEY_X3 110

#define KEY_Y1 34
#define KEY_Y2 69

// ============================================================================
// KEY LABELS
// ============================================================================

static const char *base_labels[6] = {
    "COPY", "PASTE", "SAVE",
    "HOME", "END", "DELETE"
};

static const char *frc_labels[6] = {
    "BUILD", "DEPLOY", "SIM",
    "DEBUG", "RIOLOG", "SHUFFLE"
};

static const char *ds_labels[6] = {
    "ENABLE", "DISABLE", "TELEOP",
    "AUTO", "TEST", "E-STOP"
};

static const char *limelight_labels[6] = {
    "LEFT", "FRONT", "RIGHT",
    "", "", ""
};

// ============================================================================
// DISPLAY CURRENT KEYS
// ============================================================================
static const char **get_layer_labels(void) {

    switch (get_highest_layer(layer_state)) {

        case _FRC:
            return frc_labels;

        case _DS:
            return ds_labels;

        case _LIMELITE:
            return limelight_labels;

        case _BASE:
        default:
            return base_labels;
    }
}


// ==========================================================================
// DISPLAY STATE
// ==========================================================================

bool makopad_display_is_ready(void) {
    return display_ready;
}

void makopad_display_mark_dirty(void) {
    display_dirty = true;
}


// =============================================================================
// DISPLAY INITILIZATION
// =============================================================================


void makopad_display_init(void) {
    makopad_display = qp_st7735_make_spi_device(
        MAKOPAD_DISPLAY_WIDTH,
        MAKOPAD_DISPLAY_HEIGHT,
        MAKOPAD_DISPLAY_CS_PIN,
        MAKOPAD_DISPLAY_DC_PIN,
        MAKOPAD_DISPLAY_RST_PIN,
        MAKOPAD_DISPLAY_SPI_DIVISOR,
        MAKOPAD_DISPLAY_SPI_MODE
    );

    // ----------------------------------------------------------------------
    // IMPORTANT
    // ----------------------------------------------------------------------
    // If the real DFR0928 arrives mirrored/upside-down, this will be one
    // line to change to QP_ROTATION_270.
    // ----------------------------------------------------------------------

    qp_init (
        makopad_display,
        QP_ROTATION_90
    );

    qp_power (
        makopad_display,
        true
    );

    display_ready = true;
    display_dirty = true;
}

// =============================================================================
// DRAW THE BOXES
// =============================================================================

static void draw_box (
    uint8_t left,
    uint8_t top,
    uint8_t right,
    uint8_t bottom,
    uint8_t hue,
    uint8_t saturation,
    uint8_t value,
    bool filled
) {
    qp_rect(
        makopad_display,
        left,
        top,
        right,
        bottom,
        hue,
        saturation,
        value,
        filled
    );
}


// =============================================================================
// HEADER
// =============================================================================

static void draw_header(void) {
    
    // HEADER BACKGROUND
    draw_box(
        0, 0,
        UI_WIDTH - 1,
        22,
        COLOR_BLUE,
        true
    );

    // HEADER SEPARATOR
    draw_box(
        0, 21,
        UI_WIDTH - 1,
        22,
        COLOR_CYAN,
        true
    );
}


// =============================================================================
// KEY BUTTONS
// =============================================================================

static void draw_key_boxes(void) {

    // Key 1
    draw_box(
        KEY_X1,
        KEY_Y1,
        KEY_X1 + KEY_W,
        KEY_Y1 + KEY_H,
        COLOR_CYAN,
        false
    );

    // Key 2
    draw_box(
        KEY_X2,
        KEY_Y1,
        KEY_X2 + KEY_W,
        KEY_Y1 + KEY_H,
        COLOR_CYAN,
        false
    );

    // Key 3
    draw_box(
        KEY_X3,
        KEY_Y1,
        KEY_X3 + KEY_W,
        KEY_Y1 + KEY_H,
        COLOR_CYAN,
        false
    );

    // Key 4
    draw_box(
        KEY_X1,
        KEY_Y2,
        KEY_X1 + KEY_W,
        KEY_Y2 + KEY_H,
        COLOR_CYAN,
        false
    );

    // Key 5
    draw_box(
        KEY_X2,
        KEY_Y2,
        KEY_X2 + KEY_W,
        KEY_Y2 + KEY_H,
        COLOR_CYAN,
        false
    );

    // Key 6
    draw_box(
        KEY_X3,
        KEY_Y2,
        KEY_X3 + KEY_W,
        KEY_Y2 + KEY_H,
        COLOR_CYAN,
        false
    );
}


// =============================================================================
// FOOTER
// =============================================================================

static void draw_footer(void) {
    draw_box(
        0, 112,
        UI_WIDTH - 1, 
        UI_HEIGHT - 1,
        COLOR_BLUE,
        true
    );
}


// =============================================================================
// MAIN UI
// =============================================================================

void makopad_display_update(void) {

    if (!display_ready) {
        return;
    }

    if (!display_dirty) {
        return;
    }

    display_dirty = false;

    // --------------------------------------------------------------------------
    // clear screen
    // --------------------------------------------------------------------------

    draw_box (
        0, 0,
        UI_WIDTH - 1,
        UI_HEIGHT - 1,
        COLOR_BLACK,
        true
    );


    // --------------------------------------------------------------------------
    // header
    // --------------------------------------------------------------------------

    draw_header();


    // --------------------------------------------------------------------------
    // key boxes
    // --------------------------------------------------------------------------

    draw_key_boxes();

    // --------------------------------------------------------------------------
    // footer
    // --------------------------------------------------------------------------

    draw_footer();


    // --------------------------------------------------------------------------
    // send everything to the physical display
    // --------------------------------------------------------------------------

    qp_flush(makopad_display);
}


// =============================================================================
// QMK INITIALIZATION
// =============================================================================

void makopad_display_qmk_init(void) {
    makopad_display_init();
    makopad_display_update();
}

#endif