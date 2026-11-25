TARGET := TomodachiLifeSaveEditorV2
BUILD := build
SOURCES := source
DATA := data
INCLUDES := include

APP_TITLE := Tomodachi Life Save Editor V2
APP_DESCRIPTION := Dinheiro Max no Tomodachi Life
APP_AUTHOR := Darkness Metal
APP_ICON := assets/icon.png

include $(DEVKITARM)/3ds_rules

CFLAGS := -g -Wall -O2 -mword-relocations -fomit-frame-pointer -ffunction-sections
CFLAGS += -I$(INCLUDES)
CFLAGS += -DARM11 -D_3DS

CXXFLAGS := $(CFLAGS) -fno-rtti -fno-exceptions -std=gnu++11
ASFLAGS := -g -DARM11 -D_3DS
LDFLAGS := -specs=3dsx.specs -g $(ARCH) -Wl,-Map,$(notdir $*.map)

LIBS := -lcitro2d -lcitro3d -lctru -lm

CFILES := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
SFILES := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))
BINFILES := $(foreach dir,$(DATA),$(notdir $(wildcard $(dir)/*.*)))

OFILES := $(addsuffix .o,$(BINFILES) $(CFILES) $(CPPFILES) $(SFILES))
OFILES := $(OFILES:%=$(BUILD)/%)

export OFILES

all: $(BUILD) $(TARGET).3dsx $(TARGET).smdh

$(BUILD):
	@[ -d $@ ] || mkdir -p $@

$(TARGET).3dsx: $(TARGET).elf
	@3dsxtool $< $@ --smdh=$(TARGET).smdh
	@echo "built ... $(notdir $@)"

$(TARGET).smdh: $(TARGET).elf
	@smdhtool --create "$(APP_TITLE)" "$(APP_DESCRIPTION)" "$(APP_AUTHOR)" "$(APP_ICON)" $@

$(TARGET).elf: $(OFILES)

$(BUILD)/%.o: %.c
	@echo $(notdir $<)
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/%.o: %.cpp
	@echo $(notdir $<)
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD)/%.o: %.s
	@echo $(notdir $<)
	@mkdir -p $(dir $@)
	@$(AS) $(ASFLAGS) -c $< -o $@

clean:
	@echo clean ...
	@rm -fr $(BUILD) $(TARGET).3dsx $(TARGET).smdh $(TARGET).elf
