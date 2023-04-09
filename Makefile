CPP = arm-none-eabi-c++
LD = arm-none-eabi-ld
BUILD_DIR = build

TARGET = CanStepDriverPowerstep
LDSCRIPT = LowLevel/CanStepDriverPowerstep/STM32G431CBTx_FLASH.ld
LIBS = -lc -lm -lnosys -lLowLevel
LIBDIR = 

CPU = -mcpu=cortex-m4
FPU = -mfpu=fpv4-sp-d16
FLOAT-ABI = -mfloat-abi=hard
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

CPP_FLAGS = -ICoreApp -ILowLevel
CPP_FLAGS += $(MCU) -Wall -fdata-sections -ffunction-sections

CPP_SOURCES = $(wildcard CoreApp/*.cpp)

OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(CPP_SOURCES:.cpp=.o)))
vpath %.cpp $(sort $(dir $(CPP_SOURCES)))

LDFLAGS = $(MCU) -specs=nosys.specs -T$(LDSCRIPT) -L./LowLevel/CanStepDriverPowerstep $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

all: $(OBJECTS)
	$(MAKE) -C LowLevel/CanStepDriverPowerstep lib
	$(CPP) $(OBJECTS) $(LDFLAGS) -o $(BUILD_DIR)/$(TARGET).elf

clean:
	$(MAKE) -C LowLevel/CanStepDriverPowerstep clean

$(BUILD_DIR)/%.o: %.cpp Makefile | $(BUILD_DIR) 
	$(CPP) -c $(CPP_FLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.cpp=.lst)) $< -o $@

$(BUILD_DIR):
	mkdir $@
