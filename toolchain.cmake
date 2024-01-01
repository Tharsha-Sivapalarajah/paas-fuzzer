# Set cross-compiler paths for x86_64
SET(CMAKE_C_COMPILER /usr/bin/x86_64-linux-gnu-gcc)
SET(CMAKE_CXX_COMPILER /usr/bin/x86_64-linux-gnu-g++)

# Set compiler flags
SET(CMAKE_C_FLAGS "-march=x86-64 -mtune=generic -O2 -pipe")
SET(CMAKE_CXX_FLAGS "-march=x86-64 -mtune=generic -O2 -pipe")

# Set sysroot path if necessary
# SET(CMAKE_SYSROOT /path/to/your/target/sysroot)
# SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
# SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
