#include "cursor.h"


Cursor::Cursor() {
    auto tp_i = touchpad_getinfo();
    tp_h = (float)tp_i->height;
    tp_w = (float)tp_i->width;
}

void Cursor::set_cursor_visibility(bool _switch) {
    volatile int* cur_ctl = (volatile int*)PL111_cursor_ctl;
    cur_ctl[0] = _switch;
}
void Cursor::set_cursor_position(int _x, int _y) {
    volatile int* cur_pos = (volatile int*)PL111_cursor_pos;
    uint16_t y = (((float)_y / tp_h) * 240.0f);
    uint16_t x = (((float)_x / tp_w) * 320.0f);

    uint32_t val = *cur_pos;
    val &= ~0x3FF;
    val |= (y & 0x3FF);
    val &= ~((0xFFF)<< 16);
    val |= ((x & 0x3FF) << 16);
    *cur_pos = val;
}