cmake_minimum_required(VERSION 3.10)
project(Stm32NetXHttpWebServer)
file(GLOB_RECURSE SOURCES
        "src/*.*"
)
add_library(Stm32NetXHttpWebServer STATIC ${SOURCES})
target_include_directories(Stm32NetXHttpWebServer PUBLIC
        src
)
