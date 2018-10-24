static const char FONT6x8[] = {
    /* 6x8 font, each line is a character each byte is a one pixel wide column
     * of that character. MSB is the top pixel of the column, LSB is the bottom
     * pixel of the column. 0 = pixel off. 1 = pixel on. */

    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // space
    0x00, 0x00, 0xFA, 0x00, 0x00, 0x00, // !
    0x00, 0xE0, 0x00, 0xE0, 0x00, 0x00, // "
    0x28, 0xFE, 0x28, 0xFE, 0x28, 0x00, // #
    0x24, 0x54, 0xFE, 0x54, 0x48, 0x00, // $
    0xC4, 0xC8, 0x10, 0x26, 0x46, 0x00, // %
    0x6C, 0x92, 0x6A, 0x04, 0x0A, 0x00, // &
    0x00, 0x10, 0xE0, 0xC0, 0x00, 0x00, // '
    0x00, 0x38, 0x44, 0x82, 0x00, 0x00, // (
    0x00, 0x82, 0x44, 0x38, 0x00, 0x00, // )
    0x54, 0x38, 0xFE, 0x38, 0x54, 0x00, // *
    0x10, 0x10, 0x7C, 0x10, 0x10, 0x00, // +
    0x00, 0x02, 0x1C, 0x18, 0x00, 0x00, // ,
    0x10, 0x10, 0x10, 0x10, 0x10, 0x00, // -
    0x00, 0x00, 0x06, 0x06, 0x00, 0x00, // .
    0x04, 0x08, 0x10, 0x20, 0x40, 0x00, // /
    //96 Bytes
    0x7C, 0x8A, 0x92, 0xA2, 0x7C, 0x00, // 0
    0x00, 0x42, 0xFE, 0x02, 0x00, 0x00, // 1
    0x42, 0x86, 0x8A, 0x92, 0x62, 0x00, // 2
    0x84, 0x82, 0x92, 0xB2, 0xCC, 0x00, // 3
    0x18, 0x28, 0x48, 0xFE, 0x08, 0x00, // 4
    0xE4, 0xA2, 0xA2, 0xA2, 0x9C, 0x00, // 5
    0x3C, 0x52, 0x92, 0x92, 0x0C, 0x00, // 6
    0x82, 0x84, 0x88, 0x90, 0xE0, 0x00, // 7
    0x6C, 0x92, 0x92, 0x92, 0x6C, 0x00, // 8
    0x60, 0x92, 0x92, 0x94, 0x78, 0x00, // 9
    0x00, 0x00, 0x28, 0x00, 0x00, 0x00, // :
    0x00, 0x00, 0x02, 0x2C, 0x00, 0x00, // ;
    0x00, 0x10, 0x28, 0x44, 0x82, 0x00, // <
    0x28, 0x28, 0x28, 0x28, 0x28, 0x00, // =
    0x00, 0x82, 0x44, 0x28, 0x10, 0x00, // >
    0x40, 0x80, 0x8A, 0x90, 0x60, 0x00, // ?
    //96*2 = 192 Bytes
    0x7C, 0x82, 0xBA, 0x9A, 0x72, 0x00, // @
    0x3E, 0x48, 0x88, 0x48, 0x3E, 0x00, // A
    0xFE, 0x92, 0x92, 0x92, 0x6C, 0x00, // B
    0x7C, 0x82, 0x82, 0x82, 0x44, 0x00, // C
    0xFE, 0x82, 0x82, 0x82, 0x7C, 0x00, // D
    0xFE, 0x92, 0x92, 0x92, 0x82, 0x00, // E
    0xFE, 0x90, 0x90, 0x90, 0x80, 0x00, // F
    0x7C, 0x82, 0x92, 0x92, 0x5E, 0x00, // G
    0xFE, 0x10, 0x10, 0x10, 0xFE, 0x00, // H
    0x00, 0x82, 0xFE, 0x82, 0x00, 0x00, // I
    0x04, 0x02, 0x82, 0xFC, 0x80, 0x00, // J
    0xFE, 0x10, 0x28, 0x44, 0x82, 0x00, // K
    0xFE, 0x02, 0x02, 0x02, 0x02, 0x00, // L
    0xFE, 0x40, 0x38, 0x40, 0xFE, 0x00, // M
    0xFE, 0x20, 0x10, 0x08, 0xFE, 0x00, // N
    0x7C, 0x82, 0x82, 0x82, 0x7C, 0x00, // O
    //96*3 = 288 Bytes
    0xFE, 0x90, 0x90, 0x90, 0x60, 0x00, // P
    0x7C, 0x82, 0x8A, 0x84, 0x7A, 0x00, // Q
    0xFE, 0x90, 0x98, 0x94, 0x62, 0x00, // R
    0x64, 0x92, 0x92, 0x92, 0x4C, 0x00, // S
    0x80, 0x80, 0xFE, 0x80, 0x80, 0x00, // T
    0xFC, 0x02, 0x02, 0x02, 0xFC, 0x00, // U
    0xF8, 0x04, 0x02, 0x04, 0xF8, 0x00, // V
    0xFC, 0x02, 0x1C, 0x02, 0xFC, 0x00, // W
    0xC6, 0x28, 0x10, 0x28, 0xC6, 0x00, // X
    0xC0, 0x20, 0x1E, 0x20, 0xC0, 0x00, // Y
    0x86, 0x8A, 0x92, 0xA2, 0xC2, 0x00, // Z
    0x00, 0xFE, 0x82, 0x82, 0x82, 0x00, // [
    0x40, 0x20, 0x10, 0x08, 0x04, 0x00, // '\'
    0x00, 0x82, 0x82, 0x82, 0xFE, 0x00, // ]
    0x20, 0x40, 0x80, 0x40, 0x20, 0x00, // ^
    0x01, 0x01, 0x01, 0x01, 0x01, 0x00, // _
    //96*4 = 384 Bytes
    0x00, 0xC0, 0xE0, 0x10, 0x00, 0x00, // `
    0x04, 0x2A, 0x2A, 0x2A, 0x1E, 0x00, // a
    0xFE, 0x14, 0x22, 0x22, 0x1C, 0x00, // b
    0x1C, 0x22, 0x22, 0x22, 0x14, 0x00, // c
    0x1C, 0x22, 0x22, 0x14, 0xFE, 0x00, // d
    0x1C, 0x2A, 0x2A, 0x2A, 0x18, 0x00, // e
    0x00, 0x10, 0x7E, 0x90, 0x40, 0x00, // f
    0x18, 0x25, 0x25, 0x25, 0x3E, 0x00, // g
    0xFE, 0x10, 0x20, 0x20, 0x1E, 0x00, // h
    0x00, 0x22, 0xBE, 0x02, 0x00, 0x00, // i
    0x00, 0x04, 0x02, 0x02, 0xBC, 0x00, // j
    0x00, 0xFE, 0x08, 0x14, 0x22, 0x00, // k
    0x00, 0x82, 0xFE, 0x02, 0x00, 0x00, // l
    0x3E, 0x20, 0x1E, 0x20, 0x1E, 0x00, // m
    0x3E, 0x10, 0x20, 0x20, 0x1E, 0x00, // n
    0x1C, 0x22, 0x22, 0x22, 0x1C, 0x00, // o
    //96*5 = 480 Bytes
    0x3F, 0x18, 0x24, 0x24, 0x18, 0x00, // p
    0x18, 0x24, 0x24, 0x18, 0x3F, 0x00, // q
    0x3E, 0x10, 0x20, 0x20, 0x10, 0x00, // r
    0x12, 0x2A, 0x2A, 0x2A, 0x24, 0x00, // s
    0x20, 0x20, 0xFC, 0x22, 0x24, 0x00, // t
    0x3C, 0x02, 0x02, 0x04, 0x3E, 0x00, // u
    0x38, 0x04, 0x02, 0x04, 0x38, 0x00, // v
    0x3C, 0x02, 0x0C, 0x02, 0x3C, 0x00, // w
    0x22, 0x14, 0x08, 0x14, 0x22, 0x00, // x
    0x32, 0x09, 0x09, 0x09, 0x3E, 0x00, // y
    0x22, 0x26, 0x2A, 0x32, 0x22, 0x00, // z
    0x00, 0x10, 0x6C, 0x82, 0x00, 0x00, // {
    0x00, 0x00, 0xEE, 0x00, 0x00, 0x00, // |
    0x00, 0x82, 0x6C, 0x10, 0x00, 0x00, // }
    0x40, 0x80, 0x40, 0x20, 0x40, 0x00, // ~
    0x00, 0x60, 0x90, 0x90, 0x60, 0x00, // degrees symbol
    //96*6 = 576 Bytes
    //Special Characters ** NON-ASCII **
    0x00, 0x00, 0xFE, 0x82, 0x82, 0x82, // [
    0x82, 0x82, 0x82, 0xFE, 0x00, 0x00  //  ]
};
