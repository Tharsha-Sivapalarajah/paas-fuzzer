cmake_minimum_required(VERSION 3.10)
project(pass-fuzzer)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the executable
add_executable(ConfigGenerator src/driver/ConfigGenerator.cpp)

# Specify include directories
target_include_directories(ConfigGenerator PRIVATE "/home/kabilan/Desktop/FYP/implementation/implementation-2/paas-fuzzer/vcpkg/installed/x64-linux/include")

# Specify library directories
link_directories("/home/kabilan/Desktop/FYP/implementation/implementation-2/paas-fuzzer/vcpkg/installed/x64-linux/lib")

# Link against the jsoncpp library
target_link_libraries(ConfigGenerator PRIVATE /home/kabilan/Desktop/FYP/implementation/implementation-2/paas-fuzzer/vcpkg/installed/x64-linux/lib/libjsoncpp.a)

