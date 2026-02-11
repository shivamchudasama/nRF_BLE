cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(EmptyApp)

# Add source files for application
target_sources(app PRIVATE
   src/main.c
   src/FOTA_GATT.c
   src/FW_Update.c)

# Add include directories for application
target_include_directories(app PRIVATE
   src)
