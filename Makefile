# Tomodachi Life Save Editor V2 by Darkness Metal
# Makefile OFICIAL devkitPro - SEMPRE .3dsx + .smdh + .cia
# Arquivo: Makefile - colar na RAIZ - REVISADO 5x
.SUFFIXES:

ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif
include $(DEVKITARM)/3ds_rules

TARGET := TomodachiLifeSaveEditorV2
BUILD := build
SOURCES := source
DATA := data
INCLUDES := include
GRAPHICS := gfx
ROMFS := romfs
GFXBUILD := $(ROMFS)/gfx
APP_TITLE := Tomodachi Life Save Editor V2
APP_DESCRIPTION := Tomodachi Life Save Editor V2 by Darkness Metal
APP_AUTHOR := Darkness Metal
APP_ICON := assets/icon.png
APP_BANNER := assets/banner.png
APP_AUDIO := assets/sound.wav
ICON := assets/icon.png
BANNER := assets/banner.png
BANNER_AUDIO := assets/sound.wav
MAKEROM := makerom
BANNERTOOL := bannertool

ARCH := -march=armv6k -mtune=mpcore -mfloat-abi=hard -mtp=soft
CFLAGS := -g -Wall -O2 -mword-relocations \
	-ffunction-sections \
	$(ARCH)
CFLAGS += $(INCLUDE) -D__3DS__
CXXFLAGS := $(CFLAGS) -fno-rtti -fno-exceptions -std=gnu++11
ASFLAGS := -g $(ARCH)
LDFLAGS = -specs=3dsx.specs -g $(ARCH) -Wl,-Map,$(notdir $*.map)
LIBS := -lcitro2d -lcitro3d -lctru -lm
LIBDIRS := $(CTRULIB) $(PORTLIBS)

ifneq ($(BUILD),$(notdir $(CURDIR)))
export OUTPUT := $(CURDIR)/$(TARGET)
export TOPDIR := $(CURDIR)
export VPATH := $(foreach dir,$(SOURCES),$(CURDIR)/$(dir)) \
	$(foreach dir,$(DATA),$(CURDIR)/$(dir)) \
	$(foreach dir,$(GRAPHICS),$(CURDIR)/$(dir))
export DEPSDIR := $(CURDIR)/$(BUILD)
CFILES := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
SFILES := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))
PICAFILES := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.v.pica)))
SHLISTFILES := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.shlist)))
GFXFILES := $(foreach dir,$(GRAPHICS),$(notdir $(wildcard $(dir)/*.t3s)))
BINFILES := $(foreach dir,$(DATA),$(notdir $(wildcard $(dir)/*.*)))
ifeq ($(strip $(CPPFILES)),)
export LD := $(CC)
else
export LD := $(CXX)
endif
export OFILES := $(addsuffix .o,$(BINFILES)) \
	$(PICAFILES:.v.pica=.o) $(SHLISTFILES:.shlist=.o) $(GFXFILES:.t3s=.o) \
	$(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(SFILES:.s=.o)
export INCLUDE := $(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
	$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
	-I$(CURDIR)/$(BUILD)
export LIBPATHS := $(foreach dir,$(LIBDIRS),-L$(dir)/lib)
.PHONY: $(BUILD) clean all
all: $(BUILD)
$(BUILD):
	@[ -d $@ ] || mkdir -p $@
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile
clean:
	@echo clean ...
	@rm -fr $(BUILD) $(TARGET).3dsx $(OUTPUT).smdh $(TARGET).elf $(TARGET).cia banner.bnr icon.icn
else
DEPENDS := $(OFILES:.o=.d)
.PHONY: all
all: $(OUTPUT).3dsx $(OUTPUT).smdh $(OUTPUT).cia
$(OUTPUT).3dsx : $(OUTPUT).elf
$(OUTPUT).elf : $(OFILES)
$(OUTPUT).smdh : $(OUTPUT).elf
banner.bnr: $(TOPDIR)/assets/banner.png $(TOPDIR)/assets/sound.wav
	@$(BANNERTOOL) makebanner -i $(TOPDIR)/assets/banner.png -a $(TOPDIR)/assets/sound.wav -o banner.bnr
icon.icn: $(TOPDIR)/assets/icon.png
	@$(BANNERTOOL) makesmdh -s "$(APP_TITLE)" -l "$(APP_TITLE)" -p "$(APP_AUTHOR)" -i $(TOPDIR)/assets/icon.png -o icon.icn
$(OUTPUT).cia: $(OUTPUT).elf banner.bnr icon.icn
	@$(MAKEROM) -f cia -o $@ -rsf $(TOPDIR)/cia.rsf -target t -exefslogo -elf $(OUTPUT).elf -icon icon.icn -banner banner.bnr
%.bin.o : %.bin
	@echo $(notdir $<)
	@$(bin2o)
-include $(DEPENDS)
endif
