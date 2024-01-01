cmake_minimum_required(VERSION 3.10)
project(pass-fuzzer)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the executable
add_executable(ClusterAccess
    ClusterAccess.cpp
    ../driver/JsonFileHandler.cpp
    ../driver/Patch.cpp
    # k8sApiWrapper.c
)

# Set language for source files explicitly
# set_source_files_properties(k8sApiWrapper.c PROPERTIES LANGUAGE C)

# Specify include directories
target_include_directories(ClusterAccess 
    PRIVATE 
    "/home/kabilan/Downloads/c/kubernetes" 
    "/home/kabilan/Desktop/FYP/implementation/implementation-2/paas-fuzzer/include/driver"
    "/home/kabilan/Desktop/FYP/implementation/implementation-2/paas-fuzzer/include/scheduler"
    "/home/kabilan/Desktop/FYP/implementation/implementation-2/paas-fuzzer/vcpkg/installed/x64-linux/include"
)


# Specify library directories
link_directories(
    "/home/kabilan/Downloads/c/kubernetes/build/**"
    "/home/kabilan/Desktop/FYP/implementation/implementation-2/paas-fuzzer/vcpkg/installed/x64-linux/lib"
)


# Link
target_link_libraries(ClusterAccess 
    PRIVATE
    yaml
    websockets
    kubernetes
    /home/kabilan/Desktop/FYP/implementation/implementation-2/paas-fuzzer/vcpkg/installed/x64-linux/lib/libjsoncpp.a
)