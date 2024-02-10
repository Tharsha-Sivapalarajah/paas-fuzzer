# PaaS Fuzzer setup guide.
The PaaS Fuzzer is a C++-based system utilized for fuzzing Kubernetes to uncover vulnerabilities within the Kubernetes platform itself.

## Prerequesties 
1. Kubernetes C client.
2. Vcpkg 
3. CMake

## Project File Structure (recomended)
Below is a recommended directory structure for the Pass-Fuzzer application, including necessary external libraries and clients:
```
PASS-FUZZER 
│
└───c
│
└───libwebsockets
│
└───pass-fuzzer
    |   data/
    |   include/
    |   setup-guige/
    |   src/
    │       driver/
    │       ...
    │   CMakeLists.txt
    │   toolchain.cmake
```

## Step-by-step guide
1. Setup the Kubernetes c client inside the main directory(PASS-FUZZER)
   1. Run the Script `./setup-k8s-c-client.sh`.
2.  