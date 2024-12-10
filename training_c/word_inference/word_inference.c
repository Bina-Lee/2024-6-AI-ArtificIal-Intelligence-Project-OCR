#include "../typedef_struct/korean_word.h"
#include "word_inference.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

WORDIDX convert_VAR2IDX(WORDVAR var) {
    WORDIDX returnV;
    // 초성: 매핑된 0.0~1.0 값을 0~18로 변환 (총 21개에서 -1 보정)
    returnV.first = (int)round(var.first * 21.0) - 1;
    // 중성: 매핑된 0.0~1.0 값을 0~20로 변환 (총 23개에서 -1 보정)
    returnV.middle = (int)round(var.middle * 23.0) - 1;
    // 종성: 매핑된 0.0~1.0 값을 0~27로 변환 (총 30개에서 -1 보정)
    returnV.last = (int)round(var.last * 30.0) - 1;

    // 값이 범위를 벗어나지 않도록 클램핑
    if (returnV.first < 0) returnV.first = 0;
    if (returnV.first > 18) returnV.first = 18;

    if (returnV.middle < 0) returnV.middle = 0;
    if (returnV.middle > 20) returnV.middle = 20;

    if (returnV.last < 0) returnV.last = 0;
    if (returnV.last > 27) returnV.last = 27;

    return returnV;
}

char* convert_IDX2Char(WORDIDX idx) {
    int unicode = 0xAC00 + (idx.first * 21 * 28) + (idx.middle * 28) + idx.last;

    char* result = (char*)malloc(sizeof(char)*4);

    result[0] = (char)((unicode >> 12) | 0xE0);            // 첫 바이트
    result[1] = (char)(((unicode >> 6) & 0x3F) | 0x80);    // 둘째 바이트
    result[2] = (char)((unicode & 0x3F) | 0x80);           // 셋째 바이트
    result[3] = 0;                                      // 널 문자로 종료

    return result;
}