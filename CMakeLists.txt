# The following lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.16)

set(EXTRA_COMPONENT_DIRS ./components/led_control
						 ./components/pulse_counter
						 ./components/radar_trigger
						 ./components/uart)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(sar_sync_fw)
