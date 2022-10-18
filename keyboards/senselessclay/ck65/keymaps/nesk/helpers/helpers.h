#ifdef RGBLIGHT_ENABLE
uint8_t led_pos_from_col_and_row(uint8_t col, uint8_t row);
#   define led_pos_from_record(record) led_pos_from_col_and_row(record->event.key.col, record->event.key.row)
#endif

#ifdef CONSOLE_ENABLE
#   define DEBUG_RECORD(record) dprintf( \
    "Record: kc=0x%04X col=%u row=%u pressed=%b time=%u interrupt=%b count=%u\n", \
    keycode, \
    record->event.key.col, \
    record->event.key.row, \
    record->event.pressed, \
    record->event.time, \
    record->tap.interrupted, \
    record->tap.count \
)
#else
#   define DEBUG_RECORD(record)
#endif
