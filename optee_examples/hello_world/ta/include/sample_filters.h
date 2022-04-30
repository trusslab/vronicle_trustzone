#include "RawBase.h"

#ifndef OPTEE_SAMPLE_FILTERS_H
#define OPTEE_SAMPLE_FILTERS_H

pixel* blur(pixel* image_buffer, pixel* output_buffer, int row_length, int total_num_of_pixels, int v);
pixel* sharpen(pixel* image_buffer, pixel* output_buffer, int row_length, int total_num_of_pixels, int v);
void change_brightness(pixel* image_buffer, pixel* output_buffer, int row_length, int total_num_of_pixels, float target_percentage);
void denoise_simple(pixel* image_buffer, pixel* output_buffer, int row_length, int total_num_of_pixels);
void gray_frame(pixel* image_buffer, pixel* output_buffer, int row_length, int total_num_of_pixels);

pixel* blur(pixel* image_buffer, pixel* output_buffer, int row_length, int total_num_of_pixels, int v){
    // Inspired by https://processing.org/examples/blur.html
    float avg_weight = 1.0 / (v * v);
    int pad = v / 2;
    // float** kernel = new float*[v];
	float** kernel = TEE_Malloc(sizeof(float*) * v, 0);
    for(int i = 0; i < v; ++i){
        // kernel[i] = new float[v];
		kernel[i] = TEE_Malloc(sizeof(float) * v, 0);
        for(int i2 = 0; i2 < v; ++i2){
            kernel[i][i2] = avg_weight;
        }
    }
    // printf("avg_weight: %f\n", avg_weight);
    // float kernel[3][3] = {{1, 2, 1}, {2, 4, 2}, {1, 2, 1}};
    int column_length = total_num_of_pixels / row_length;   // or height
    for(int y = 0; y < column_length; ++y){
        for(int x = 0; x < row_length; ++x){
            float temp_r = 0.0, temp_g = 0.0, temp_b = 0.0;
            for(int ky = 0 - pad; ky <= pad; ++ky){
                for(int kx = 0 - pad; kx <= pad; ++kx){
                    int pos = (y + ky) * row_length;
                    if(pos >= 0 && pos < total_num_of_pixels - 1 && (x + kx) >= 0 && (x + kx) < row_length){
                        pos += (x + kx);
                        temp_r += kernel[ky + pad][kx + pad] * image_buffer[pos].r;
                        temp_g += kernel[ky + pad][kx + pad] * image_buffer[pos].g;
                        temp_b += kernel[ky + pad][kx + pad] * image_buffer[pos].b;
                    }
                    // printf("y: %d, x: %d, ky: %d, kx: %d\n", y, x, ky, kx);
                }
            }
            /*
            temp_r *= 1.0 / 16.0;
            temp_g *= 1.0 / 16.0;
            temp_b *= 1.0 / 16.0;
            */
            output_buffer[y * row_length + x].r = truncate(temp_r);
            output_buffer[y * row_length + x].g = truncate(temp_g);
            output_buffer[y * row_length + x].b = truncate(temp_b);
        }
    }
    for(int i = 0; i < v; ++i){
		// free(kernel[i]);
        TEE_Free(kernel[i]);
    }
	// free(kernel);
    TEE_Free(kernel);
    return output_buffer;
}

pixel* sharpen(pixel* image_buffer, pixel* output_buffer, int row_length, int total_num_of_pixels, int v){
    // Inspired (not copy) by https://ai.stanford.edu/~syyeung/cvweb/tutorial1.html
    float avg_weight = 1.0 / ((v * v) - 1);
    int pad = v / 2;
    // float** kernel = new float*[v];
	float** kernel = TEE_Malloc(sizeof(float*) * v, 0);
    for(int i = 0; i < v; ++i){
        // kernel[i] = new float[v];
		kernel[i] = TEE_Malloc(sizeof(float) * v, 0);
        for(int i2 = 0; i2 < v; ++i2){
            kernel[i][i2] = 0 - avg_weight;
        }
    }
    kernel[pad][pad] = 2.0;
    // printf("avg_weight: %f\n", 0 - avg_weight);
    // float kernel[3][3] = {{1, 2, 1}, {2, 4, 2}, {1, 2, 1}};
    int column_length = total_num_of_pixels / row_length;   // or height
    for(int y = 0; y < column_length; ++y){
        for(int x = 0; x < row_length; ++x){
            float temp_r = 0.0, temp_g = 0.0, temp_b = 0.0;
            for(int ky = 0 - pad; ky <= pad; ++ky){
                for(int kx = 0 - pad; kx <= pad; ++kx){
                    int pos = (y + ky) * row_length;
                    if(pos >= 0 && pos < total_num_of_pixels - 1 && (x + kx) >= 0 && (x + kx) < row_length){
                        pos += (x + kx);
                        temp_r += kernel[ky + pad][kx + pad] * image_buffer[pos].r;
                        temp_g += kernel[ky + pad][kx + pad] * image_buffer[pos].g;
                        temp_b += kernel[ky + pad][kx + pad] * image_buffer[pos].b;
                    }
                    // printf("y: %d, x: %d, ky: %d, kx: %d\n", y, x, ky, kx);
                }
            }
            /*
            temp_r *= 1.0 / 16.0;
            temp_g *= 1.0 / 16.0;
            temp_b *= 1.0 / 16.0;
            */
            output_buffer[y * row_length + x].r = truncate(temp_r);
            output_buffer[y * row_length + x].g = truncate(temp_g);
            output_buffer[y * row_length + x].b = truncate(temp_b);
        }
    }
    for(int i = 0; i < v; ++i){
        // free(kernel[i]);
        TEE_Free(kernel[i]);
    }
    // free(kernel);
    TEE_Free(kernel);
    return output_buffer;
}

void change_brightness(pixel* image_buffer, pixel* output_buffer, int row_length, int total_num_of_pixels, float target_percentage){
    int column_length = total_num_of_pixels / row_length;   // or height
    for(int y = 0; y < column_length; ++y){
        for(int x = 0; x < row_length; ++x){
            float temp_r = 0.0, temp_g = 0.0, temp_b = 0.0;

            temp_r = image_buffer[y * row_length + x].r * target_percentage;
            temp_g = image_buffer[y * row_length + x].g * target_percentage;
            temp_b = image_buffer[y * row_length + x].b * target_percentage;

            output_buffer[y * row_length + x].r = truncate(temp_r);
            output_buffer[y * row_length + x].g = truncate(temp_g);
            output_buffer[y * row_length + x].b = truncate(temp_b);
        }
    }
}

void denoise_simple(pixel* image_buffer, pixel* output_buffer, int row_length, int total_num_of_pixels){
    // Modified from https://github.com/m-cody/ImageEditor
    int column_length = total_num_of_pixels / row_length;   // or height
    int yStart, xStart, yEnd, xEnd;
    for (int y = 0; y < column_length - 1; y++)
	{
        for (int x = 0; x < row_length - 1; x++)
		{
            if (y == 0)
			{
				yStart = y;
				yEnd = y + 1;
			}
			else if (y == column_length - 1)
			{
				yStart = y - 1;
				yEnd = y;
			}
			else
			{
				yStart = y - 1;
				yEnd = y + 1;
			}

            if (x == 0)
			{
				xStart = x;
				xEnd = x + 1;
			}
			else if (x == row_length - 1)
			{
				xStart = x - 1;
				xEnd = x;
			}
			else
			{
				xStart = x - 1;
				xEnd = x + 1;
			}

            int red = 0, green = 0, blue = 0;
			for (int i = yStart; i <= yEnd; i++)
			{
				for (int j = xStart; j <= xEnd; j++)
				{
					red += image_buffer[i * row_length + j].r;
					green += image_buffer[i * row_length + j].g;
					blue += image_buffer[i * row_length + j].b;
				}
			}
			output_buffer[y * row_length + x].r = red / 9;
			output_buffer[y * row_length + x].g = green / 9;
			output_buffer[y * row_length + x].b = blue / 9;
        }
    }
}

void gray_frame(pixel* image_buffer, pixel* output_buffer, int row_length, int total_num_of_pixels){
    // Modified from https://github.com/m-cody/ImageEditor
    int column_length = total_num_of_pixels / row_length;   // or height
    int pixelValue;
	for (int i = 0; i < column_length; i++)
	{
		for (int j = 0; j < row_length; j++)
		{
            int current_position = i * row_length + j;
			pixelValue = image_buffer[current_position].r;
			pixelValue += image_buffer[current_position].g;
			pixelValue += image_buffer[current_position].b;
			pixelValue /= 3;
			output_buffer[current_position].r = pixelValue;
			output_buffer[current_position].g = pixelValue;
			output_buffer[current_position].b = pixelValue;
		}
	}
}

#ifndef ClampToByte
#define ClampToByte(v)(((unsigned) (int)(v)) < (255) ? (v) : (v < 0) ? (int)(0) : (int)(255))
#endif
#ifndef min
#define min(a, b) (((a) < (b)) ? (a) : (b))
#endif
#ifndef max
#define max(a, b) (((a) > (b)) ? (a) : (b))
#endif

void
rgb2ycbcr(unsigned char R, unsigned char G, unsigned char B, unsigned char *y, unsigned char *cb, unsigned char *cr) {
    *y = (unsigned char) ((19595 * R + 38470 * G + 7471 * B) >> 16);
    *cb = (unsigned char) (((36962 * (B - *y)) >> 16) + 128);
    *cr = (unsigned char) (((46727 * (R - *y)) >> 16) + 128);
}


void
autoLevel(const unsigned int *histgram, unsigned char *remapLut, int numberOfPixels, float cutLimit, float contrast) {
    int minPos = 0, maxPos = 255;
    int minValue = 0, maxValue = 255;
    for (int I = 0; I < 256; I++) {
        if (histgram[I] != 0) {
            minValue = I;
            break;
        }
    }
    for (int I = 255; I >= 0; I--) {
        if (histgram[I] != 0) {
            maxValue = I;
            break;
        }
    }
    int sum = 0;
    for (int I = minValue; I < 256; I++) {
        sum = sum + histgram[I];
        if (sum >= numberOfPixels * cutLimit) {
            minPos = I;
            break;
        }
    }
    sum = 0;
    for (int I = 255; I >= 0; I--) {
        sum = sum + histgram[I];
        if (sum >= numberOfPixels * cutLimit) {
            maxPos = I;
            break;
        }
    }

    int delta = (int) ((maxValue - minValue) * contrast * 0.5f);
    minValue = ClampToByte(minValue - delta);
    maxValue = ClampToByte(maxValue + delta);
    if (maxPos != minPos) {
        for (int I = 0; I < 256; I++) {
            if (I < minPos)
                remapLut[I] = (unsigned char) minValue;
            else if (I > maxPos)
                remapLut[I] = (unsigned char) maxValue;
            else
                remapLut[I] = (unsigned char) ClampToByte(
                        (maxValue - minValue) * (I - minPos) / (maxPos - minPos) + minValue);

        }
    } else {
        for (int I = 0; I < 256; I++) {
            remapLut[I] = (unsigned char) maxPos;
        }
    }
}

int isColorCast(const unsigned int *histogramCb, const unsigned int *histogramCr, int numberOfPixels, int colorCoeff) {
    unsigned int sumCb = 0;
    unsigned int sumCr = 0;
    float meanCb = 0, meanCr = 0;
    for (unsigned int i = 0; i < 256; i++) {
        sumCb += histogramCb[i] * i;
        sumCr += histogramCr[i] * i;
    }
    meanCb = sumCb * (1.0f / numberOfPixels);
    meanCr = sumCr * (1.0f / numberOfPixels);
    int avgColorCoeff = (abs(meanCb - 127) + abs(meanCr - 127));
    if (avgColorCoeff < colorCoeff) {
        return 0;
    }
    return 1;
}


int autoWhiteBalance(unsigned char *input, unsigned char *output, int width, int height, int channels, int stride,
                      int colorCoeff, float cutLimit,
                      float contrast) {
    // 0 for failure, 1 for success
    int ret = 0;
    if (channels == 3 || channels == 4) {
        int numberOfPixels = height * width;
        unsigned int histogramYcbcr[768] = {0};
        unsigned int *histogramY = &histogramYcbcr[0];
        unsigned int *histogramCb = &histogramYcbcr[256];
        unsigned int *histogramCr = &histogramYcbcr[512];
        unsigned int histogramRGB[768] = {0};
        unsigned int *histogramR = &histogramRGB[0];
        unsigned int *histogramG = &histogramRGB[256];
        unsigned int *histogramB = &histogramRGB[512];
        unsigned char Y = 0;
        unsigned char Cb = 0;
        unsigned char Cr = 0;
        for (int y = 0; y < height; y++) {
            const unsigned char *scanIn = input + y * stride;
            for (int x = 0; x < width; x++) {
                const unsigned char R = scanIn[0];
                const unsigned char G = scanIn[1];
                const unsigned char B = scanIn[2];
                histogramR[R]++;
                histogramG[G]++;
                histogramB[B]++;
                rgb2ycbcr(R, G, B, &Y, &Cb, &Cr);
                histogramY[Y]++;
                histogramCb[Cb]++;
                histogramCr[Cr]++;
                scanIn += channels;
            }
        }
        ret = isColorCast(histogramCb, histogramCr, numberOfPixels, colorCoeff);
        if (!ret) {
            memcpy(output, input, numberOfPixels
                                  * channels * sizeof(*input));
            return ret;
        }
        unsigned char mapRGB[256 * 3] = {0};
        unsigned char *mapR = &mapRGB[0];
        unsigned char *mapG = &mapRGB[256];
        unsigned char *mapB = &mapRGB[256 + 256];
        autoLevel(histogramR, mapR, numberOfPixels, cutLimit, contrast);
        autoLevel(histogramG, mapG, numberOfPixels, cutLimit, contrast);
        autoLevel(histogramB, mapB, numberOfPixels, cutLimit, contrast);
        for (int y = 0; y < height; y++) {
            unsigned char *scanIn = input + y * stride;
            unsigned char *scanOut = output + y * stride;
            for (int x = 0; x < width; x++) {
                scanOut[0] = mapR[scanIn[0]];
                scanOut[1] = mapG[scanIn[1]];
                scanOut[2] = mapB[scanIn[2]];
                scanIn += channels;
                scanOut += channels;
            }
        }
    }
    return ret;
}

void auto_white_balance_pure_c(pixel* image_buffer, pixel* output_buffer, int row_length, int total_num_of_pixels){
    int colorCoeff = 15;//def:15 [0,127]
    float cutLimit = 0.01;//def:0.01  [0.0,1.0]
    float contrast = 0.9;//def:0.9 [0.0,1.0]
    int colorChannels = 3;
    int column_length = total_num_of_pixels / row_length;   // or height
    autoWhiteBalance((unsigned char*) image_buffer, (unsigned char*) output_buffer, row_length, column_length, colorChannels, row_length * colorChannels, colorCoeff, cutLimit, contrast);
}

#endif //OPTEE_SAMPLE_FILTERS_H