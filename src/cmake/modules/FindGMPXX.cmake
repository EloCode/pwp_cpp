# CMake’s `find_package` expects either:
# • A Config file (<Package>Config.cmake) installed by the library
# • A Find module (Find<Package>.cmake) shipped with CMake or written by you.
# However GMP in vcpkg does not provide either of these. 
# Instead, it installs gmp.pc and gmpxx.pc files, which are pkg‑config metadata files.
# Thus we write our own FindGMP.cmake file to keep the CMakeLists.txt uniform with only find_package
# Moreover, this allows us to drop this workaround if gmp ever starts shipping cmake files

find_package(PkgConfig REQUIRED)

pkg_check_modules(GMPXX REQUIRED IMPORTED_TARGET gmpxx)

add_library(GMP::CXX INTERFACE IMPORTED)
set_target_properties(GMP::CXX PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${GMPXX_INCLUDE_DIRS}"
    INTERFACE_LINK_LIBRARIES "PkgConfig::GMPXX"
)