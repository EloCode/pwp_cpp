cmake_minimum_required(VERSION 3.20)

# set the project name
project(PWP VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)
find_package(GMP REQUIRED)
find_package(GMPXX REQUIRED)
find_package(OpenMP)
find_package(Doxygen OPTIONAL_COMPONENTS dot mscgen dia)

configure_file(config.hpp.in config.hpp)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(SRC
    main.cpp
    config.hpp
    PowersWithoutPowersFinder_Test.hpp
    PowersWithoutPowersWorklist_Test.hpp
    SuffixClass.cpp
    SuffixMath.cpp
    SuffixSet.cpp
    PowersWithoutPowersFinder.cpp
    PowersWithoutPowersWorklist.cpp
    Stats.cpp
    SuffixClass.hpp
    SuffixMath.hpp
    SuffixSet.hpp
    ExtraOperators.hpp
    PowersWithoutPowersFinder.hpp
    PowersWithoutPowersWorklist.hpp
    Stats.hpp SuffixClass_Test.hpp
    SuffixMath_Test.hpp
    SuffixSet_Test.hpp
)

# Define common compile options based on compiler
if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(COMMON_COMPILE_OPTIONS /Zc:__cplusplus $<$<CONFIG:Debug>: /W4>)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
    set(COMMON_COMPILE_OPTIONS $<$<CONFIG:Debug>: -Wall -Wextra -Wpedantic>)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "IntelLLVM")
    set(COMMON_COMPILE_OPTIONS $<$<CONFIG:Debug>: -Wall -Wextra>)
endif()

# Define common link libraries
set(COMMON_LINK_LIBRARIES GMP::C GMP::CXX)
if(OpenMP_CXX_FOUND)
    list(APPEND COMMON_LINK_LIBRARIES OpenMP::OpenMP_CXX)
endif()

# Main executable
add_executable(pwp ${SRC})
target_compile_options(pwp PUBLIC ${COMMON_COMPILE_OPTIONS})
target_link_libraries(pwp PUBLIC ${COMMON_LINK_LIBRARIES})

# Test executable
enable_testing()
add_executable(pwp_tests ${SRC})
target_compile_definitions(pwp_tests PUBLIC -DTEST)
target_compile_options(pwp_tests PUBLIC ${COMMON_COMPILE_OPTIONS})
target_link_libraries(pwp_tests PUBLIC ${COMMON_LINK_LIBRARIES})
add_test(NAME pwp_tests COMMAND pwp_tests)

# Documentation
if(DOXYGEN_FOUND)
    doxygen_add_docs(
        doxygen
        ${PROJECT_SOURCE_DIR}
        COMMENT "Generate documentation"
    )
endif()