//define hex symbols
#define _7SegOne 0xF5
#define _7SegTwo 0x4C
#define _7SegThree 0x64
#define _7SegFour 0x35
#define _7SegFive 0x26
#define _7SegSix 0x06
#define _7SegSeven 0xB4
#define _7SegEight 0x04
#define _7SegNine 0x34
#define _7SegZero 0x84
#define _7SegA 0x14
#define _7SegB 0x07
#define _7SegC 0x8E
#define _7SegD 0x45
#define _7SegE 0x0E
#define _7SegF 0x1E

unsigned char int_to_SSD(char hex_value) {
    switch(hex_value) {
        case 0:
            return _7SegZero;
        case 1:
            return _7SegOne;
        case 2:
            return _7SegTwo;
        case 3:
            return _7SegThree;
        case 4:
            return _7SegFour;
        case 5:
            return _7SegFive;
        case 6:
            return _7SegSix;
        case 7:
            return _7SegSeven;
        case 8:
            return _7SegEight;
        case 9:
            return _7SegNine;
        case 10:
            return _7SegA;
        case 11:
            return _7SegB;
        case 12:
            return _7SegC;
        case 13:
            return _7SegD;
        case 14:
            return _7SegE;
        case 15:
            return _7SegF;
        default:
            return 0xFF;
    }
}
