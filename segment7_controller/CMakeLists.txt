project(segment7_controller)
cmake_minimum_required(VERSION 2.8)

# Add the headers

FILE(GLOB_RECURSE SOURCES "src/*.c" "../sn74hc595_driver2/src/sn74hc595.c"
	"../sn74hc165_driver/src/sn74hc165.c")
FILE(GLOB_RECURSE HEADERS "include/*.h")

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/../sn74hc165_driver/include)
include_directories(${CMAKE_SOURCE_DIR}/../sn74hc595_driver2/include)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Pthread
find_package (Threads)

# Set Flags
SET(CMAKE_CXX_FLAGS "-pthread" "-lwiringPi")

# Add math library
target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT} m)
# Add option to choose among RPI and X86
OPTION(ARM "Option description" OFF) # Enabled by default

IF(ARM)
    ADD_DEFINITIONS(-DARM)
    target_link_libraries(${PROJECT_NAME} wiringPi)
ENDIF(ARM)
