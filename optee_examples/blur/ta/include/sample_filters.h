#include "RawBase.h"

#ifndef OPTEE_SAMPLE_FILTERS_H
#define OPTEE_SAMPLE_FILTERS_H

pixel* blur(pixel* image_buffer, pixel* output_buffer, int row_length, int total_num_of_pixels, int v);

#endif //OPTEE_SAMPLE_FILTERS_H