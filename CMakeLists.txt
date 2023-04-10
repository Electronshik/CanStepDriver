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

project(CanStepDriverPowerstep)
set(PROJECT_BINARY_DIR build)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

set(MCPU cortex-m4)

add_compile_definitions(ARM_MATH_CM4;ARM_MATH_MATRIX_CHECK;ARM_MATH_ROUNDING)
add_compile_options(-mfloat-abi=hard -mfpu=fpv4-sp-d16)
add_link_options(-mfloat-abi=hard -mfpu=fpv4-sp-d16)

add_compile_options(-mcpu=${MCPU} -mthumb -mthumb-interwork)
add_compile_options(-ffunction-sections -fdata-sections -fno-common -fmessage-length=0)

include_directories(
	${CMAKE_SOURCE_DIR}/CoreApp
	${CMAKE_SOURCE_DIR}/LowLevel
	)

# add_definitions(#[[TODO insert definitions, ex. -DUSE_HAL_DRIVER -DCORE_CM4 -DDEBUG -DSTM32H745xx)

file(GLOB_RECURSE SOURCES
        "CoreApp/*.*"
        # "Drivers/*.*" "../Drivers/*.*"
        # "../Common/*.*"
        #[[TODO rest of cource locations]]
        )

set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/LowLevel/CanStepDriverPowerstep/STM32G431CBTx_FLASH.ld)
# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Map=${PROJECT_SOURCE_DIR}/build/${PROJECT_NAME}.map")

add_link_options(-Wl,-gc-sections,--print-memory-usage,-Map=${PROJECT_SOURCE_DIR}/${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map)
add_link_options(-mcpu=${MCPU} -mthumb -mthumb-interwork)
add_link_options(-specs=nosys.specs -T ${LINKER_SCRIPT})
# add_link_options(-L${PROJECT_SOURCE_DIR}/LowLevel/CanStepDriverPowerstep -lc -lm -lnosys -lLowLevel)
add_link_options(-lc -lm -lnosys)

add_executable(${PROJECT_NAME}.elf ${SOURCES} ${LINKER_SCRIPT})
# set_target_properties(${PROJECT_BINARY_DIR}/${PROJECT_NAME}.elf PROPERTIES LINKER_LANGUAGE C)

target_link_libraries(${PROJECT_NAME}.elf ${PROJECT_SOURCE_DIR}/LowLevel/${PROJECT_NAME}/libLowLevel.a)

set(HEX_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin)

# add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
#         COMMAND ${CMAKE_OBJCOPY} -O ihex ${PROJECT_NAME}.elf ${HEX_FILE}
#         # COMMAND ${CMAKE_OBJCOPY} -O binary -S $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
#         COMMENT "Building ${HEX_FILE}
# Building ${BIN_FILE}")

add_custom_target(${PROJECT_NAME}.hex ALL DEPENDS ${PROJECT_NAME}.elf COMMAND ${CMAKE_OBJCOPY} -Oihex ${PROJECT_NAME}.elf ${PROJECT_NAME}.hex)
add_custom_target(${PROJECT_NAME}.bin ALL DEPENDS ${PROJECT_NAME}.elf COMMAND ${CMAKE_OBJCOPY} -Obinary -S ${PROJECT_NAME}.elf ${PROJECT_NAME}.bin)
