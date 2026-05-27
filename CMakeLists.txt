cmake_minimum_required(VERSION 3.15)
project(dppp VERSION 1.0.0 LANGUAGES CXX)

add_library(dppp INTERFACE)
add_library(dppp::dppp ALIAS dppp)

target_include_directories(dppp INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_compile_features(dppp INTERFACE cxx_std_17)

include(CMakePackageConfigHelpers)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/dpppConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/dpppConfig.cmake
    INSTALL_DESTINATION lib/cmake/dppp
)

install(TARGETS dppp EXPORT dpppTargets)
install(EXPORT dpppTargets
    NAMESPACE dppp::
    DESTINATION lib/cmake/dppp
)

install(DIRECTORY include/dppp DESTINATION include)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/dpppConfig.cmake
    DESTINATION lib/cmake/dppp
)
