cmake_minimum_required(VERSION 3.10)

project(RTWeekendNoOOP)

set(CMAKE_CXX_STANDARD 14)

add_executable(RTWeekendNoOOP 
  ./src/main.cc ./src/material.cc ./src/hittable.cc
  ./src/ray.cc ./src/vec3.cc
)
