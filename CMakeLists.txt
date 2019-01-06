cmake_minimum_required(VERSION 3.10)
project(ProjectMC)

set(CMAKE_CXX_STANDARD 11)

add_executable(ProjectMC
        .idea/codeStyles/Project.xml
        .idea/misc.xml
        .idea/modules.xml
        .idea/ProjectMC.iml
        .idea/workspace.xml
        cmake-build-debug/cmake_install.cmake
        cmake-build-debug/Makefile
        cmake-build-debug/ProjectMC.cbp
        cmake-build-debug/ProjectMC.exe
        main.c)
