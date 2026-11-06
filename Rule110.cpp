#include "Rule110.h"

char Rule110::evaluate(char left, char center, char right) {
    int pattern = ((left - '0') << 2) | ((center - '0') << 1) | (right - '0');
    
    switch (pattern) {
        case 0b111: return '0';
        case 0b110: return '1';
        case 0b101: return '1';
        case 0b100: return '0';
        case 0b011: return '1';
        case 0b010: return '1';
        case 0b001: return '1';
        case 0b000: return '0';
        default:    return '0';
    }
}