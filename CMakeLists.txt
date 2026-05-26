#############################################################################################################################
# file:  CMakeLists.txt
# brief: Template "CMakeLists.txt" for building of executables and static libraries.
#
# usage: Edit "VARIABLES"-section to suit project requirements.
#        For debug build:
#          cmake -DCMAKE_TOOLCHAIN_FILE=cubeide-gcc.cmake  -S ./ -B Debug -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug
#          make -C Debug VERBOSE=1 -j
#        For release build:
#          cmake -DCMAKE_TOOLCHAIN_FILE=cubeide-gcc.cmake  -S ./ -B Release -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Release
#          make -C Release VERBOSE=1 -j
#############################################################################################################################
cmake_minimum_required(VERSION 3.20)

###################### CONSTANTS ######################################
set (PROJECT_TYPE_EXECUTABLE          "exe")
set (PROJECT_TYPE_STATIC_LIBRARY      "static-lib")
set (MCPU_CORTEX_M0				      "-mcpu=cortex-m0")
set (MCPU_CORTEX_M0PLUS				  "-mcpu=cortex-m0plus")
set (MCPU_CORTEX_M3				      "-mcpu=cortex-m3")
set (MCPU_CORTEX_M4				      "-mcpu=cortex-m4")
set (MCPU_CORTEX_M7				      "-mcpu=cortex-m7")
set (MCPU_CORTEX_M33				  "-mcpu=cortex-m33")
set (MCPU_CORTEX_M55				  "-mcpu=cortex-m55")
set (MCPU_CORTEX_M85				  "-mcpu=cortex-m85")
set (MFPU_FPV4_SP_D16                 "-mfpu=fpv4-sp-d16")
set (MFPU_FPV4                        "-mfpu=vfpv4")
set (MFPU_FPV5_D16                    "-mfpu=fpv5-d16")
set (RUNTIME_LIBRARY_REDUCED_C        "--specs=nano.specs")
set (RUNTIME_LIBRARY_STD_C            "")
set (RUNTIME_LIBRARY_SYSCALLS_MINIMAL "--specs=nosys.specs")
set (RUNTIME_LIBRARY_SYSCALLS_NONE    "")
set (MFLOAT_ABI_SOFTWARE              "-mfloat-abi=soft")
set (MFLOAT_ABI_HARDWARE              "-mfloat-abi=hard")
set (MFLOAT_ABI_MIX                   "-mfloat-abi=softfp")
#######################################################################





###################### VARIABLES ######################################
set (PROJECT_NAME             "RentLow")
set (PROJECT_TYPE             "exe")
set (LINKER_SCRIPT            "../STM32F411CEUX_FLASH.ld")
set (MCPU                     "-mcpu=Cortex-M4")
set (MFPU                     "-mfpu=fpv4-sp-d16")
set (MFLOAT_ABI               "")
set (RUNTIME_LIBRARY          "--specs=nano.specs")
set (RUNTIME_LIBRARY_SYSCALLS "--specs=nosys.specs")

set (PROJECT_SOURCES
	# LIST SOURCE FILES HERE
	Startup/startup_stm32f411ceux.s
	Sources/main.cpp                 # 1. CHANGED: Renamed from .c to .cpp
	Include/syscalls.c
	Sources/usb_setup.c
	Include/sysmem.c
	
	
	Libraries/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c
	Libraries/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c
	Libraries/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c
	Libraries/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c
	Libraries/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.c
	Libraries/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c
	
	# TinyUSB Core Stack Files
    Libraries/TinyUSB/tusb.c
    Libraries/TinyUSB/device/usbd.c
    Libraries/TinyUSB/common/tusb_fifo.c
    
    # TinyUSB CDC (Virtual COM Port) Class Driver
    Libraries/TinyUSB/class/cdc/cdc_device.c
    Libraries/TinyUSB/portable/synopsys/dwc2/dcd_dwc2.c
    Libraries/TinyUSB/portable/synopsys/dwc2/dwc2_common.c
       	)

set (PROJECT_DEFINES
	# LIST COMPILER DEFINITIONS HERE
	"USE_HAL_DRIVER"
    "STM32F411xE"
    )

set (PROJECT_INCLUDES
	# LIST INCLUDE DIRECTORIES HERE
	"Include"
    "Libraries/CMSIS/Device/ST/STM32F4xx/Include"
    "Libraries/CMSIS/Include"
    "Libraries/STM32F4xx_HAL_Driver/Inc"
    "Libraries/CMSIS/Device/ST/STM32F4xx/Include/ST"
    "Libraries/TinyUSB"
    "Libraries/TinyUSB/device"
    "Libraries/TinyUSB/common"
    )







############ MODIFY ACCORDING TO REQUIREMENTS) ########################

#######################################################################

################## PROJECT SETUP ######################################
project(${PROJECT_NAME})
enable_language(ASM)
enable_language(CXX) # 2. ADDED: Tells CMake to enable the C++ compiler

if (${PROJECT_TYPE} MATCHES ${PROJECT_TYPE_EXECUTABLE})
  add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
  add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${CMAKE_PROJECT_NAME}>)
elseif (${PROJECT_TYPE} MATCHES ${PROJECT_TYPE_STATIC_LIBRARY})
  add_library(${PROJECT_NAME} ${PROJECT_SOURCES})
endif()

add_compile_definitions (${PROJECT_DEFINES})
include_directories (${PROJECT_INCLUDES})

set (CMAKE_EXECUTABLE_SUFFIX ".elf")
set (CMAKE_STATIC_LIBRARY_SUFFIX ".a")

# 3. ADDED CXX FLAGS: Configures flags specifically for your C++ compiler (C++17 standard used here)
set (CMAKE_C_FLAGS "${MCPU} -std=gnu11 ${MFPU} ${MFLOAT_ABI} ${RUNTIME_LIBRARY} -mthumb -Wall -Werror")
set (CMAKE_CXX_FLAGS "${MCPU} -std=gnu++17 ${MFPU} ${MFLOAT_ABI} ${RUNTIME_LIBRARY} -mthumb -Wall -Werror -fno-exceptions -fno-rtti")
set (CMAKE_EXE_LINKER_FLAGS "-T${LINKER_SCRIPT} ${RUNTIME_LIBRARY_SYSCALLS} -Wl,-Map=test.map -Wl,--gc-sections -static -Wl,--start-group -lc -lm -lstdc++ -lsupc++ -Wl,--end-group")
set (CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS} -x assembler-with-cpp")