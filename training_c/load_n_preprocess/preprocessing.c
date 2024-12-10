#include "preprocessing.h"
#include <math.h>

cropRange cropIndex(unsigned char originalImage[64][64]){
    unsigned int r[64] = {0,};
    unsigned int c[64] = {0,};

    for(int i=0;i<64;i++){
        for(int j=0;j<64;j++){
            r[i] += originalImage[i][j];
            c[j] += originalImage[i][j];
        }
    }

    cropRange returnV;

    for(int i=0 ;i<64;i++)if(c[i]){returnV.c_s = i;break;}
    for(int i=63;i>=0;i--)if(c[i]){returnV.c_e = i;break;}
    for(int i=0 ;i<64;i++)if(r[i]){returnV.r_s = i;break;}
    for(int i=63;i>=0;i--)if(r[i]){returnV.r_e = i;break;}

    returnV.r_size = returnV.r_e - returnV.r_s;
    returnV.c_size = returnV.c_e - returnV.c_s;

    return returnV;
}

void imageStretch(unsigned char originalImage[][64],
                unsigned char returnImage[][64],
                cropRange cropRangeInput){
    for(int i=0;i<64;i++){
        for(int j=0;j<64;j++){
            unsigned int oR = round(cropRangeInput.r_s + cropRangeInput.r_size * i/63);
            unsigned int oC = round(cropRangeInput.c_s + cropRangeInput.c_size * j/63);
            returnImage[i][j] = originalImage[oR][oC];
        }
    }
    return;
}

void image64to16(unsigned char originalImage[][64],
                double returnImage[][16]){
    for(int i=0;i<16;i++){
        for(int j=0;j<16;j++){
            unsigned int sum = 0;
            for(int x=0;x<4;x++){
                for(int y=0;y<4;y++){
                    sum += originalImage[i*4+x][j*4+y];
                }   
            }
            returnImage[i][j] = ((double)(sum)) / 16;
        }
    }
    return;
}