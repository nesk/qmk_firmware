#ifdef RGBLIGHT_ENABLE
uint8_t led_pos_from_col_and_row(uint8_t col, uint8_t row) {
    switch (row) {
        case 0:
            switch (col) {
                case 0:
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                case 6:
                case 7:
                case 8:
                case 9:
                case 10:
                case 11:
                case 12:
                case 13:
                case 14:
                    return col;
            }
            break;

        case 1:
            switch (col) {
                case 0:
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                case 6:
                case 7:
                case 8:
                case 9:
                case 10:
                case 11:
                case 12:
                case 13:
                case 14:
                    return (14 - col) + 15;
            }
            break;

        case 2:
            switch (col) {
                case 0:
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                case 6:
                case 7:
                case 8:
                case 9:
                case 10:
                case 11:
                case 12:
                    return col + 30;
                case 14:
                    return 14 + 30;
            }
            break;

        case 3:
            switch (col) {
                case 0:
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                case 6:
                case 7:
                case 8:
                case 9:
                case 10:
                case 11:
                case 12:
                case 13:
                case 14:
                    return (14 - col) + 44;
            }
            break;

        case 4:
            switch (col) {
                case 0:
                case 1:
                case 2:
                    return col + 58;
                case 6:
                    return 4 + 58;
                case 9:
                case 10:
                case 11:
                case 12:
                case 13:
                case 14:
                    return (col - 5) + 58;
            }
            break;
    }

    return RGBLED_NUM;
}
#endif
