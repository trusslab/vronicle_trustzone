//
// Created by lyx98 on 11/12/2020.
//

#ifndef OPTEE_RAWBASE_H
#define OPTEE_RAWBASE_H

typedef struct pixel {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} pixel;

unsigned char truncate_int(int value);
unsigned char truncate(float value);

unsigned char truncate_int(int value)
{
    if(value < 0) return 0;
    if(value > 255) return 255;

    return value;
}

unsigned char truncate(float value)
{
    if(value < 0.0) return 0;
    if(value > 255.0) return 255;

    return value;
}

#endif //OPTEE_RAWBASE_H