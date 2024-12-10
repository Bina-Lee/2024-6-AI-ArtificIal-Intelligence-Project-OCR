#include "../typedef_struct/image_struct.h"
#include "image_load.h"
#include <stdio.h>

void readBMPHeader(FILE *file, BMPHeader *header) {
    fread(header->signature, 1, 2, file);
    fread(&header->fileSize, 4, 1, file);
    fread(&header->reserved1, 2, 1, file);
    fread(&header->reserved2, 2, 1, file);
    fread(&header->dataOffset, 4, 1, file);
    fread(&header->headerSize, 4, 1, file);
    fread(&header->width, 4, 1, file);
    fread(&header->height, 4, 1, file);
    fread(&header->planes, 2, 1, file);
    fread(&header->bitsPerPixel, 2, 1, file);
    fread(&header->compression, 4, 1, file);
    fread(&header->imageSize, 4, 1, file);
    fread(&header->xPixelsPerMeter, 4, 1, file);
    fread(&header->yPixelsPerMeter, 4, 1, file);
    fread(&header->colorsUsed, 4, 1, file);
    fread(&header->importantColors, 4, 1, file);
}