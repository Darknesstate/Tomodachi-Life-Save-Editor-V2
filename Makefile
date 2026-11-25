TARGET := TomodachiLifeSaveEditorV2
BUILD := build
SOURCES := source
DATA := data
INCLUDES := include

APP_TITLE := Tomodachi Life Save Editor V2
APP_DESCRIPTION := Dinheiro Max no Tomodachi Life
APP_AUTHOR := Darkness Metal
APP_ICON := icon.png

ARCH := -march=armv6k -mtune=mpcore -mfloat-abi=hard -mtp=soft

CFLAGS := -g -O2 -w -mword-relocations -ffunction-sections -fdata-sections $(ARCH)
CFLAGS += -Wno-error -Wno-format -Wno-int-conversion -Wno-misleading-indentation -Wno-unused-parameter -Wno-unused-variable
CFLAGS += -D__3DS__ -DARM11 -D_3DS
CFLAGS += -I$(INCLUDES)

CXXFLAGS := $(CFLAGS) -fno-rtti -fno-exceptions -std=gnu++17
ASFLAGS := -g $(ARCH)
LDFLAGS = -specs=3dsx.specs -g $(ARCH) -Wl,-Map,$(notdir $*.map)
LIBS := -lctru -lm

include $(DEVKITARM)/3ds_rules

CFILES := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
SFILES := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))
BINFILES := $(foreach dir,$(DATA),$(notdir $(wildcard $(dir)/*.*)))

OFILES_BIN := $(addsuffix .o,$(BINFILES))
OFILES_SOURCES := $(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(SFILES:.s=.o)
OFILES := $(OFILES_BIN) $(OFILES_SOURCES)
OFILES := $(OFILES:%=$(BUILD)/%)
export OFILES

VPATH := $(foreach dir,$(SOURCES),$(CURDIR)/$(dir)) $(foreach dir,$(DATA),$(CURDIR)/$(dir))

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

$(BUILD)/%.o: %.bin
	@echo $(notdir $<)
	@mkdir -p $(dir $@)
	@$(bin2o)

clean:
	@echo clean ...
	@rm -fr $(BUILD) $(TARGET).3dsx $(TARGET).smdh $(TARGET).elf
