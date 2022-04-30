/*
 * Copyright (c) 2016-2017, Linaro Limited
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
#ifndef TA_HELLO_WORLD_H
#define TA_HELLO_WORLD_H


/*
 * This UUID is generated with uuidgen
 * the ITU-T UUID generator at http://www.itu.int/ITU-T/asn1/uuid.html
 */
#define TA_HELLO_WORLD_UUID \
	{ 0x8aaaf200, 0x2450, 0x11e4, \
		{ 0xab, 0xe2, 0x00, 0x02, 0xa5, 0xd5, 0xc5, 0x1b} }

/* The function IDs implemented in this TA */
// Note: For some reason currently I don't know, only at most 3 functions are allowed here...
// #define TA_HELLO_WORLD_CMD_INC_VALUE				0
// #define TA_HELLO_WORLD_CMD_DEC_VALUE				1
#define TA_HELLO_WORLD_CMD_CUSTOM_TEST										0
#define TA_HELLO_WORLD_CMD_SAMPLE_BLUR_FILTER_TEST							1
#define TA_HELLO_WORLD_CMD_SAMPLE_SHARPEN_FILTER_TEST						2
#define TA_HELLO_WORLD_CMD_SAMPLE_BRIGHTNESS_FILTER_TEST					3
#define TA_HELLO_WORLD_CMD_SAMPLE_DENOISE_SIMPLE_FILTER_TEST				4
#define TA_HELLO_WORLD_CMD_SAMPLE_GRAY_FILTER_TEST							5
#define TA_HELLO_WORLD_CMD_SAMPLE_WHITE_BALANCE_FILTER_TEST					6
#define TA_HELLO_WORLD_CMD_SAMPLE_FRAME_DELETION_FILTER_TEST				7
#define TA_HELLO_WORLD_CMD_SAMPLE_C1_FILTER_TEST							8
#define TA_HELLO_WORLD_CMD_SAMPLE_C2_FILTER_TEST							9

#endif /*TA_HELLO_WORLD_H*/
