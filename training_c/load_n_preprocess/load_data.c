#include "load_data.h"
#include "image_load.h"
#include "../typedef_struct/image_struct.h"
#include <stdio.h>
#include <stdlib.h>

FILE* load_image_n_data(unsigned char pixels[64][64], int idx){
    char filepath[200];
    char* filepathP = filepath;
    if(idx)sprintf(filepath,"../makeImage/exportImage/text%05d.bmp",idx);
    else{
        printf("Enter Image File Path : ");
        scanf("%s",filepath);
    }

    FILE *file = fopen(filepathP, "rb");
    if (file == NULL) {
        printf("파일을 열 수 없습니다.\n");
        exit(1);
    }

    BMPHeader header;
    readBMPHeader(file, &header);

    if (header.bitsPerPixel != 1) {
        printf("1비트 BMP 파일만 지원합니다.\n");
        fclose(file);
        exit(1);
    }

    int width = header.width;
    int height = header.height;

    if(width != 64 || height != 64){
        printf("image size error\n");
        fclose(file);
        exit(1);
    }

    // 이미지 데이터 위치로 이동
    fseek(file, header.dataOffset, SEEK_SET);

    // 각 픽셀을 저장할 배열 할당
    unsigned char raw_pixels[64][8] = {0,};

    // 이미지 데이터 읽기
    fread(raw_pixels, 1, width * height / 8, file);

    for(int i=0;i<64;i++){
        for(int j=0;j<64;j++){
            int byteIndex = j / 8;
            int bitIndex = 7 - (j % 8);
            pixels[63-i][j] = ((raw_pixels[i][byteIndex] >> bitIndex) & 1);
        }
    }
    return file;
}