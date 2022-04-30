# Added to link with FFMPEG H264 decoder libraries
F_DECODER_SRCS := $(shell find ./ffmpeg_decoder -name "*.c" -not -name "*_template.c")
F_DECODER_INCLUDE_PATH := ./ffmpeg_decoder/ffmpeg-src

global-incdirs-y += include
global-incdirs-y += $(F_DECODER_INCLUDE_PATH)
srcs-y += hello_world_ta.c
srcs-y += $(F_DECODER_SRCS)

# To remove a certain compiler flag, add a line like this
#cflags-template_ta.c-y += -Wno-strict-prototypes
# libnames += m
# libdirs += $(shell pwd)
