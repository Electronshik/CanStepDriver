# template was taken from https://gist.github.com/elmot/3b4f0e9f8b23864fdf8bb509c329d051

cmake_minimum_required(VERSION 3.17)
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)

set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_OBJDUMP arm-none-eabi-objdump)
set(SIZE arm-none-eabi-size)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

project(CanStepDriver)
set(PROJECT_BINARY_DIR build)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

# set(TARGET CanStepDriverPowerstep)
# set(TARGET CanStepDriverMosfets)
set(TARGET CanStepDriverF429)

message(TARG="$ENV{TARG}")

set(MCPU cortex-m4)

add_compile_definitions(ARM_MATH_CM4; ARM_MATH_MATRIX_CHECK; ARM_MATH_ROUNDING)
add_compile_options(-mfloat-abi=hard -mfpu=fpv4-sp-d16)
add_compile_options(-mcpu=${MCPU} -mthumb -mthumb-interwork)
add_compile_options(-ffunction-sections -fdata-sections -fno-common -fmessage-length=0)

# set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/LowLevel/${TARGET}/STM32G431CBTx_FLASH.ld)
set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/LowLevel/${TARGET}/STM32F429ZITx_FLASH.ld)

add_link_options(-mfloat-abi=hard -mfpu=fpv4-sp-d16)
add_link_options(-Wl,-gc-sections,--print-memory-usage,-Map=${PROJECT_SOURCE_DIR}/${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map)
add_link_options(-mcpu=${MCPU} -mthumb -mthumb-interwork)
add_link_options(-specs=nosys.specs -T ${LINKER_SCRIPT})
# add_link_options(-L${PROJECT_SOURCE_DIR}/LowLevel/CanStepDriverPowerstep -lc -lm -lnosys -lLowLevel)
add_link_options(-lc -lm -lnosys)
# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Map=${PROJECT_SOURCE_DIR}/build/${PROJECT_NAME}.map")

file(GLOB_RECURSE SOURCES
	"CoreApp/*.*"
)

include_directories(
	${CMAKE_SOURCE_DIR}/CoreApp
	${CMAKE_SOURCE_DIR}/LowLevel
)

add_executable(${TARGET}.elf ${SOURCES} ${LINKER_SCRIPT})

target_link_libraries(${TARGET}.elf ${PROJECT_SOURCE_DIR}/LowLevel/${TARGET}/lib${TARGET}.a)

set(HEX_FILE ${PROJECT_BINARY_DIR}/${TARGET}.hex)
set(BIN_FILE ${PROJECT_BINARY_DIR}/${TARGET}.bin)

add_custom_target(${TARGET}.hex ALL DEPENDS ${TARGET}.elf COMMAND ${CMAKE_OBJCOPY} -Oihex ${TARGET}.elf ${TARGET}.hex)
add_custom_target(${TARGET}.bin ALL DEPENDS ${TARGET}.elf COMMAND ${CMAKE_OBJCOPY} -Obinary -S ${TARGET}.elf ${TARGET}.bin)
