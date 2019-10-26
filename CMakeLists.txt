cmake_minimum_required(VERSION 2.8)

project(ReadDICOMSeries)

find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

add_executable(ReadDICMOSeries ReadDICMOSeries.cxx)
target_link_libraries(ReadDICMOSeries ${ITK_LIBRARIES})