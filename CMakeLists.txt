cmake_minimum_required(VERSION 3.20)

project(nfa_to_min_pdfa_transform
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)

add_executable(startMe main.cpp src/nfa.cpp)