/*
 * Copyright (c) 2016, Linaro Limited
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <tee_internal_api.h>
#include <tee_internal_api_extensions.h>
#include <stdlib.h>
#include <time.h>

#include <hello_world_ta.h>
#include <crypto.h>
#include <common.h>

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
// #include "math.h"

// Make memory related opertions easier
// #define malloc(x) TEE_malloc(x, 0)
#define realloc(x, y) TEE_Realloc(x, y)
#define free(x) TEE_Free(x)

// Decoder related definitions and variables
// #include <errno.h>
#include "libavcodec/avcodec.h"
#include "ffmpeg_decoder/decoder.h"
#define BUFFER_CAPACITY 4096*64
extern AVCodec ff_h264_decoder;
extern AVCodecParser ff_h264_parser;
AVCodec *codec;
AVCodecContext *codec_ctx;
AVCodecParserContext* parser;
AVFrame *frame;
int ending = 0;
int frame_index = 0;
uint8_t buffer[BUFFER_CAPACITY];
uint8_t* buf = buffer;
int buf_size = 0;
AVPacket packet;
u8 *byteStrm, *audio_strm, *audio_meta_strm;
unsigned char *audio_sig;
u8 *tempByteStrm; // For (moving) byteStrm pointer...
u32 readBytes;
u32 len;	// size of byteStrm
size_t size_of_audio_strm = 0, size_of_audio_meta_strm = 0, size_of_audio_sig = 0;
int is_decoding_finished = 0;
u32 status;
int numPics = 0;
size_t frame_size_in_rgb = 0;
u8* pic;
size_t pic_sig_len = 0;
u32 picId, isIdrPic, numErrMbs;
u32 top, left, width = 0, height = 0, croppingFlag;

#include "sample_filters.h"
#include "yuvconverter.h"

void fill_pixels(pixel* pixels, size_t count) {
    size_t i;
    for(i = 0; i < count; ++i) {
        pixels[i].r = rand() % 256;
        pixels[i].g = rand() % 256;
        pixels[i].b = rand() % 256;
    }
}

/* Encoder Related definitions and variables */
#include "minih264e.h"
#define DEFAULT_GOP 20
#define DEFAULT_QP 33
#define DEFAULT_DENOISE 0
#define DEFAULT_FPS 30
#define DEFAULT_IS_YUYV 0
#define DEFAULT_IS_RGB 1	// Assume we are only using it in optee
#define ENABLE_TEMPORAL_SCALABILITY 0
#define MAX_LONG_TERM_FRAMES        8 // used only if ENABLE_TEMPORAL_SCALABILITY==1
#define DEFAULT_MAX_FRAMES  99999
H264E_persist_t *enc;
H264E_scratch_t *scratch;
H264E_create_param_t create_param;
H264E_run_param_t run_param;
H264E_io_yuv_t yuv;
H264E_io_yuy2_t yuyv;
uint8_t *buf_in, *buf_save;
uint8_t *temp_buf_in, *p;
uint8_t *coded_data, *all_coded_data;
int sizeof_coded_data, frame_size, yuyv_frame_size, temp_frame_size, g_w, g_h, _qp, frame_counter = 0;
size_t total_coded_data_size;
unsigned char* total_coded_data;
cmdline* cl;
size_t processed_pixels_size;

/*
 * Called when the instance of the TA is created. This is the first call in
 * the TA.
 */
TEE_Result TA_CreateEntryPoint(void)
{
	DMSG("has been called");

	return TEE_SUCCESS;
}

/*
 * Called when the instance of the TA is destroyed if the TA has not
 * crashed or panicked. This is the last call in the TA.
 */
void TA_DestroyEntryPoint(void)
{
	DMSG("has been called");
}

/*
 * Called when a new session is opened to the TA. *sess_ctx can be updated
 * with a value to be able to identify this session in subsequent calls to the
 * TA. In this function you will normally do the global initialization for the
 * TA.
 */
TEE_Result TA_OpenSessionEntryPoint(uint32_t param_types,
		TEE_Param __maybe_unused params[4],
		void __maybe_unused **sess_ctx)
{
	uint32_t exp_param_types = TEE_PARAM_TYPES(TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE);

	DMSG("has been called");

	if (param_types != exp_param_types)
		return TEE_ERROR_BAD_PARAMETERS;

	/* Unused parameters */
	(void)&params;
	(void)&sess_ctx;

	/*
	 * The DMSG() macro is non-standard, TEE Internal API doesn't
	 * specify any means to logging from a TA.
	 */
	IMSG("Hello World!\n");

	/* If return value != TEE_SUCCESS the session will not be created. */
	return TEE_SUCCESS;
}

/*
 * Called when a session is closed, sess_ctx hold the value that was
 * assigned by TA_OpenSessionEntryPoint().
 */
void TA_CloseSessionEntryPoint(void __maybe_unused *sess_ctx)
{
	(void)&sess_ctx; /* Unused parameter */
	IMSG("Goodbye!\n");
}

static TEE_Result inc_value(uint32_t param_types,
	TEE_Param params[4])
{
	uint32_t exp_param_types = TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INOUT,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE);

	DMSG("has been called");

	if (param_types != exp_param_types)
		return TEE_ERROR_BAD_PARAMETERS;

	IMSG("Got value: %u from NW", params[0].value.a);
	params[0].value.a++;
	IMSG("Increase value to: %u", params[0].value.a);

	return TEE_SUCCESS;
}

static TEE_Result dec_value(uint32_t param_types,
	TEE_Param params[4])
{
	uint32_t exp_param_types = TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INOUT,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE);

	DMSG("has been called");

	if (param_types != exp_param_types)
		return TEE_ERROR_BAD_PARAMETERS;

	IMSG("Got value: %u from NW", params[0].value.a);
	params[0].value.a--;
	IMSG("Decrease value to: %u", params[0].value.a);

	return TEE_SUCCESS;
}

static TEE_Result custom_value(uint32_t param_types,
	TEE_Param params[4])
{
	uint32_t exp_param_types = TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INOUT,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE);

	DMSG("has been called");

	if (param_types != exp_param_types)
		return TEE_ERROR_BAD_PARAMETERS;

	IMSG("Got value: %u from NW", params[0].value.a);
	params[0].value.a = 888;
	IMSG("Make value to: %u", params[0].value.a);

	return TEE_SUCCESS;
}

static void yuv_save(unsigned char *buf[], int wrap[], int xsize,int ysize, unsigned char *target_buffer)
{
	int i;
	unsigned char* temp_target = target_buffer;	
	for (i = 0; i < ysize; i++) {
		// fwrite(buf[0] + i * wrap[0], 1, xsize, f);
		memcpy(temp_target, buf[0] + i * wrap[0], xsize);
		// if (!is_test_2_printed) {
		// 	printf("real first five chars: {%d} {%d} {%d} {%d} {%d}\n", *(buf[0] + i * wrap[0]), *(buf[0] + i * wrap[0] + 1), *(buf[0] + i * wrap[0] + 2), *(buf[0] + i * wrap[0] + 3), *(buf[0] + i * wrap[0] + 4));
		// 	printf("copied first five chars: {%d} {%d} {%d} {%d} {%d}\n", target_buffer[0], target_buffer[1], target_buffer[2], target_buffer[3], target_buffer[4]);
		// 	is_test_2_printed = 1;
		// }
		temp_target += xsize;
	}
	for (i = 0; i < ysize / 2; i++) {
		// fwrite(buf[1] + i * wrap[1], 1, xsize/2, f);
		memcpy(temp_target, buf[1] + i * wrap[1], xsize/2);
		temp_target += xsize/2;
	}
	for (i = 0; i < ysize / 2; i++) {
		// fwrite(buf[2] + i * wrap[2], 1, xsize/2, f);
		memcpy(temp_target, buf[2] + i * wrap[2], xsize/2);
		temp_target += xsize/2;
	}
}

// int is_test_printed = 0;

static int decode_write_frame(unsigned char *target_buffer, AVCodecContext *avctx,
							  AVFrame *frame, int *frame_index, AVPacket *pkt, int flush, int *is_frame_decoded)
{
	int got_frame = 0;
	do {
		int len = avcodec_decode_video2(avctx, frame, &got_frame, pkt);
		if (len < 0) {
			// fprintf(stderr, "Error while decoding frame %d\n", *frame_index);
			DMSG("[decoder:TestEnclave]: Error while decoding frame %d\n", *frame_index);
			return len;
		}
		if (got_frame) {
			// printf("Got frame %d\n", *frame_index);
			if (target_buffer) {
				size_t size_of_temp_yuv_data = sizeof(unsigned char) * frame->width * frame->height * 3 / 2;
				unsigned char *temp_yuv_data = (unsigned char*)malloc(size_of_temp_yuv_data);
				memset(temp_yuv_data, 0, size_of_temp_yuv_data);
				yuv_save(frame->data, frame->linesize, frame->width, frame->height, temp_yuv_data);
				// if (!is_test_printed) {
				// 	printf("first five chars: {%d} {%d} {%d} {%d} {%d}\n", temp_yuv_data[0], temp_yuv_data[1], temp_yuv_data[2], temp_yuv_data[3], temp_yuv_data[4]);
				// 	int total_size = sizeof(unsigned char) * frame->width * frame->height * 3 / 2;
				// 	printf("last five chars: {%d} {%d} {%d} {%d} {%d}\n", temp_yuv_data[total_size - 1], temp_yuv_data[total_size - 2], temp_yuv_data[total_size - 3], temp_yuv_data[total_size - 4], temp_yuv_data[total_size - 5]);
				// 	is_test_printed = 1;
				// }
				yuv420_prog_planar_to_rgb_packed(temp_yuv_data, target_buffer, frame->width, frame->height);
				free(temp_yuv_data);
			}
			(*frame_index)++;
		}
	} while (flush && got_frame);
	*is_frame_decoded = got_frame;
	return 0;
}

int prepare_decoder(void* input_content_buffer, long size_of_input_content_buffer) {
	// Return 1 on success, return 0 on fail

	int res = 1;

	byteStrm = (u8*)input_content_buffer;
	len = size_of_input_content_buffer;

	avcodec_register(&ff_h264_decoder);
	av_register_codec_parser(&ff_h264_parser);
	
	codec = avcodec_find_decoder(AV_CODEC_ID_H264);
	if (!codec) {
		// fprintf(stderr, "Codec not found\n");
		DMSG("[decoder:TestEnclave]: Codec not found\n");
		return 0;
	}

	codec_ctx = avcodec_alloc_context3(codec);
	if (!codec_ctx) {
		// fprintf(stderr, "Could not allocate video codec context\n");
		DMSG("[decoder:TestEnclave]: Could not allocate video codec context\n");
		return 0;
	}
	
	if (avcodec_open2(codec_ctx, codec, NULL) < 0) {
		// fprintf(stderr, "Could not open codec\n");
		DMSG("[decoder:TestEnclave]: Could not open codec\n");
		return 0;
	}
	
	parser = av_parser_init(AV_CODEC_ID_H264);
	if(!parser) {
		// fprintf(stderr, "Could not create H264 parser\n");
		DMSG("[decoder:TestEnclave]: Could not create H264 parser\n");
		return 0;
	}

	frame = av_frame_alloc();
	if (!frame) {
		// fprintf(stderr, "Could not allocate video frame\n");
		DMSG("[decoder:TestEnclave]: Could not allocate video frame\n");
		return 0;
	}

	// if (byteStrm) {
	// 	printf("[decoder:TestEnclave]: byteStrm does exist...\n");
	// }

	// For the following decoding frame process
	tempByteStrm = byteStrm;


	return res;
}

int decode_single_frame(void* decoded_frame, long size_of_decoded_frame) {
	
	// Return 0 on success; return -1 on finish all decoding; otherwise fail...

	if(is_decoding_finished){
		printf("[decoder:TestEnclave]: decoding is already finished...\n");
		return 1;
	}

	u8* decoded_frame_temp = (u8*)decoded_frame;
	memset(decoded_frame_temp, 0, size_of_decoded_frame);

	int is_single_frame_successfully_decoded = 0;

	// For some temp variables
	int res = -1;
	u8* pic_rgb = NULL;

	// printf("[decoder:TestEnclave]: Currently in t_sgxver_decode_single_frame, with remaining len: %d\n", len);

	while (len > 0 && !is_single_frame_successfully_decoded) {
		// u32 result = h264bsdDecode(&dec, byteStrm, len, 0, &readBytes);
		uint8_t* data = NULL;
  		int size = 0;
		// printf("[decoder:TestEnclave]: Going to call av_parser_parse2\n");
		readBytes = av_parser_parse2(parser, codec_ctx, &data, &size, tempByteStrm, len, 0, 0, AV_NOPTS_VALUE);
		// printf("[decoder:TestEnclave]: readBytes: [%d], frame_size: [%d]\n", readBytes, frame_size_in_rgb);

		if (readBytes > 0) {
			// Set up some parameters for the first time
			if(!frame_size_in_rgb){
				width = parser->width;
				height = parser->height;
				frame_size_in_rgb = width * height * 3;
				if(size_of_decoded_frame != frame_size_in_rgb){
					printf("[decoder:TestEnclave]: Incorrect size...size_of_decoded_frame: [%d]; frame_size_in_rgb: [%d]...\n", size_of_decoded_frame, frame_size_in_rgb);
					return 1;
				}
				InitConvt(width, height);
			}

			av_init_packet(&packet);
			packet.data = data;
			packet.size = size;
			int got_frame = 0;
			int ret = decode_write_frame(decoded_frame_temp, codec_ctx, frame, &frame_index, &packet, 0, &got_frame);
			if (ret < 0) {
				printf("Decode or write frame error\n");
				return 1;
			}

			if (got_frame) {

				// Check size of decoded_rgb_frame
				if (frame_size_in_rgb != (size_t)size_of_decoded_frame) {
					printf("[decode:TestEnclave]: Incorrect decoded_frame size...frame_size_in_rgb: [%d]; size_of_decoded_frame: [%ld]\n", frame_size_in_rgb, size_of_decoded_frame);
					return 1;
				}

				// printf("[decode:TestEnclave]: Finished cleaning for frame %d\n", tmp->frame_id);

				is_single_frame_successfully_decoded = 1;
			}

			len -= readBytes;
			tempByteStrm += readBytes;
		}

	}

	if(len <= 0){
		// printf("[decode:TestEnclave]: Decoding should be finished...going to clean...\n");
		// h264bsdShutdown(&dec);
		// Flush the decoder
		packet.data = NULL;
		packet.size = 0;
		// TO-DO: Check if possible that there is still one last frame when program gets here...
		avcodec_close(codec_ctx);
		av_free(codec_ctx);
		av_parser_close(parser);
		av_frame_free(&frame);
		is_decoding_finished = 1;

		// printf("[decode:TestEnclave]: Decoding should be finished...going to actually clean byteStrm...\n");
		// Do not need to free it in OP-TEE
		// if (byteStrm) {
		// 	free(byteStrm);
		// }
		// The cleaning of audio_strm and audio_meta_strm is performed when they are being copied out in a seperate function
		// printf("[decode:TestEnclave]: Decoding should be finished...going to actually clean audio_strm...\n");
		// if (audio_strm) {
		// 	free(audio_strm);
		// }
		// printf("[decode:TestEnclave]: Decoding should be finished...going to actually clean audio_meta_strm...\n");
		// if (audio_meta_strm) {
		// 	free(audio_meta_strm);
		// }
		
		// printf("[decode:TestEnclave]: Decoding should be finished...cleaning also finished...\n");

		return -1;
	}
	
	return 0;
}

static void psnr_init()
{
    memset(&g_psnr, 0, sizeof(g_psnr));
}

static void psnr_add(unsigned char *p0, unsigned char *p1, int w, int h, int bytes)
{
    int i, k;
    for (k = 0; k < 3; k++)
    {
        double s = 0;
        for (i = 0; i < w*h; i++)
        {
            int d = *p0++ - *p1++;
            s += d*d;
        }
        g_psnr.count[k] += w*h;
        g_psnr.noise[k] += s;
        if (!k) w >>= 1, h >>= 1;
    }
    g_psnr.count[3] = g_psnr.count[0] + g_psnr.count[1] + g_psnr.count[2];
    g_psnr.noise[3] = g_psnr.noise[0] + g_psnr.noise[1] + g_psnr.noise[2];
    g_psnr.frames++;
    g_psnr.bytes += bytes;
}

// static TEE_Result encoder_init (cmdline *cl_in, size_t cl_size, 
//                     uint8_t* frame, size_t frame_size, int height, int width) {
static TEE_Result encoder_init (uint8_t* frame, size_t frame_size, int height, int width) {
	TEE_Result ret = TEE_SUCCESS;

	// cl = (cmdline*) TEE_Malloc(sizeof(cmdline), 0);
    // memset(cl, 0, sizeof(cmdline));
    // memcpy(cl, cl_in, sizeof(cmdline));

	// Create a new cl
	cl = (cmdline*) malloc(sizeof(cmdline));
    memset(cl, 0, sizeof(cmdline));
    cl->gop = DEFAULT_GOP;
    cl->qp = DEFAULT_QP;
    cl->max_frames = DEFAULT_MAX_FRAMES;
    cl->kbps = 0;
    //cl->kbps = 2048;
    cl->denoise = DEFAULT_DENOISE;
    cl->fps = DEFAULT_FPS;
    cl->is_yuyv = DEFAULT_IS_YUYV;
    cl->is_rgb = DEFAULT_IS_RGB;

	g_h = height;
    g_w = width;
    frame_counter = 0;
	
	create_param.enableNEON = 1;
#if H264E_SVC_API
    create_param.num_layers = 1;
    create_param.inter_layer_pred_flag = 1;
    create_param.inter_layer_pred_flag = 0;
#endif
    create_param.gop = cl->gop;
    create_param.height = g_h;
    create_param.width  = g_w;
    create_param.max_long_term_reference_frames = 0;
#if ENABLE_TEMPORAL_SCALABILITY
    create_param.max_long_term_reference_frames = MAX_LONG_TERM_FRAMES;
#endif
    create_param.fine_rate_control_flag = 0;
    create_param.const_input_flag = cl->psnr ? 0 : 1;
    create_param.vbv_size_bytes = 100000/8;
    create_param.temporal_denoise_flag = cl->denoise;

	
    // Allocate space for yuv420 (the one used for actually process data)
    buf_in   = (uint8_t*) malloc(frame_size);
    memset(buf_in, 0, frame_size);
    buf_save = (uint8_t*) malloc(frame_size);
    memset(buf_save, 0, frame_size);

	// If yuyv frames are used, allocate space for both the src and temp space for converting chroma format
    if(cl->is_yuyv){
        // Allocate space for temp space
        temp_frame_size = g_w * g_h * 2;
        temp_buf_in = (uint8_t*) malloc(temp_frame_size * sizeof(uint8_t));
        memset(temp_buf_in, 0, temp_frame_size);
        // printf("yuyv detected\n");
    }

	// If rgb frames are used, allocate space for both the src and temp space for converting chroma format
    if(cl->is_rgb){
        // Allocate space for temp space of dest (yuv 4:2:0 planar)
        // Update: Probably no longer needed
        // temp_frame_size = g_w * g_h * 3 / 2;
        // temp_buf_in = (uint8_t*)malloc(temp_frame_size * sizeof(uint8_t));
        // memset(temp_buf_in, 0, temp_frame_size);
        // printf("rgb detected, init with width: %d, height: %d\n", g_w, g_h);
        // Init rgbToYuv conversion
        InitConvt(g_w, g_h);
    }

	if (!buf_in || !buf_save)
    {
        DMSG("ERROR: not enough memory\n");
        return 1;
    }
	enc = NULL;
    scratch = NULL;
    total_coded_data = NULL;
    total_coded_data_size = 0;
    int sizeof_persist = 0, sizeof_scratch = 0, error;
    if (cl->psnr)
        psnr_init();

    error = H264E_sizeof(&create_param, &sizeof_persist, &sizeof_scratch);
    if (error)
    {
        printf("H264E_sizeof error = %d\n", error);
        return 1;
    }
    enc     = (H264E_persist_t *) malloc(sizeof_persist);
    memset(enc, 0, sizeof_persist);
    scratch = (H264E_scratch_t *) malloc(sizeof_scratch);
    memset(scratch, 0, sizeof_scratch);
    error = H264E_init(enc, &create_param);
    if (error)
    {
        printf("H264E_init error = %d\n", error);
        return 1;
    }

	return ret;
}

static TEE_Result encode_frame (uint8_t* frame, size_t frame_size) {
	TEE_Result ret = TEE_SUCCESS;

	int fps = cl->fps;
    frame_counter++;

	// Encode frame
    if (cl->is_yuyv) {
        p = frame;   // Record head adddress

        // temp conversion address
        yuyv.Y = temp_buf_in;
        yuyv.U = yuyv.Y + g_w * g_h;
        yuyv.V = yuyv.U + (g_w * g_h >> 1);   // Y  U  V  =4 : 2 ; 2

        // final incoming yuv data address
        yuv.yuv[0] = buf_in; yuv.stride[0] = g_w;
        yuv.yuv[1] = buf_in + g_w*g_h; yuv.stride[1] = g_w/2;
        yuv.yuv[2] = buf_in + g_w*g_h*5/4; yuv.stride[2] = g_w/2;

        // yuyv to yuv
        int k, j;
        for (k = 0; k < g_h; ++k)
        {
            for (j = 0; j < (g_w >> 1); ++j)
            {
                yuyv.Y[j * 2] = p[4 * j];
                yuyv.U[j] = p[4 * j + 1];
                yuyv.Y[j * 2 + 1] = p[4 * j + 2];
                yuyv.V[j] = p[4 * j + 3];
            }
            p = p + g_w * 2;

            yuyv.Y = yuyv.Y + g_w;
            yuyv.U = yuyv.U + (g_w >> 1);
            yuyv.V = yuyv.V + (g_w >> 1);
        }
        // Now packed is planar
        // reset
        yuyv.Y = temp_buf_in;
        yuyv.U = yuyv.Y + g_w * g_h;
        yuyv.V = yuyv.U + (g_w * g_h >> 1);

        int l;
        for (l = 0; l < g_h / 2; ++l)
        {
            memcpy(yuv.yuv[1], yuyv.U, g_w >> 1);
            memcpy(yuv.yuv[2], yuyv.V, g_w >> 1);

            yuv.yuv[1] = yuv.yuv[1] + (g_w >> 1);
            yuv.yuv[2] = yuv.yuv[2] + (g_w >> 1);

            yuyv.U = yuyv.U + (g_w);
            yuyv.V = yuyv.V + (g_w);
        }

        memcpy(yuv.yuv[0], yuyv.Y, g_w * g_h);

        // reset
        yuv.yuv[0] = buf_in;
        yuv.yuv[1] = buf_in + g_w*g_h;
        yuv.yuv[2] = buf_in + g_w*g_h*5/4;
    } else if (cl->is_rgb) {
        // printf("Processing rgb frame with frame size: %d...\n", frame_size);
        rgb_packed_to_yuv420_prog_planar(frame, buf_in, g_w, g_h);
        yuv.yuv[0] = buf_in; yuv.stride[0] = g_w;
        yuv.yuv[1] = buf_in + g_w*g_h; yuv.stride[1] = g_w/2;
        yuv.yuv[2] = buf_in + g_w*g_h*5/4; yuv.stride[2] = g_w/2;
    } else {
        buf_in = frame;
        yuv.yuv[0] = buf_in; yuv.stride[0] = g_w;
        yuv.yuv[1] = buf_in + g_w*g_h; yuv.stride[1] = g_w/2;
        yuv.yuv[2] = buf_in + g_w*g_h*5/4; yuv.stride[2] = g_w/2;
    }

    // For printing psnr
    if (cl->psnr)
        memcpy(buf_save, buf_in, frame_size);

    run_param.frame_type = 0;
    run_param.encode_speed = cl->speed;
    run_param.target_fps = fps;
    //run_param.desired_nalu_bytes = 100;

    if (cl->kbps)
    {
        printf("kbps is set manually to %i\n", cl->kbps);
        run_param.desired_frame_bytes = cl->kbps*1000/8/30;    // Modified for framerates
        run_param.qp_min = 10;
        run_param.qp_max = 50;
    } else
    {
        run_param.qp_min = run_param.qp_max = cl->qp;
    }

#if ENABLE_TEMPORAL_SCALABILITY
    int level, logmod = 1;
    int j, mod = 1 << logmod;
    static int fresh[200] = {-1,-1,-1,-1};

    run_param.frame_type = H264E_FRAME_TYPE_CUSTOM;

    for (level = logmod; level && (~i & (mod >> level)); level--){}

    run_param.long_term_idx_update = level + 1;
    if (level == logmod && logmod > 0)
        run_param.long_term_idx_update = -1;
    if (level == logmod - 1 && logmod > 1)
        run_param.long_term_idx_update = 0;

    //if (run_param.long_term_idx_update > logmod) run_param.long_term_idx_update -= logmod+1;
    //run_param.long_term_idx_update = logmod - 0 - level;
    //if (run_param.long_term_idx_update > 0)
    //{
    //    run_param.long_term_idx_update = logmod - run_param.long_term_idx_update;
    //}
    run_param.long_term_idx_use    = fresh[level];
    for (j = level; j <= logmod; j++)
    {
        fresh[j] = run_param.long_term_idx_update;
    }
    if (!i)
    {
        run_param.long_term_idx_use = -1;
    }
#endif
    int res = H264E_encode(enc, scratch, &run_param, &yuv, &coded_data, &sizeof_coded_data);
    if (res)
    {
        DMSG("[EncoderEnclave]: t_encode_frame: ERROR during encoding: %d\n", res);
        return 1;
    }

    if (cl->psnr)
        psnr_add(buf_save, buf_in, g_w, g_h, sizeof_coded_data);

    // Save encoded frame to global variable
    unsigned char* tmp;
    tmp = (unsigned char*) TEE_Realloc(total_coded_data, (size_t)(total_coded_data_size + sizeof_coded_data));
    if (tmp)
    {
        memset(tmp + total_coded_data_size, 0, sizeof_coded_data);
        memcpy(tmp + total_coded_data_size, coded_data, sizeof_coded_data);
        total_coded_data_size += sizeof_coded_data;
        // DMSG("[EncoderEnclave]: Now has total_coded_data_size: %d\n", total_coded_data_size);
        total_coded_data = tmp;
    }
    else
    {
        DMSG("[EncoderEnclave]: t_encode_frame: ERROR no memory available\n");
        return 1;
    }

	return ret;
}

static TEE_Result sample_filter_test(uint32_t param_types,
	TEE_Param params[4], uint32_t cmd_id)
{
	uint32_t exp_param_types = TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INOUT,
						   TEE_PARAM_TYPE_VALUE_INOUT,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE);

	DMSG("has been called");

	if (param_types != exp_param_types)
		return TEE_ERROR_BAD_PARAMETERS;

	int image_width = params[0].value.a;
	int image_height = params[0].value.b;
	int num_of_frames = params[1].value.a;
	int with_sign_verify = params[1].value.b;

	IMSG("sample_filter_test: image_width: %d, image_height: %d, sizeof_pixel: %d, num_of_frames: %d, with_sign_verify: %d\n", image_width, image_height, sizeof(pixel), num_of_frames, with_sign_verify);

	// srand(time(NULL));

	// Prepare signing and verifying operations
	TEE_Result ret;
	TEE_ObjectHandle fixed_rsa_keypair = (TEE_ObjectHandle)NULL;
	TEE_ObjectHandle rsa_keypair = (TEE_ObjectHandle)NULL;
	// size_t key_size = 512;	// For new key
	size_t key_size = 1024;	// For fixed key
	uint32_t rsa_alg = TEE_ALG_RSASSA_PKCS1_V1_5_SHA1;
	char hash[64] = {0}; /*sha1*/
	char sig[512] = {0};
	uint32_t sig_len = 512, hash_len = 64;
	TEE_Attribute rsa_attrs[3];

	if (with_sign_verify) {
		/* Modulo */
		rsa_attrs[0].attributeID = TEE_ATTR_RSA_MODULUS;
		rsa_attrs[0].content.ref.buffer = modulus;
		rsa_attrs[0].content.ref.length = SIZE_OF_VEC(modulus);

		/* Public exp */
		rsa_attrs[1].attributeID = TEE_ATTR_RSA_PUBLIC_EXPONENT;
		rsa_attrs[1].content.ref.buffer = public_exp;
		rsa_attrs[1].content.ref.length = SIZE_OF_VEC(public_exp);

		/* Private exp */
		rsa_attrs[2].attributeID = TEE_ATTR_RSA_PRIVATE_EXPONENT;
		rsa_attrs[2].content.ref.buffer = private_exp;
		rsa_attrs[2].content.ref.length = SIZE_OF_VEC(private_exp);

		ret = TEE_AllocateTransientObject(TEE_TYPE_RSA_KEYPAIR, key_size, &fixed_rsa_keypair);
		if (ret != TEE_SUCCESS) {
			DMSG("Failed to alloc transient fixed_rsa_keypair handle : 0x%x", ret);
			return TEE_ERROR_BAD_PARAMETERS;
		}
		ret = TEE_AllocateTransientObject(TEE_TYPE_RSA_KEYPAIR, key_size, &rsa_keypair);
		if (ret != TEE_SUCCESS) {
			DMSG("Failed to alloc transient rsa_keypair handle : 0x%x", ret);
			return TEE_ERROR_BAD_PARAMETERS;
		}
		ret = TEE_PopulateTransientObject(fixed_rsa_keypair, (TEE_Attribute *)&rsa_attrs, 3);	// Use fixed key pair
		if (ret != TEE_SUCCESS) {
			DMSG("Read key failure : 0x%x", ret);
			return TEE_ERROR_BAD_PARAMETERS;
		}
		ret = TEE_GenerateKey(rsa_keypair, key_size, (TEE_Attribute *)NULL, 0);	// Generate new key pair
		if (ret != TEE_SUCCESS) {
			DMSG("Generate key failure : 0x%x", ret);
			return TEE_ERROR_BAD_PARAMETERS;
		}

		uint8_t modulus_ex[258];
		uint32_t modulus_ex_len = sizeof(modulus);
		uint8_t pubexp_ex[258];
		uint32_t pubexp_ex_len = sizeof(modulus);
		ret = TEE_GetObjectBufferAttribute(rsa_keypair, TEE_ATTR_RSA_MODULUS, (void*) modulus_ex, &modulus_ex_len);
		if (ret != TEE_SUCCESS) {
			DMSG("TEE_GetObjectBufferAttribute failed for modulus_ex! TEE_GetObjectBufferAttribute res: 0x%x", ret);
			return TEE_ERROR_BAD_PARAMETERS;
		}
		ret = TEE_GetObjectBufferAttribute(rsa_keypair, TEE_ATTR_RSA_PUBLIC_EXPONENT, pubexp_ex, &pubexp_ex_len);
		if (ret != TEE_SUCCESS) {
			DMSG("TEE_GetObjectBufferAttribute failed for pubexp_ex! TEE_GetObjectBufferAttribute res: 0x%x", ret);
			return TEE_ERROR_BAD_PARAMETERS;
		}
		IMSG("RSA_MODULUS %d bytes\n", (int)modulus_ex_len);
   		IMSG("RSA_PUBLIC_EXPONENT %d bytes\n", (int)pubexp_ex_len);
		// To-do: Use fixed key to sign the new key
	}

	for (int i = 0; i < num_of_frames; ++i) {
		pixel *input_image_buffer, *output_image_buffer;
		processed_pixels_size = sizeof(pixel) * image_height * image_width;

		input_image_buffer = (pixel*)malloc(processed_pixels_size);
		// fill_pixels(input_image_buffer, processed_pixels_size);
		TEE_GenerateRandom(input_image_buffer, processed_pixels_size);
		output_image_buffer = (pixel*)malloc(processed_pixels_size);
		
		// Do processing accordingly
		switch (cmd_id) {
			case TA_HELLO_WORLD_CMD_SAMPLE_BLUR_FILTER_TEST:
				blur((pixel*)input_image_buffer, output_image_buffer, image_width, image_width * image_height, 7);
				break;
			case TA_HELLO_WORLD_CMD_SAMPLE_SHARPEN_FILTER_TEST:
				sharpen((pixel*)input_image_buffer, output_image_buffer, image_width, image_width * image_height, 7);
				break;
			case TA_HELLO_WORLD_CMD_SAMPLE_BRIGHTNESS_FILTER_TEST:
				change_brightness((pixel*)input_image_buffer, output_image_buffer, image_width, image_width * image_height, 0.5);
				break;
			case TA_HELLO_WORLD_CMD_SAMPLE_DENOISE_SIMPLE_FILTER_TEST:
				denoise_simple((pixel*)input_image_buffer, output_image_buffer, image_width, image_width * image_height);
				break;
			case TA_HELLO_WORLD_CMD_SAMPLE_GRAY_FILTER_TEST:
				gray_frame((pixel*)input_image_buffer, output_image_buffer, image_width, image_width * image_height);
				break;
			case TA_HELLO_WORLD_CMD_SAMPLE_WHITE_BALANCE_FILTER_TEST:
				auto_white_balance_pure_c((pixel*)input_image_buffer, output_image_buffer, image_width, image_width * image_height);
				break;
			case TA_HELLO_WORLD_CMD_SAMPLE_FRAME_DELETION_FILTER_TEST:
				memset(output_image_buffer, 0, processed_pixels_size);
				break;
			case TA_HELLO_WORLD_CMD_SAMPLE_C1_FILTER_TEST:
				sharpen((pixel*)input_image_buffer, output_image_buffer, image_width, image_width * image_height, 7);
				auto_white_balance_pure_c(output_image_buffer, (pixel*)input_image_buffer, image_width, image_width * image_height);
				denoise_simple((pixel*)input_image_buffer, output_image_buffer, image_width, image_width * image_height);
				break;
			case TA_HELLO_WORLD_CMD_SAMPLE_C2_FILTER_TEST:
				blur((pixel*)input_image_buffer, output_image_buffer, image_width, image_width * image_height, 7);
				change_brightness(output_image_buffer, (pixel*)input_image_buffer, image_width, image_width * image_height, 0.5);
				gray_frame((pixel*)input_image_buffer, output_image_buffer, image_width, image_width * image_height);
				break;
			default:
				return TEE_ERROR_NOT_IMPLEMENTED;
		}

		// Some cleaning
		TEE_Free(input_image_buffer);

		if (with_sign_verify) {
			if (calc_digest(TEE_ALG_SHA1, output_image_buffer, processed_pixels_size, hash, &hash_len)) {
				DMSG("Hash failed : 0x%x", ret);
				return TEE_ERROR_BAD_PARAMETERS;
			}

			if (warp_asym_op(rsa_keypair, TEE_MODE_SIGN, rsa_alg, (TEE_Attribute *)NULL, 0, hash, hash_len, sig, &sig_len)) {
				DMSG("Sign failed : 0x%x", ret);
				return TEE_ERROR_BAD_PARAMETERS;
			}

			if (warp_asym_op(rsa_keypair, TEE_MODE_VERIFY, rsa_alg, (TEE_Attribute *)NULL, 0, hash, hash_len, sig, &sig_len)) {
				DMSG("Verify failed : 0x%x", ret);
				return TEE_ERROR_BAD_PARAMETERS;
			}
		}

		// Start of encoding
		if (frame_counter == 0) {	// Do not edit frame_counter, as it will grow accordingly
			ret = encoder_init((uint8_t*)output_image_buffer, processed_pixels_size, image_height, image_width);
			if (ret != TEE_SUCCESS) {
				DMSG("Encoder init failed...");
				return TEE_ERROR_BAD_PARAMETERS;
			}
		}

		ret = encode_frame((uint8_t*)output_image_buffer, processed_pixels_size);
		if (ret != TEE_SUCCESS) {
			DMSG("Frame encoding failed at potentially frame: %d...", frame_counter);
			return TEE_ERROR_BAD_PARAMETERS;
		}
		// End of encoding

		// Some more cleaning
		TEE_Free(output_image_buffer);
	}

	// Try clean some space for decoder eval
	if (with_sign_verify) {
		TEE_FreeTransientObject(rsa_keypair);
	}
	if (enc)
        free(enc);
    if (scratch)
        free(scratch);
	if (buf_in)
        free(buf_in);
    if (buf_save)
        free(buf_save);
	if (cl->is_yuyv) {
        if (temp_buf_in){
            printf("free memory for yuyv\n");
            free(temp_buf_in);
        }
    }
	if (cl)
        free(cl);

	// Decoding
	if (!prepare_decoder(total_coded_data, total_coded_data_size)) {
		DMSG("Decoder prepare failed...");
		return TEE_ERROR_BAD_PARAMETERS;
	}
	u8* single_frame_buf = (u8*) malloc(processed_pixels_size);
	for (int i = 0; i < frame_counter; ++i) {
		// IMSG("Decoding frame: %d", i);
		decode_single_frame(single_frame_buf, processed_pixels_size);
	}

	IMSG("Decoding finished...");

	// More cleaning
	if (total_coded_data)
        free(total_coded_data);
	if (single_frame_buf) {
		free(single_frame_buf);
	}

	return TEE_SUCCESS;
}

/*
 * Called when a TA is invoked. sess_ctx hold that value that was
 * assigned by TA_OpenSessionEntryPoint(). The rest of the paramters
 * comes from normal world.
 */
TEE_Result TA_InvokeCommandEntryPoint(void __maybe_unused *sess_ctx,
			uint32_t cmd_id,
			uint32_t param_types, TEE_Param params[4])
{
	(void)&sess_ctx; /* Unused parameter */

	// Below line is just for testing
	// params[0].value.a = cmd_id;

	switch (cmd_id) {
		case TA_HELLO_WORLD_CMD_CUSTOM_TEST:
			return custom_value(param_types, params);
		case TA_HELLO_WORLD_CMD_SAMPLE_BLUR_FILTER_TEST:
			return sample_filter_test(param_types, params, cmd_id);
		case TA_HELLO_WORLD_CMD_SAMPLE_SHARPEN_FILTER_TEST:
			return sample_filter_test(param_types, params, cmd_id);
		case TA_HELLO_WORLD_CMD_SAMPLE_BRIGHTNESS_FILTER_TEST:
			return sample_filter_test(param_types, params, cmd_id);
		case TA_HELLO_WORLD_CMD_SAMPLE_DENOISE_SIMPLE_FILTER_TEST:
			return sample_filter_test(param_types, params, cmd_id);
		case TA_HELLO_WORLD_CMD_SAMPLE_GRAY_FILTER_TEST:
			return sample_filter_test(param_types, params, cmd_id);
		case TA_HELLO_WORLD_CMD_SAMPLE_WHITE_BALANCE_FILTER_TEST:
			return sample_filter_test(param_types, params, cmd_id);
		case TA_HELLO_WORLD_CMD_SAMPLE_FRAME_DELETION_FILTER_TEST:
			return sample_filter_test(param_types, params, cmd_id);
		case TA_HELLO_WORLD_CMD_SAMPLE_C1_FILTER_TEST:
			return sample_filter_test(param_types, params, cmd_id);
		case TA_HELLO_WORLD_CMD_SAMPLE_C2_FILTER_TEST:
			return sample_filter_test(param_types, params, cmd_id);
		default:
			return TEE_ERROR_NOT_IMPLEMENTED;
	}
}
