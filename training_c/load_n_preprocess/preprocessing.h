#include <math.h>

typedef struct{
    unsigned int r_s;
    unsigned int c_s;
    unsigned int r_e;
    unsigned int c_e;
    unsigned int r_size;
    unsigned int c_size;
}cropRange;

cropRange cropIndex(unsigned char originalImage[64][64]);

void imageStretch(unsigned char originalImage[][64],
                unsigned char returnImage[][64],
                cropRange cropRangeInput);

void image64to16(unsigned char originalImage[][64],
                double returnImage[][16]);