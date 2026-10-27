#
BUILD =	$(ROOT_DIR)/build
BUILD_DIR =	$(BUILD)/$(TARGET)
# 此工程的根目录
M_VERSION = $(shell date +"0x%Y%m%d")
    OPT_DEFS += -DAPP_VERSION=$(M_VERSION)

#加入自动版本号，为当前年+月+日期
M_BULIT_TIME = $(shell date +"%Y%m")
ELF_FILE = $(BUILD_DIR)/$(TARGET).elf
MAP_FILE = $(BUILD_DIR)/$(TARGET).map
HEX_FILE = $(BUILD_DIR)/$(TARGET)-$(strip $(M_BULIT_TIME)).hex
BIN_FILE = $(BUILD)/$(TARGET)-$(strip $(M_BULIT_TIME)).bin
UF2_FILE = $(BUILD)/$(TARGET)-$(strip $(M_BULIT_TIME)).uf2
UF2_FAMILY_ID = 0xF4604601
PYTHON ?= python3

C_SOURCES +=\
	$(APP)/main.c 

C_INCLUDES +=\
	-I$(APP)/config

ASM_SOURCES +=\
$(CORE)/Startup/startup_hc32f460.s

ifeq (HC32F460xE,$(strip $(DEVICE)))
LDSCRIPT += $(CORE)/linker/HC32F460xE.ld
else
LDSCRIPT += $(CORE)/linker/HC32F460xC.ld
endif

PREFIX = /Applications/ArmGNUToolchain/14.2.rel1/arm-none-eabi/bin/arm-none-eabi-
CC = $(PREFIX)gcc
CPP = $(PREFIX)g++
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S

CFLAGS += $(OPT_DEFS)
CFLAGS += -mcpu=cortex-m4 -mthumb -mthumb-interwork -mfloat-abi=hard -mfpu=fpv4-sp-d16 -fmessage-length=0 \
		-fsigned-char -ffunction-sections -fdata-sections -fno-common -Wall -Wno-main -Wno-comment -O2 -g3 \
		-D__DEBUG -DHC32F460 -DUSE_DDL_DRIVER $(C_INCLUDES) -std=gnu11
ASFLAGS = -mcpu=cortex-m4 -mthumb -mthumb-interwork -mfloat-abi=hard -mfpu=fpv4-sp-d16 -fmessage-length=0 \
		-fsigned-char -ffunction-sections -fdata-sections -fno-common -Wall -Wno-main -Wno-comment -O2 -g3 \
		-x assembler-with-cpp
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
ASFLAGS += -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"

ifdef CONFIG_H
CFLAGS += -DCONFIG_H_FILE=\"$(CONFIG_H)\"
endif

LDFLAGS = -mcpu=cortex-m4 -mthumb -mthumb-interwork -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O2 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wall -Wno-main -Wno-comment -g3 -T
LDFLAGS += $(LDSCRIPT) -nostartfiles -Xlinker --gc-sections -Wl,-Map,"$(MAP_FILE)" --specs=nano.specs --specs=nosys.specs -o "$(ELF_FILE)" $(OBJS) $(USER_OBJS) $(LIBS) -lm 

.PHONY: all

all: $(ELF_FILE) $(HEX_FILE) $(BIN_FILE) $(UF2_FILE)

OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))

OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c $(ROOT_DIR)/app.mk Makefile $(DRIVERS)/drivers.mk
	mkdir -p "$(@D)"
	$(CC) $(CFLAGS)

$(BUILD_DIR)/%.o: %.s $(ROOT_DIR)/app.mk Makefile $(DRIVERS)/drivers.mk
	mkdir -p "$(@D)"
	$(CC) $(ASFLAGS)

$(BUILD_DIR)/%.o: %.c $(ROOT_DIR)/app.mk Makefile $(DRIVERS)/drivers.mk | $(BUILD_DIR)
	$(CC) $(CFLAGS)

$(BUILD_DIR)/%.o: %.s $(ROOT_DIR)/app.mk Makefile $(DRIVERS)/drivers.mk | $(BUILD_DIR)
	$(CC) $(ASFLAGS)

$(BUILD_DIR):
	mkdir -p "$@"

$(ELF_FILE): $(OBJECTS) $(ROOT_DIR)/app.mk Makefile $(DRIVERS)/drivers.mk $(QMK_CORE)/quantum.mk $(QMK_CORE)/nvm/nvm.mk | $(BUILD_DIR)
	$(CC) $(LDFLAGS) $(OBJECTS) $(LIBS)
	
$(HEX_FILE): $(ELF_FILE)
	$(HEX) $< $@
	$(SZ) $<

$(BIN_FILE): $(ELF_FILE)
	$(BIN) $< $@

$(UF2_FILE): $(HEX_FILE) $(ROOT_DIR)/python/uf2conv.py
	$(PYTHON) $(ROOT_DIR)/python/uf2conv.py $< -f $(UF2_FAMILY_ID) -c -o $@

# 包含自动生成的依赖文件
-include $(OBJECTS:.o=.d)

clean:
	rm -rf $(BUILD_DIR)


