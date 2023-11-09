cmake_minimum_required(VERSION 3.27)
project(funtools)

add_executable(cosetLeader cosetLeader.cpp)
target_compile_options(cosetLeader PRIVATE -std=c++23)