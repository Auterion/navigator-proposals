cmake_minimum_required(VERSION 3.5)

project(Test LANGUAGES CXX)

add_definitions(-DGTEST_LANGUAGE_CXX11)

find_package(Threads REQUIRED)

if (NOT EXISTS ${GOOGLETEST_DIR})
    message( SEND_ERROR "You need to pass in GOOGLETEST_DIR" )
endif ()
if (EXISTS ${GOOGLETEST_DIR})
    set(GTestSrc ${GOOGLETEST_DIR}/googletest)
    set(GMockSrc ${GOOGLETEST_DIR}/googlemock)
elseif (UNIX AND EXISTS /usr/src/gtest)
    set(GTestSrc /usr/src/gtest)
    message(WARNING "Using gtest from system")
    if (EXISTS /usr/src/gmock)
        set(GMockSrc /usr/src/gmock)
    endif ()
else ()
    message( FATAL_ERROR "No googletest src dir found - set GOOGLETEST_DIR to enable!")
endif ()

set(AppDir ${CMAKE_CURRENT_SOURCE_DIR}/..)

include_directories(${AppDir})


set(GTestFiles ${GTestSrc}/src/gtest-all.cc)
set(GTestIncludes ${GTestSrc} ${GTestSrc}/include)
if (NOT ${GMockSrc} STREQUAL "")
    list(APPEND GTestFiles ${GMockSrc}/src/gmock-all.cc)
    list(APPEND GTestIncludes ${GMockSrc} ${GMockSrc}/include)
endif ()

include_directories(${GTestIncludes})

add_executable(Test main.cpp
               ${GTestFiles}
               TestMcPosControlFlightMode.cpp
               )
add_test(NAME Test COMMAND Test)

target_link_libraries(Test PRIVATE Threads::Threads)

