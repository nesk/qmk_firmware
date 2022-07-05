/*
    TODO:

    - add layer keys with tap dance on GUI and ALT keys
    - add KC_CAPS with tap dance on CAPSWRD
    - add media keys on a higher layer
*/

#include "print.h"
#include QMK_KEYBOARD_H

#include "./helpers/helpers.h"

#ifdef CAPS_WORD_ENABLE
#define NK_CAPS CAPS_WORD
#define IS_CAPS_ACTIVE is_caps_word_on()
#else
#define NK_CAPS KC_CAPS_LOCK
#define IS_CAPS_ACTIVE (host_keyboard_leds() & (1<<USB_LED_CAPS_LOCK))
#endif

enum layers {
    _0,
    _1,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_0] = LAYOUT_65_iso(
        KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC, KC_DEL,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC,          _______,
        NK_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_BSLS, KC_ENT,  KC_PGUP,
        KC_LSFT, KC_GRV,  KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, KC_UP,   KC_PGDN,
        KC_LCTL, KC_LALT, KC_LGUI,                            KC_SPC,                    KC_RGUI, KC_RALT, KC_RCTL, KC_LEFT, KC_DOWN, /* KC_RGHT */QK_BOOT
    ),
    [_1] = LAYOUT_65_iso(
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

#if defined(CAPS_WORD_ENABLE) && CAPS_WORD_IDLE_TIMEOUT > 0
uint16_t caps_timer;
#endif
#endif

#ifdef TAP_DANCE_ENABLE
qk_tap_dance_action_t tap_dance_actions[] = {};
#endif

void keyboard_post_init_user(void) {
    #ifdef CONSOLE_ENABLE
    debug_enable = true;
    #endif

    #ifdef RGBLIGHT_ENABLE
    // Enable rgblight
    rgblight_enable_noeeprom();

    // Sets animation mode to RGBLIGHT_MODE_STATIC_LIGHT so that the built-in
    // animations don't interfere with our animation
    rgblight_mode_noeeprom(1);

    rgblight_sethsv_noeeprom(HSV_RED);
    #endif
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    DEBUG_RECORD(record)

    #if RGBLIGHT_ENABLE
    // store the position of the caps word key
    if (!has_caps_led_pos && keycode == NK_CAPS) {
        caps_led_pos = led_pos_from_record(record);
        has_caps_led_pos = true;

        #if CAPS_WORD_IDLE_TIMEOUT > 0
        caps_timer = timer_read();
        #endif
    }

    // if the caps key led is not in the expected state, change it
    if (has_caps_led_pos && is_caps_led_active != IS_CAPS_ACTIVE) {
        is_caps_led_active = IS_CAPS_ACTIVE;
        if (is_caps_led_active) {
            rgblight_sethsv_range(HSV_WHITE, caps_led_pos, caps_led_pos + 1);
        } else {
            rgblight_sethsv_range(HSV_RED, caps_led_pos, caps_led_pos + 1);
        }
    }
    #endif

    return true;
}

#if defined(RGBLIGHT_ENABLE) && defined(CAPS_WORD_ENABLE) && CAPS_WORD_IDLE_TIMEOUT > 0
void matrix_scan_user(void) {
    if (is_caps_led_active && !IS_CAPS_ACTIVE && timer_elapsed(caps_timer) > CAPS_WORD_IDLE_TIMEOUT) {
        rgblight_sethsv_range(HSV_RED, caps_led_pos, caps_led_pos + 1);
    }
}
#endif
