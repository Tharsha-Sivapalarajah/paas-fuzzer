# PaaSFuzzer

---


## Table of Contents

- [Introduction](#introduction)
- [Prerequisite](#prerequisite)
- [Getting Stated](#usage)
- [Architecture](#high-level-architecture)
- [Contributors](#contributors)
- [Contact](#contact)


## Introduction

PaaSFuzzer is a C++ based fuzzing tool designed to detect Reconciliation Bugs within cloud-based Platform as a Service (PaaS) systems, specifically tailored to scrutinize the scheduler module of Container Orchestration Systems. This tool has been implemented on Kubernetes as the Software Under Test (SUT), leveraging Kubernetes' widespread adoption as the leading container orchestration tool and foundational technology for PaaS application development.



## Pre-requisite

- Kubernetes C client - This library is used to provide a set of APIs and tools for interacting with Kubernetes clusters using the C programming language
- Vcpkg - Vcpkg is used to simplify the process of acquiring and building open-source C and C++ libraries on Windows, Linux, and macOS.
- CMake - CMake is used to help manage the build process of software projects by generating build scripts and configuration files for various build environments
- MySQL Connector/C++ - This library is used to provide a C++ API for connecting and interacting with MySQL databases


## Project File Structure (recommended)
Below is the recommended Directory Structure for the PaaSFuzzer application, including necessary external libraries and clients:

```
📦 PaaSFuzzer 
 ┣ 📂 c
 ┣ 📂 libwebsockets
 ┣ 📂 pass-fuzzer
   ┗ 📂 data
   ┗ 📂 include
   ┗ 📂 setup-guide
   ┗ 📂 src
   ┃  ┗ 📂 driver
   ┃  ┗ 📂 QueryGeneration
   ┃    .....
   ┗ 📂 static
   ┗ 📂 vcpkg
   ┗ 📜 CMakeLists.txt
   ┗ 📜 toolchain.cmake
```


## Getting Started

To start detecting Reconciliation Bugs within cloud-based Platform as a Service (PaaS) systems using PaaSFuzzer, follow these steps:

### 1. Cloning PaaSFuzzer
To clone the PaaSFuzzer repository, follow these steps:

1. Open a Terminal Window.
2. Change to the directory where you want to clone the repository.
3. Run the following command:

```
git clone https://github.com/KabilanMA/paas-fuzzer.git
```

This will create a local copy of the PaaSFuzzer repository in the current directory.


### 2. Setup Kubernetes C Client
To setup Kubernetes C Client Library inside the main directory, follow these steps:

1. Open a Terminal Window.
2. Change to the directory where you have cloned the repository.
3. Run the follwing command:

```
./setup-k8s-c-client.sh
```
  
***Note:*** The provided shell script will guide you through installing the ***Kubernetes C client*** in the parent directory (please refer to the script for details).


### 3. Build the Tool
To build the Tool, follow these steps:

1. Open a Terminal Window.
2. Change to the directory where you have cloned the repository.
3. Run the follwing command:
  
   ```
    cd ../src/driver
    mkdir build && cd build
    cmake ..
    make
   ```

Build by making a build directory (i.e. `build/`), run `cmake` in `drive` directory, and then use `make` to build the target.
   
### 4. Run the Tool
To run the Tool, run the following command:

```
.\ConfigGenerator ../../
```


[Back to Table of Contents](#table-of-contents)



## Architecture

### High Level Architecture

<p align="center">
  <img src="static/Operation-flow.png" width="600">
</p>

The high-level architecture of PaaSFuzzer is engineered to detect reconciliation bugs within the scheduler component of container orchestration tools and cloud-based PaaS systems. It comprises three main seamlessly integrated components such as Driver Program, Fuzzing Scheduler and Monitoring System collaborating to efficiently uncover bugs.

### Driver Program

This component serves as the foundational program, encompassing a configuration file generator and a generator for program-related operations.

### Fuzzing Scheduler

This component orchestrates the scheduling of diverse policies, rules, and patches within the cluster and containers.

### Monitoring System

This component vigilantly observes the cluster, capturing the reflection time of each policy enforcement and patch orchestrated by the fuzzing scheduler component.

[Back to Table of Contents](#table-of-contents)



## Contributors

Principal Investigators:

- [Dr. Sunimal Rathnayake](https://sunimalr.staff.uom.lk) - University of Moratuwa <br>
- [Dr. Ridwan Shariffdeen](https://rshariffdeen.com) - National University of Singapore <br>


Developers:

- [Kabilan Mahathevan](https://github.com/KabilanMA) <br>
- [Tharsha Sivapalarajah](https://github.com/Tharsha-Sivapalarajah) <br>
- [Sivakajan Sivaparan](https://github.com/sivakajan-tech) <br>

[Back to Table of Contents](#table-of-contents)



## Contact

If you have any questions or comments about PaaSFuzzer, feel free to reach out to us directly or open an issue on [GitHub Issues](https://github.com/KabilanMA/paas-fuzzer/issues?q=is%3Aissue+is%3Aopen+sort%3Aupdated-desc).

[Back to Table of Contents](#table-of-contents)









# Fuzzing Cloud PaaS Platforms

### Description

Container-based cloud platforms provide scalability, efficiency, and reliability which made them developed or adopted by many Information Technology organisations. Minimised capital and operation cost, interoperability, compatibility, auto-elasticity on-demand availability of resources, ability to run on multiple platforms and multiple browsers, quicker to market, and multi-tenancy are the benefits of container-based PaaS cloud systems that created the path for increasing usage. However, this higher demand also leads to potential challenges such as security issues, elasticity issues, high availability issues, and multi-tenancy issues. Any vulnerabilities or bugs in these platforms can have wide-ranging and severe consequences for the services they support. Therefore, these issues have to be taken into account and solved before releasing these high-quality solutions to the customers and market.

Kubernetes stands as the forefront and preeminent container orchestration tool, proficiently overseeing containerized applications spanning multiple hosts. Numerous enterprises have constructed their Platform as a Service (PaaS) offerings atop Kubernetes, underscoring its pivotal role in the contemporary cloud landscape. Although Kubernetes does not qualify as a PaaS product, it serves as a foundational technology for developing PaaS applications, exemplified by platforms like Openshift. Given the widespread adoption of Kubernetes, a comprehensive examination and analysis of fixed and outstanding bugs within the Kubernetes ecosystem assume paramount significance in research and evaluation.

Developing a fuzzing framework for testing the scheduler module of container orchestration tools is crucial, given the increasing usage of orchestration tools in the industry and the evolving nature of the scheduler module. The absence of a dedicated fuzzer for the scheduler module in container orchestration tools and PaaS systems underscores the project’s importance. It not only plays a vital role in evaluating these tools but also opens avenues for future developments in the domain.

### Commands

`minikube start --extra-config=kubelet.allowed-unsafe-sysctls=net.core.somaxconn`


### Setup
Find the attachment in the `./setup guide` directory

### Methodology

![Methodology of Fuzzing Cloud PaaS Platforms](static/Operation-flow.png)

### Contributors

Principal Investigators:

- [Dr. Sunimal Rathnayake](https://sunimalr.staff.uom.lk) - University of Moratuwa <br>
- [Dr. Ridwan Shariffdeen](https://rshariffdeen.com) - National University of Singapore <br>


Developers:

- [Kabilan Mahathevan](https://github.com/KabilanMA) <br>
- [Tharsha Sivapalarajah](https://github.com/Tharsha-Sivapalarajah) <br>
- [Sivakajan Sivaparan](https://github.com/sivakajan-tech) <br>
