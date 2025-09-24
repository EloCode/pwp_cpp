cmake_minimum_required(VERSION 3.20)

# set the project name
project(PWP VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GMP REQUIRED IMPORTED_TARGET gmp)
pkg_check_modules(GMPXX REQUIRED IMPORTED_TARGET gmpxx)
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

add_executable(pwp ${SRC})
target_link_libraries(pwp PUBLIC 
	PkgConfig::GMP
	PkgConfig::GMPXX
)
if(OpenMP_CXX_FOUND)
    target_link_libraries(pwp PUBLIC OpenMP::OpenMP_CXX)
endif()
target_compile_options(pwp PUBLIC $<$<CONFIG:Debug>:-Wall -Wextra -Wpedantic>)


enable_testing()
add_executable(pwp_tests ${SRC})
target_compile_definitions(pwp_tests PUBLIC -DTEST)
target_link_libraries(pwp_tests PUBLIC 
	PkgConfig::GMP 
	PkgConfig::GMPXX
)
add_test(NAME pwp_tests COMMAND pwp_tests)


if(DOXYGEN_FOUND)
    doxygen_add_docs(
        doxygen
        ${PROJECT_SOURCE_DIR}
        COMMENT "Generate documentation"
    )
endif()
