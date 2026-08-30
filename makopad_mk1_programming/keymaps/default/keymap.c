#include "makopad_display.h"

#include QMK_KEYBOARD_H

// ENCODER MODES
typedef enum {
    ENC_VOLUME,
    ENC_SCROLL,
    ENC_BRIGHTNESS
} encoder_mode_t;

static encoder_mode_t encoder_mode = ENC_VOLUME;


// =============================================================================
// LAYERS
// =============================================================================

enum layers {
    _BASE,
    _FRC,
    _DS,
    _LIMELITE,
    _LAYER_COUNT
};


// =============================================================================
// CUSTOM KEYCODES
// =============================================================================

enum custom_keycodes {

    // FRC / VS Code
    FRC_BUILD = SAFE_RANGE,
    FRC_DEPLOY,
    FRC_SIMULATE,
    FRC_DEBUG,
    FRC_RIOLOG,
    FRC_SHUFFLEBOARD,

    // Driver Station
    DS_ENABLE,
    DS_DISABLE,
    DS_TELEOP,
    DS_AUTO,
    DS_TEST,
    DS_ESTOP,
    DS_ASTOP,
    DS_REFRESH,

    // Limelight
    LIMELIGHT_LEFT,
    LIMELIGHT_FRONT,
    LIMELIGHT_RIGHT
};


// =============================================================================
// STANDALONE GPIO PINS
// =============================================================================

#define ENCODER_SW_PIN GP0
#define LAYER_NEXT_PIN GP28
#define LAYER_PREV_PIN GP29


// =============================================================================
// INITIALIZATION
// =============================================================================

void keyboard_post_init_user(void) {
    // -------------------------------------------------------------------------
    // Encoder push switch
    // -------------------------------------------------------------------------

    setPinInputHigh(ENCODER_SW_PIN);


    // -------------------------------------------------------------------------
    // Layer buttons
    // -------------------------------------------------------------------------

    setPinInputHigh(LAYER_NEXT_PIN);
    setPinInputHigh(LAYER_PREV_PIN);


    // -------------------------------------------------------------------------
    // Display
    // -------------------------------------------------------------------------

    makopad_display_init();
    makopad_display_update();

}


// =============================================================================
// KEYMAPS
// =============================================================================

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    // -------------------------------------------------------------------------
    // BASE LAYER
    // -------------------------------------------------------------------------

    [_BASE] = LAYOUT_ortho_2x3(
        LCTL(KC_C), LCTL(KC_V), LCTL(KC_S),
        KC_HOME,    KC_END,     KC_DEL
    ),


    // -------------------------------------------------------------------------
    // FRC PROGRAMMING
    // -------------------------------------------------------------------------

    [_FRC] = LAYOUT_ortho_2x3(
        FRC_BUILD, FRC_DEPLOY, FRC_SIMULATE,
        FRC_DEBUG, FRC_RIOLOG, FRC_SHUFFLEBOARD
    ),


    // -------------------------------------------------------------------------
    // DRIVER STATION
    // -------------------------------------------------------------------------

    [_DS] = LAYOUT_ortho_2x3(
        DS_ENABLE, DS_DISABLE, DS_TELEOP,
        DS_AUTO,   DS_TEST,    DS_ESTOP
    ),


    // -------------------------------------------------------------------------
    // LIMELITE
    // -------------------------------------------------------------------------

    [_LIMELITE] = LAYOUT_ortho_2x3(
        LIMELIGHT_LEFT, LIMELIGHT_FRONT, LIMELIGHT_RIGHT,
        KC_NO,          KC_NO,           KC_NO
    )
};


// =============================================================================
// CUSTOM KEYCODE PROCESSING
// =============================================================================

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

    // Only execute macros when the key is pressed.
    if (!record->event.pressed) {
        return true;
    }

    switch (keycode) {

        // =====================================================================
        // FRC / VS CODE
        // =====================================================================

        case FRC_BUILD:

            // Ctrl + Alt + B
            tap_code16(LCTL(LALT(KC_B)));

            return false;


        case FRC_DEPLOY:

            // Ctrl + Alt + D
            tap_code16(LCTL(LALT(KC_D)));

            return false;


        case FRC_SIMULATE:

            // Ctrl + Alt + S
            tap_code16(LCTL(LALT(KC_S)));

            return false;


        case FRC_DEBUG:

            // Ctrl + Alt + G
            tap_code16(LCTL(LALT(KC_G)));

            return false;


        case FRC_RIOLOG:

            // Ctrl + Alt + R
            tap_code16(LCTL(LALT(KC_R)));

            return false;


        case FRC_SHUFFLEBOARD:

            // Ctrl + Alt + M
            tap_code16(LCTL(LALT(KC_M)));

            return false;


        // =====================================================================
        // DRIVER STATION
        // =====================================================================

        case DS_ENABLE:

            // Driver Station native enable shortcut:
            // [ + ] + \ 
            tap_code(KC_LBRC);
            tap_code(KC_RBRC);
            tap_code(KC_BSLS);

            return false;


        case DS_DISABLE:
            // Enter
            tap_code(KC_ENT);

            return false;


        case DS_TELEOP:
            // Ctrl + Alt + T
            tap_code16(LCTL(LALT(KC_T)));

            return false;


        case DS_AUTO:
            // Ctrl + Alt + A
            tap_code16(LCTL(LALT(KC_A)));

            return false;


        case DS_TEST:
            // Ctrl + Alt + S
            tap_code16(LCTL(LALT(KC_S)));

            return false;


        case DS_ESTOP:
            // Space
            tap_code(KC_SPC);

            return false;


        case DS_ASTOP:
            // Backspace
            tap_code(KC_BSPC);

            return false;


        case DS_REFRESH:
            // F1
            tap_code(KC_F1);

            return false;


        // =====================================================================
        // LIMELITE
        // =====================================================================

        case LIMELIGHT_LEFT:

            SEND_STRING("10.51.99.11:5801");

            return false;


        case LIMELIGHT_FRONT:

            SEND_STRING("10.51.99.12:5801");

            return false;


        case LIMELIGHT_RIGHT:

            SEND_STRING("10.51.99.13:5801");

            return false;
    }

    return true;
}


// =============================================================================
// ROTARY ENCODER
// =============================================================================

bool encoder_update_user(uint8_t index, bool clockwise) {

    // We only have one encoder.
    if (index != 0) {
        return false;
    }


    // -------------------------------------------------------------------------
    // HOLD ENCODER BUTTON + ROTATE
    // -------------------------------------------------------------------------
    // Holding the encoder switch changes its function:
    //
    // Volume -> Scroll -> Brightness -> Volume
    //
    // The encoder itself continues to generate rotation events normally.
    // -------------------------------------------------------------------------

    if (!readPin(ENCODER_SW_PIN)) {

        if (clockwise) {

            encoder_mode++;

            if (encoder_mode > ENC_BRIGHTNESS) {
                encoder_mode = ENC_VOLUME;
            }

        } else {

            if (encoder_mode == ENC_VOLUME) {
                encoder_mode = ENC_BRIGHTNESS;
            } else {
                encoder_mode--;
            }
        }
        
        makopad_display_mark_dirty();

        return false;
    }


    // -------------------------------------------------------------------------
    // NORMAL ENCODER OPERATION
    // -------------------------------------------------------------------------

    switch (encoder_mode) {

        // ---------------------------------------------------------------------
        // VOLUME
        // ---------------------------------------------------------------------

        case ENC_VOLUME:

            if (clockwise) {
                tap_code(KC_VOLU);
            } else {
                tap_code(KC_VOLD);
            }

            break;


        // ---------------------------------------------------------------------
        // SCROLL
        // ---------------------------------------------------------------------

        case ENC_SCROLL:

            if (clockwise) {
                tap_code(KC_WH_D);
            } else {
                tap_code(KC_WH_U);
            }

            break;


        // ---------------------------------------------------------------------
        // BRIGHTNESS
        // ---------------------------------------------------------------------

        case ENC_BRIGHTNESS:

            if (clockwise) {
                tap_code(KC_BRIU);
            } else {
                tap_code(KC_BRID);
            }

            break;
    }

    makopad_display_mark_dirty();

    return false;
}


// =============================================================================
// LAYER BUTTONS
// =============================================================================
// GP28 = NEXT LAYER
// GP29 = PREVIOUS LAYER
//
// These buttons are NOT part of the keyboard matrix, so they cannot use
// normal keymap entries. Instead, QMK polls the GPIO pins here.
//
// Both buttons use the internal pull-up resistor:
//
//   Not pressed = HIGH
//   Pressed     = LOW
// =============================================================================

void housekeeping_task_user(void) {

    static bool last_next_pressed = false;
    static bool last_prev_pressed = false;

    static uint16_t next_debounce_time = 0;
    static uint16_t prev_debounce_time = 0;

    bool next_pressed = !readPin(LAYER_NEXT_PIN);
    bool prev_pressed = !readPin(LAYER_PREV_PIN);


    // -------------------------------------------------------------------------
    // NEXT LAYER BUTTON
    // -------------------------------------------------------------------------

    if (next_pressed != last_next_pressed) {

        if (timer_elapsed(next_debounce_time) >= 30) {

            last_next_pressed = next_pressed;
            next_debounce_time = timer_read();

            if (next_pressed) {

                uint8_t current_layer =
                    get_highest_layer(layer_state);

                uint8_t next_layer =
                    (current_layer + 1) % _LAYER_COUNT;

                layer_move(next_layer);
                makopad_display_mark_dirty();
            }
        }
    }


    // -------------------------------------------------------------------------
    // PREVIOUS LAYER BUTTON
    // -------------------------------------------------------------------------

    if (prev_pressed != last_prev_pressed) {

        if (timer_elapsed(prev_debounce_time) >= 30) {

            last_prev_pressed = prev_pressed;
            prev_debounce_time = timer_read();

            if (prev_pressed) {

                uint8_t current_layer =
                    get_highest_layer(layer_state);

                uint8_t previous_layer =
                    (current_layer + _LAYER_COUNT - 1)
                    % _LAYER_COUNT;

                layer_move(previous_layer);
                makopad_display_mark_dirty();
            }
        }
    }


    // -------------------------------------------------------------------------
    // DISPLAY
    // -------------------------------------------------------------------------

    makopad_display_update();
}