CFG_TEE_TA_LOG_LEVEL ?= 4

# Added to link with FFMPEG H264 decoder libraries
F_DECODER_SRCS := $(shell find ./ffmpeg_decoder -name "*.c" -not -name "*_template.c")
F_DECODER_INCLUDE_PATH := ./ffmpeg_decoder/ffmpeg-src

CPPFLAGS += -DCFG_TEE_TA_LOG_LEVEL=$(CFG_TEE_TA_LOG_LEVEL) -static \
-Wall -Wextra -ffast-math -fno-stack-protector -fomit-frame-pointer -ffunction-sections -fdata-sections -Wl,--gc-sections -ftree-vectorize \
-lm -lpthread -I$(F_DECODER_INCLUDE_PATH) -w -std=c99 -Wno-implicit-function-declaration

# The UUID for the Trusted Application
BINARY=8aaaf200-2450-11e4-abe2-0002a5d5c51b

-include $(TA_DEV_KIT_DIR)/mk/ta_dev_kit.mk

ifeq ($(wildcard $(TA_DEV_KIT_DIR)/mk/ta_dev_kit.mk), )
clean:
	@echo 'Note: $$(TA_DEV_KIT_DIR)/mk/ta_dev_kit.mk not found, cannot clean TA'
	@echo 'Note: TA_DEV_KIT_DIR=$(TA_DEV_KIT_DIR)'
endif
