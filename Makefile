# Tomodachi Life Save Editor V2 by Darkness Metal
# CORRIGIDO: so .3dsx primeiro, sem .cia no all, sem -mtp=soft no LDFLAGS
# Arquivo: Makefile - colar na RAIZ

TARGET		:=	TomodachiLifeSaveEditorV2
BUILD		:=	build
SOURCES		:=	source
DATA		:=	data
INCLUDES	:=	include
ROMFS		:=	romfs

APP_TITLE		:=	Tomodachi Life Save Editor V2
APP_DESCRIPTION	:=	Tomodachi Life Save Editor V2 by Darkness Metal
APP_AUTHOR		:=	Darkness Metal
APP_ICON		:=	assets/icon.png
APP_BANNER		:=	assets/banner.png
APP_AUDIO		:=	assets/sound.wav

ARCH	:=	-march=armv6k -mtune=mpcore -mfloat-abi=hard -mtp=soft

CFLAGS	:=	-g -Wall -O2 -mword-relocations \
			-ffunction-sections \
			$(ARCH)

CFLAGS	+=	$(INCLUDE) -DARM11 -D_3DS

CXXFLAGS	:=	$(CFLAGS) -fno-rtti -fno-exceptions -std=gnu++11

ASFLAGS	:=	-g $(ARCH)

# CORRIGIDO: sem $(ARCH) aqui, era isso que dava tp=soft no ld
LDFLAGS	=	-specs=3dsx.specs -g -Wl,-Map,$(notdir $*.map)

LIBS	:=	-lcitro2d -lcitro3d -lctru -lm

ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

include $(DEVKITARM)/3ds_rules

.PHONY: all clean

# CORRIGIDO: so .3dsx + .smdh no all, .cia vem depois que o .3dsx funcionar
all: $(TARGET).3dsx $(TARGET).smdh

clean:
	@echo clean ...
	@rm -fr $(BUILD) $(TARGET).3dsx $(TARGET).smdh $(TARGET).cia $(TARGET).elf
