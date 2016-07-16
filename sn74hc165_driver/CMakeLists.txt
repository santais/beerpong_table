project(sn74hc165_driver)
cmake_minimum_required(VERSION 2.8)

# Add the headers

FILE(GLOB_RECURSE SOURCES "src/*.c")
FILE(GLOB_RECURSE HEADERS "include/*.h")

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Set Flags
SET(CMAKE_CXX_FLAGS "-pthread" "-lwiringPi")

# Add math library
target_link_libraries(${PROJECT_NAME} m)
# Add option to choose among RPI and X86
OPTION(ARM "Option description" OFF) # Enabled by default

IF(ARM)
    ADD_DEFINITIONS(-DARM)
    target_link_libraries(${PROJECT_NAME} wiringPi)
ENDIF(ARM)
