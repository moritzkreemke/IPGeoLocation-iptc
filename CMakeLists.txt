# cmake_minimum_required(VERSION <specify CMake version here>)
project(AgabeC C)

set(CMAKE_C_STANDARD 99)


add_executable(AgabeC main.c)
FILE(COPY resources/asn-country-ipv4.csv DESTINATION resources)

enable_testing()
add_subdirectory(tests)
