cmake_minimum_required (VERSION 3.10)
project("lab05")

include_directories("C:\\Program Files (x86)\\Microsoft SDKs\\MPI\\Include")
link_libraries("C:\\Program Files (x86)\\Microsoft SDKs\\MPI\\Lib\\x64\\msmpi.lib")

add_executable (hello-mpi hello-mpi.cpp)
#add_executable(hello-mpi hello-mpi.cpp)