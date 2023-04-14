CPP = arm-none-eabi-c++
LD = arm-none-eabi-ld
SZ = arm-none-eabi-size
CP = arm-none-eabi-objcopy
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S

BUILD_DIR = build

TARGET = CanStepDriverPowerstep
LDSCRIPT = LowLevel/CanStepDriverPowerstep/STM32G431CBTx_FLASH.ld
LIBS = -lc -lm -lnosys -lCanStepDriverPowerstep
LIBDIR = 

MCU = -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard

CPP_FLAGS = -ICoreApp -ILowLevel
CPP_FLAGS += $(MCU) -Wall -fdata-sections -ffunction-sections

CPP_SOURCES = $(wildcard CoreApp/*.cpp)

OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(CPP_SOURCES:.cpp=.o)))
vpath %.cpp $(sort $(dir $(CPP_SOURCES)))

LDFLAGS = $(MCU) -specs=nosys.specs -T$(LDSCRIPT) -L./LowLevel/CanStepDriverPowerstep $(LIBDIR) $(LIBS)
LDFLAGS += -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections -Wl,--print-memory-usage

#    text    data     bss     dec     hex filename
#   10612    1096    1852   13560    34f8 build/CanStepDriverPowerstep.elf

            #  RAM:        2936 B        32 KB      8.96%
        #    FLASH:       11708 B       128 KB      8.93%

all: $(OBJECTS)
	$(MAKE) -C LowLevel/CanStepDriverPowerstep lib
	$(CPP) $(OBJECTS) $(LDFLAGS) -o $(BUILD_DIR)/$(TARGET).elf
	$(HEX) $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex
	$(BIN) $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).bin
	$(SZ) $(BUILD_DIR)/$(TARGET).elf

clean:
	rm -f $(BUILD_DIR)
	$(MAKE) -C LowLevel/CanStepDriverPowerstep clean

$(BUILD_DIR)/%.o: %.cpp Makefile | $(BUILD_DIR) 
	$(CPP) -c $(CPP_FLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.cpp=.lst)) $< -o $@

$(BUILD_DIR):
	mkdir $@
