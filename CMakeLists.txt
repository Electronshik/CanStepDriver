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
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 11)

file(GLOB_RECURSE SOURCES
	"CoreApp/*.*"
	"CoreBoard/*.*"
	"CoreOS/*.*"
)

include_directories(
	${CMAKE_SOURCE_DIR}/CoreApp
	${CMAKE_SOURCE_DIR}/CoreBoard
	${CMAKE_SOURCE_DIR}/CoreOS
	${CMAKE_SOURCE_DIR}/LowLevel
)

set(CMAKE_BUILD_TYPE DEBUG)

# ALL TARGETS COMPILER AND LINKER OPTIONS
set(MCPU cortex-m4)
add_compile_options(-mfloat-abi=hard -mfpu=fpv4-sp-d16)
add_compile_options(-mcpu=${MCPU} -mthumb -fno-common)
add_compile_options(-ffunction-sections -fdata-sections -fmessage-length=0)
add_compile_options(-g -O0 -Wwrite-strings -funwind-tables) # -fno-exceptions -fno-rtti)
#  -funwind-tables -std=c++20
add_link_options(-mfloat-abi=hard -mfpu=fpv4-sp-d16)
add_link_options(-Wl,--gc-sections,--print-memory-usage -Wwrite-strings)
add_link_options(-mcpu=${MCPU} -mthumb -funwind-tables -lm -specs=nosys.specs)
# -specs=nano.specs -lc -specs=nosys.specs -lnosys -lstdc++ -lsupc++
# ALL TARGETS COMPILER AND LINKER OPTIONS

# FREERTOS SETTINGS
include(FetchContent)
FetchContent_Declare( freertos_kernel
  GIT_REPOSITORY https://github.com/FreeRTOS/FreeRTOS-Kernel.git
  GIT_TAG        main #Note: Best practice to use specific git-hash or tagged version
)

add_library(freertos_config INTERFACE)
include_directories(freertos_config SYSTEM
	INTERFACE
	${CMAKE_SOURCE_DIR}/CoreOS
)

add_compile_definitions(freertos_config
	INTERFACE
	projCOVERAGE_TEST=0
)

set(FREERTOS_HEAP "4" CACHE STRING "FreeRTOS Heap Model" FORCE)
set(FREERTOS_PORT "GCC_ARM_CM4F" CACHE STRING "FreeRTOS Cross Compile Port" FORCE)

FetchContent_MakeAvailable(freertos_kernel)
# FREERTOS SETTINGS

# ETL SETTINGS
FetchContent_Declare( etl
  GIT_REPOSITORY https://github.com/ETLCPP/etl.git
  GIT_TAG        master
)

FetchContent_MakeAvailable(etl)
# ETL SETTINGS

set(TARGET_POWERSTEP CanStepDriverPowerstep)
set(TARGET_MOSFETS CanStepDriverMosfets)
set(TARGET_F429 CanStepDriverF429)

function(add_common_targets CURRENT_TARGET)
	add_custom_target(${CURRENT_TARGET}.hex ALL DEPENDS ${CURRENT_TARGET}.elf COMMAND ${CMAKE_OBJCOPY} -Oihex ${CURRENT_TARGET}.elf ${CURRENT_TARGET}.hex)
	add_custom_target(${CURRENT_TARGET}.bin ALL DEPENDS ${CURRENT_TARGET}.elf COMMAND ${CMAKE_OBJCOPY} -Obinary -S ${CURRENT_TARGET}.elf ${CURRENT_TARGET}.bin)
endfunction()

# TARGET_POWERSTEP
add_executable(${TARGET_POWERSTEP}.elf ${SOURCES})
target_link_libraries(${TARGET_POWERSTEP}.elf ${PROJECT_SOURCE_DIR}/LowLevel/${TARGET_POWERSTEP}/lib${TARGET_POWERSTEP}.a)
target_link_options(${TARGET_POWERSTEP}.elf PUBLIC
	-Wl,-Map=${PROJECT_SOURCE_DIR}/${PROJECT_BINARY_DIR}/${TARGET_POWERSTEP}.map
	-T${PROJECT_SOURCE_DIR}/LowLevel/${TARGET_POWERSTEP}/STM32G431CBTx_FLASH.ld
)
add_custom_target(${TARGET_POWERSTEP}.hex ALL DEPENDS ${TARGET_POWERSTEP}.elf COMMAND ${CMAKE_OBJCOPY} -Oihex ${TARGET_POWERSTEP}.elf ${TARGET_POWERSTEP}.hex)
add_custom_target(${TARGET_POWERSTEP}.bin ALL DEPENDS ${TARGET_POWERSTEP}.elf COMMAND ${CMAKE_OBJCOPY} -Obinary -S ${TARGET_POWERSTEP}.elf ${TARGET_POWERSTEP}.bin)
# TARGET_POWERSTEP

# TARGET_F429
add_executable(${TARGET_F429}.elf ${SOURCES})
target_link_libraries(${TARGET_F429}.elf ${PROJECT_SOURCE_DIR}/LowLevel/${TARGET_F429}/lib${TARGET_F429}.a)
target_link_libraries(${TARGET_F429}.elf freertos_kernel)
target_link_libraries(${TARGET_F429}.elf etl::etl)
target_link_options(${TARGET_F429}.elf PUBLIC
	-Wl,-Map=${PROJECT_SOURCE_DIR}/${PROJECT_BINARY_DIR}/${TARGET_F429}.map
	-T${PROJECT_SOURCE_DIR}/${PROJECT_BINARY_DIR}/${TARGET_F429}.ld
)
set(FLASH_START 0x8000000)
configure_file(${PROJECT_SOURCE_DIR}/LowLevel/${TARGET_F429}/STM32F429ZITx_FLASH.ld ${PROJECT_SOURCE_DIR}/${PROJECT_BINARY_DIR}/${TARGET_F429}.ld)
# add_custom_target(${TARGET_F429}.hex ALL DEPENDS ${TARGET_F429}.elf COMMAND ${CMAKE_OBJCOPY} -Oihex ${TARGET_F429}.elf ${TARGET_F429}.hex)
# add_custom_target(${TARGET_F429}.bin ALL DEPENDS ${TARGET_F429}.elf COMMAND ${CMAKE_OBJCOPY} -Obinary -S ${TARGET_F429}.elf ${TARGET_F429}.bin)
# configure_file(${PROJECT_SOURCE_DIR}/${PROJECT_BINARY_DIR}/${TARGET_F429}.elf ${PROJECT_SOURCE_DIR}/${PROJECT_BINARY_DIR}/${PROJECT_NAME}.elf COPYONLY)
add_common_targets(${TARGET_F429})
# TARGET_F429

# TARGET_MOSFETS
add_executable(${TARGET_MOSFETS}.elf ${SOURCES})
target_link_libraries(${TARGET_MOSFETS}.elf ${PROJECT_SOURCE_DIR}/LowLevel/${TARGET_MOSFETS}/lib${TARGET_MOSFETS}.a)
target_link_options(${TARGET_MOSFETS}.elf PUBLIC
	-Wl,-Map=${PROJECT_SOURCE_DIR}/${PROJECT_BINARY_DIR}/${TARGET_MOSFETS}.map
	-T${PROJECT_SOURCE_DIR}/LowLevel/${TARGET_MOSFETS}/STM32G431CBTx_FLASH.ld
)
add_custom_target(${TARGET_MOSFETS}.hex ALL DEPENDS ${TARGET_MOSFETS}.elf COMMAND ${CMAKE_OBJCOPY} -Oihex ${TARGET_MOSFETS}.elf ${TARGET_MOSFETS}.hex)
add_custom_target(${TARGET_MOSFETS}.bin ALL DEPENDS ${TARGET_MOSFETS}.elf COMMAND ${CMAKE_OBJCOPY} -Obinary -S ${TARGET_MOSFETS}.elf ${TARGET_MOSFETS}.bin)
# TARGET_MOSFETS
