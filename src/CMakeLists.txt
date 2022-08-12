cmake_minimum_required(VERSION 3.18)

# set the project name
project(PWP VERSION 1.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_library(libgmp gmp REQUIRED)
find_library(libgmpxx gmpxx REQUIRED)
find_package(OpenMP)


include_directories(${PROJECT_SOURCE_DIR})
add_executable(pwp main.cpp config.hpp PowersWithoutPowersFinder_Test.hpp PowersWithoutPowersWorklist_Test.hpp SuffixClass.cpp SuffixMath.cpp SuffixSet.cpp
defines.hpp PowersWithoutPowersFinder.cpp PowersWithoutPowersWorklist.cpp Stats.cpp SuffixClass.hpp SuffixMath.hpp SuffixSet.hpp
ExtraOperators.hpp PowersWithoutPowersFinder.hpp PowersWithoutPowersWorklist.hpp Stats.hpp SuffixClass_Test.hpp SuffixMath_Test.hpp SuffixSet_Test.hpp
)
configure_file(config.hpp.in config.hpp)
target_include_directories(pwp PUBLIC "${PROJECT_BINARY_DIR}")
target_link_libraries(pwp PUBLIC ${libgmp} ${libgmpxx})
if(OpenMP_CXX_FOUND)
    target_link_libraries(pwp PUBLIC OpenMP::OpenMP_CXX)
endif()

