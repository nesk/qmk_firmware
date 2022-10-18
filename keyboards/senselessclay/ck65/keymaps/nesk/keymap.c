/*
    The NK_ prefix stands for my username "nesk", it is used to define my own key codes.

    TODO:

    - add layer keys with tap dance on GUI and ALT keys
    - add media keys on a higher layer
    - don't quit capswrd when using diacritics
*/

#include "print.h"
#include QMK_KEYBOARD_H

#include "./helpers/helpers.h"

enum layers {
    BASE,
    ALTERNATE,
};

#ifdef TAP_DANCE_ENABLE
void on_left_gui_dance_finished(qk_tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        register_mods(MOD_BIT(KC_LEFT_GUI));
    } else {
        layer_on(ALTERNATE);
    }
}

void on_left_gui_dance_reset(qk_tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        unregister_mods(MOD_BIT(KC_LEFT_GUI));
    } else {
        layer_off(ALTERNATE);
    }
}

void on_right_gui_dance_finished(qk_tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        register_mods(MOD_BIT(KC_RIGHT_GUI));
    } else {
        layer_on(ALTERNATE);
    }
}

void on_right_gui_dance_reset(qk_tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        unregister_mods(MOD_BIT(KC_RIGHT_GUI));
    } else {
        layer_off(ALTERNATE);
    }
}

enum {
    TD_LGUI_MO1,
    TD_RGUI_MO1,
};

qk_tap_dance_action_t tap_dance_actions[] = {
    [TD_LGUI_MO1] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, on_left_gui_dance_finished, on_left_gui_dance_reset),
    [TD_RGUI_MO1] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, on_right_gui_dance_finished, on_right_gui_dance_reset),
};

#   define NK_LGUI TD(TD_LGUI_MO1)
#   define NK_RGUI TD(TD_RGUI_MO1)
#else
#   define NK_LGUI KC_RGUI
#   define NK_RGUI KC_RGUI
#endif

#ifdef CAPS_WORD_ENABLE
#   define NK_CAPS CAPS_WORD
#   define IS_CAPS_ACTIVE is_caps_word_on()
#else
#   define NK_CAPS KC_CAPS_LOCK
#   define IS_CAPS_ACTIVE (host_keyboard_leds() & (1<<USB_LED_CAPS_LOCK))
#endif

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [BASE] = LAYOUT_65_iso(
        KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC, KC_DEL,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC,          _______,
        NK_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_BSLS, KC_ENT,  KC_PGUP,
        KC_LSFT, KC_GRV,  KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, KC_UP,   KC_PGDN,
        KC_LCTL, KC_LALT, NK_LGUI,                            KC_SPC,                    NK_RGUI, KC_RALT, KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT
    ),
    [ALTERNATE] = LAYOUT_65_iso(
        _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
        _______, _______, _______, _______, _______, _______, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_LEFT, _______, _______, _______, KC_HOME,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_END,
        _______, _______, _______,                            _______,                   _______, _______, _______, _______, _______, _______
    ),
};

#if RGBLIGHT_ENABLE
    uint8_t caps_led_pos;
    bool has_caps_led_pos = false;
    bool is_caps_led_active = false;
#endif

void keyboard_post_init_user(void) {
#ifdef CONSOLE_ENABLE
    debug_enable = true;
#endif

#ifdef RGBLIGHT_ENABLE
    // TODO: we should remove the following lines and bind all the necessary key codes to manipulate the animations
    rgblight_enable_noeeprom();
    rgblight_mode_noeeprom(RGBLIGHT_MODE_BREATHING + 1);
    rgblight_sethsv_noeeprom(HSV_BLUE);
#endif
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    DEBUG_RECORD(record);

#ifdef RGBLIGHT_ENABLE
        // store the position of the caps key
        if (!has_caps_led_pos && keycode == NK_CAPS) {
            caps_led_pos = led_pos_from_record(record);
            has_caps_led_pos = true;
        }

        // if the caps key led is not in the expected state, change it
        if (has_caps_led_pos && is_caps_led_active != IS_CAPS_ACTIVE) {
            is_caps_led_active = IS_CAPS_ACTIVE;
        }
#endif

    return true;
}

#ifdef RGBLIGHT_ENABLE
    void matrix_scan_user(void) {
        if (is_caps_led_active && IS_CAPS_ACTIVE) {
            rgblight_sethsv_range(HSV_CAPS_ACTIVE, caps_led_pos, caps_led_pos + 1);
        }
#   if defined(CAPS_WORD_ENABLE) && CAPS_WORD_IDLE_TIMEOUT > 0
        // if the caps led is not in the expected state and the timer is elapsed, then turn off the led
        else if (is_caps_led_active && !IS_CAPS_ACTIVE) {
            rgblight_sethsv_range(HSV_OFF, caps_led_pos, caps_led_pos + 1);
            is_caps_led_active = false;
        }
#   endif
    }
#endif
