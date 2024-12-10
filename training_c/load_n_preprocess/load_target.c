#include "../typedef_struct/korean_word.h"
#include "load_target.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

WORDIDX find_target(int idx) {
    WORDIDX returnV = {0,0,0};

    FILE *file = fopen("../makeImage/TextIndexCode.txt", "r");
    if (file == NULL) {
        printf("Failed to open file");
        exit(1);
    }

    char line[256];
    char target[16]; // To store text{%05d}
    sprintf(target, "text%05d_", idx);

    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, target, strlen(target)) == 0) {
            // Extract the three %02d values from the line
            if (sscanf(line + strlen(target), "%02d_%02d_%02d", &returnV.first, &returnV.middle, &returnV.last) == 3) {
                fclose(file);
                return returnV; // Success
            }
        }
    }

    fclose(file);

    returnV.first = -1;
    returnV.middle = -1;
    returnV.last = -1;
    return returnV;
}

WORDVAR convert_IDX2VAR(WORDIDX idx){
    WORDVAR returnV;
    // 초성: 0 ~ 18을 0 ~ 1로 매핑 (총 21개로 확장)
    returnV.first = (double)(idx.first + 1) / 21.0;
    // 중성: 0 ~ 20을 0 ~ 1로 매핑 (총 23개로 확장)
    returnV.middle = (double)(idx.middle + 1) / 23.0;
    // 종성: 0 ~ 27을 0 ~ 1로 매핑 (총 30개로 확장)
    returnV.last = (double)(idx.last + 1) / 30.0;

    return returnV;
}