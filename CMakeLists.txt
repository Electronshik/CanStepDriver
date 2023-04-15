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

file(GLOB_RECURSE SOURCES
	"CoreApp/*.*"
)

include_directories(
	${CMAKE_SOURCE_DIR}/CoreApp
	${CMAKE_SOURCE_DIR}/LowLevel
)


# ALL TARGETS COMPILER AND LINKER OPTIONS
set(MCPU cortex-m4)
add_compile_definitions(ARM_MATH_CM4; ARM_MATH_MATRIX_CHECK; ARM_MATH_ROUNDING)
add_compile_options(-mfloat-abi=hard -mfpu=fpv4-sp-d16)
add_compile_options(-mcpu=${MCPU} -mthumb -mthumb-interwork)
add_compile_options(-ffunction-sections -fdata-sections -fno-common -fmessage-length=0)

add_link_options(-mfloat-abi=hard -mfpu=fpv4-sp-d16)
add_link_options(-Wl,-gc-sections,--print-memory-usage)
add_link_options(-mcpu=${MCPU} -mthumb -mthumb-interwork -specs=nosys.specs -lc -lm -lnosys)
# ALL TARGETS COMPILER AND LINKER OPTIONS

set(TARGET_POWERSTEP CanStepDriverPowerstep)
set(TARGET_MOSFETS CanStepDriverMosfets)
set(TARGET_F429 CanStepDriverF429)


# TARGET_POWERSTEP
add_executable(${TARGET_POWERSTEP}.elf ${SOURCES})
target_link_libraries(${TARGET_POWERSTEP}.elf ${PROJECT_SOURCE_DIR}/LowLevel/${TARGET_POWERSTEP}/lib${TARGET_POWERSTEP}.a)
target_compile_definitions(${TARGET_POWERSTEP}.elf PUBLIC
	TARGET_POWERSTEP
)
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
target_compile_definitions(${TARGET_F429}.elf PUBLIC
	TARGET_F429
)
target_link_options(${TARGET_F429}.elf PUBLIC
	-Wl,-Map=${PROJECT_SOURCE_DIR}/${PROJECT_BINARY_DIR}/${TARGET_F429}.map
	-T${PROJECT_SOURCE_DIR}/LowLevel/${TARGET_F429}/STM32F429ZITx_FLASH.ld
)
add_custom_target(${TARGET_F429}.hex ALL DEPENDS ${TARGET_F429}.elf COMMAND ${CMAKE_OBJCOPY} -Oihex ${TARGET_F429}.elf ${TARGET_F429}.hex)
add_custom_target(${TARGET_F429}.bin ALL DEPENDS ${TARGET_F429}.elf COMMAND ${CMAKE_OBJCOPY} -Obinary -S ${TARGET_F429}.elf ${TARGET_F429}.bin)
# TARGET_F429

# TARGET_MOSFETS
add_executable(${TARGET_MOSFETS}.elf ${SOURCES})
target_link_libraries(${TARGET_MOSFETS}.elf ${PROJECT_SOURCE_DIR}/LowLevel/${TARGET_MOSFETS}/lib${TARGET_MOSFETS}.a)
target_compile_definitions(${TARGET_MOSFETS}.elf PUBLIC
	TARGET_MOSFETS
)
target_link_options(${TARGET_MOSFETS}.elf PUBLIC
	-Wl,-Map=${PROJECT_SOURCE_DIR}/${PROJECT_BINARY_DIR}/${TARGET_MOSFETS}.map
	-T${PROJECT_SOURCE_DIR}/LowLevel/${TARGET_MOSFETS}/STM32G431CBTx_FLASH.ld
)
add_custom_target(${TARGET_MOSFETS}.hex ALL DEPENDS ${TARGET_MOSFETS}.elf COMMAND ${CMAKE_OBJCOPY} -Oihex ${TARGET_MOSFETS}.elf ${TARGET_MOSFETS}.hex)
add_custom_target(${TARGET_MOSFETS}.bin ALL DEPENDS ${TARGET_MOSFETS}.elf COMMAND ${CMAKE_OBJCOPY} -Obinary -S ${TARGET_MOSFETS}.elf ${TARGET_MOSFETS}.bin)
# TARGET_MOSFETS
