cmake_minimum_required(VERSION 3.16)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
	cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

set(CMAKE_CXX_STANDARD 17)
project(biosim)

file(GLOB_RECURSE source_files "src/*.cpp")
file(GLOB_RECURSE header_files "src/*.h")

add_executable(biosim
    ${source_files}
    ${header_files}
)

target_link_libraries(biosim PRIVATE sfml-graphics sfml-window sfml-system)
